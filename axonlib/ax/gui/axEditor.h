/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axEditor_included
#define axEditor_included
//----------------------------------------------------------------------
/*
  takes care of two things:
  - widget/parameter connections
      not all parameters are connected to a widget (gui-less pluginas)
      not all widgets are connected to parameters (combo-boxes, menus, resizer,. )
      so this is where we connect everything.
  - window resizing
      embedded windowd have no window borders or titlebar,
      so we 'cheat' by resizing the window-widget ourselves,
      forcing a reAlign-ment
      and eventually notify the vst-host and os about the change
      note that nort all hosts support resizing of the editor,
      and other hosts might require you to close and reopen the
      editor for the new size to take effect
  - and contains a mDefaultSkin

*/

#include "core/axArray.h"
#include "gui/axWindow.h"
#include "skins/axSkinBasic.h"

//----------------------------------------------------------------------
//
// connection
//
//----------------------------------------------------------------------

// widget <-> parameter connection
struct wp_connection
{
  axWidget*     mWidget;
  axParameter*  mParameter;
  wp_connection(axWidget* aWidget, axParameter* aParameter)
    {
      mWidget=aWidget;
      mParameter=aParameter;
    }
};

typedef axArray<wp_connection> wp_connections;

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class axEditor : public axWindow
{
  private:
    axBase*         mBase;
    axInstance*     mInstance;
  protected:
    axInterface*    mInterface;
    wp_connections  mConnections;
    axSkinBasic*    mDefaultSkin;

  public:

  // aFormat   = 'owner' (plugin or exe)
  // aContext  = the aContext you receive in doOpenEditor
  // aRect     = pos & size of window
  // aWinFlags = flags to pass on to (os-specific) window
  //             defines double-buffering, message--threading, window-parenting

    axEditor(axBase* aBase, void* aParent, axRect aRect, int aWinFlags)
    : axWindow(aBase,aParent,aRect,aWinFlags)
      {
        //trace("axEditor");
        //mFormat = aFormat;
        mBase = aBase;
        axCanvas* canvas = getCanvas();
        mDefaultSkin = new axSkinBasic(canvas);
        applySkin(mDefaultSkin);

      }

    //----------

    virtual ~axEditor()
      {
        delete mDefaultSkin;
      }

    //--------------------------------------------------

    // called from doOpenEditor
    inline void setInstance(axInstance* aInstance) { mInstance=aInstance; }

    inline axSkin* defaultSkin(void) { return mDefaultSkin; }

    //--------------------------------------------------

    void connect(axWidget* aWidget, axParameter* aParameter)
      {
        int conn = mConnections.size();
        aWidget->setConnection(conn);
        aWidget->setParameter(aParameter);
        aParameter->setConnection(conn);
        aWidget->setValue( aParameter->doGetValue() );
        //TODO: fix!!
        mConnections.append( wp_connection(aWidget,aParameter) );   // ??? delete?
      }

    //void deleteConnections(void)
    //  {
    //    for (int i=0; i<mConnections.size(); i++) delete
    //  }

    //--------------------------------------------------

    //[internal]
    inline void internal_redraw(axWidget* aWidget)
    //inline void internal_redraw(axWidget* aWidget, bool fromgui/*=false*/)
      {
        //trace("internal_redraw");
        #ifdef AX_FORMAT_EXE
          redrawWidget(aWidget);
        #else
          #ifndef AX_WIDGET_NOUPDATELIST
            appendUpdate(aWidget);
          #else
            redrawWidget(aWidget);
          #endif
        #endif
      }

    //----------

    //virtual void redrawAll(void)                        { invalidate( mRect.x, mRect.y, mRect.x2(), mRect.y2() ); }
    //virtual void redrawRect(axRect aRect)               { invalidate( aRect.x, aRect.y, aRect.x2(), aRect.y2() ); }
    //virtual void redrawWidget(axWidget* aWidget)        { redrawRect(aWidget->getRect()); }

    //----------

    // called from axFormat.onChange(axParameter) if editor is open
    //
    // when the vast host automates a parameter, the parameter's onChange is
    // called (via axFormatVst.setParameter)..
    // this func is then called to redraw the widget.
    //
    // automation can theoretically come at any time, so we shouldn't just
    // blindly redraw the widget..
    // the internal_redraw function either sends an 'invalidate' notification
    // to the window/editor, or append the widget to an updatelist, and then this
    // will be redrawn in the next editor idle calls...

    virtual void paramChanged(axParameter* aParameter)
      {
        //trace("paramChanged");
        int conn = aParameter->getConnection();
        if (conn>=0)
        {
          axWidget* wdg = mConnections[conn].mWidget;
          float val = aParameter->doGetValue();
          wdg->setValue(val);
          internal_redraw(wdg); //redrawWidget(wdg);
        }
      }

    //----------

      // this one needs more work. specific hosts might need specific actions...
      // - axWindow.setSize()
      // - set size of rect that will be returned in effEditGetRect
      // - vst function sizeWindow()
      // various hosts react differently to these, and we need to do a host-check at startup,
      // and do more testing to see which host require what.

    void resizeWindow(int aWidth, int aHeight)
      {
        //trace("axEditor.resizeWindow: " << aWidth << "," << aHeight);
        mRect.w = aWidth;
        mRect.h = aHeight;
        // axWindow::setSize not needed for reaper, (effEditRect)
        // but if commented out, our window woon't be told to resize
        // same thing with energy xt2 (it supports sizeWindow
        // (only if host doesn't do any resizing?)
        // standalone needs it..
        // todo: #ifdef AX_FORMAT_...
        //#ifdef AX_FORMAT_EXE
        axWindow::setSize(aWidth,aHeight); // double resize!
        //#endif
        //#ifdef AX_FORMAT_VST
        //resizeBuffer(aWidth,aHeight);
        //doSetSize(aWidth,aHeight);
        //#endif
//        if (mInstance) mInstance->notifyResizeEditor(aWidth,aHeight);
      }

    //--------------------------------------------------
    // do..
    //--------------------------------------------------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        //trace("axEditor.doSetSize: " << aWidth << "," << aHeight);
//        if (mInstance) mInstance->notifyResizeEditor(aWidth,aHeight);
        axWindow::doSetSize(aWidth,aHeight);
      }

    //--------------------------------------------------
    // on..
    //--------------------------------------------------

    //TODO: if tweaked from gui, redraw directly
    // else append to updatelist

    // this is called from a widget when its internal state has changed.
    // we notify the parameter, if there's any associated with the widget,
    // and redraw the widget

    virtual void onChange(axWidget* aWidget)
      {
        //trace("onChange wdg");
        int conn = aWidget->getConnection();
        //trace("  conn: " << conn);
        if (conn>=0)
        {
          //trace("...connected...");
          axParameter* par = mConnections[conn].mParameter;
          float val = aWidget->getValue();
          //trace("  mInstance: " << mInstance);
//          if (mInstance) mInstance->notifyParamChanged(par);
          //par->doSetValue(val,false);
          par->doSetValue(val,true); // true = notify listener
        }
        // since exe's don't have parameters, there won't be
        // any incoming onChange as the result of the above
        // doSetValue, so we redraw directly here...
        #ifdef AX_FORMAT_LIB
        else
        #endif
          redrawWidget(aWidget);
      }

    //----------

    virtual void onRedraw(axWidget* aWidget)
      {
        //trace("onRedraw");
        internal_redraw(aWidget);
      }

    //----------

    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY, int aMode)
      {
        if (aMode<0)
        {
          axRect R = mInstance->getEditorRect();
          int w = R.w + aDeltaX; //mRect.w + aDeltaX;
          int h = R.h + aDeltaY; //mRect.h + aDeltaY;
          //axWidget::doSetSize(w,h);
          //resizeWindow( R.w + aDeltaX, R.h + aDeltaY );
          resizeWindow(w,h);
        }
        else axWindow::onSize(aWidget,aDeltaX,aDeltaY,aMode);
      }


};

//----------------------------------------------------------------------
#endif

