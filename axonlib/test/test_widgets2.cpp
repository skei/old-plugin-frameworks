#define AX_DEBUG
#define AX_DEBUG_AUTO_STD

#include "axFormat.h"
#include "axEditor.h"

#include "gui/axSymbols.h"

#include "wdg/wdgPanel.h"
#include "wdg/wdgSizer.h"
#include "wdg/wdgLabel.h"
#include "wdg/wdgButton.h"
#include "wdg/wdgButtons.h"
#include "wdg/wdgSlider.h"
#include "wdg/wdgValue.h"
#include "wdg/wdgImage.h"
#include "wdg/wdgBitmap.h"
#include "wdg/wdgKnob.h"
#include "wdg/wdgScrollBar.h"
#include "wdg/wdgScrollBox.h"
#include "wdg/wdgGroupBox.h"

//----------------------------------------------------------------------

class mySB : public wdgScrollBox//axWidget
{
  public:
    mySB(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    //: axWidget(aListener,aRect,aAlignment)
    : wdgScrollBox(aListener,aRect,aAlignment)
      {
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
//        trace( "mRect: " << wContainer->getRect().x
//                  << "," << wContainer->getRect().y
//                  << "," << wContainer->getRect().w
//                  << "," << wContainer->getRect().h);
        aCanvas->setBrushColor( aCanvas->getColor(0,128,0) );
        aCanvas->fillRect(mRect.x, mRect.y, mRect.x2(), mRect.y2() );
        wdgScrollBox::doPaint(aCanvas,aRect);
      }

};

//----------------------------------------------------------------------
class myEditor : public axEditor
{
  friend class myPlugin;
  private:
    wdgPanel*     w_MainPanel;
    wdgScrollBox* sb1;
    wdgButtons*   b1;
  public:

    myEditor(axFormat* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
      {
        appendWidget( w_MainPanel = new wdgPanel(this,NULL_RECT,wa_Client) );

        w_MainPanel->appendWidget( sb1 = new mySB(this,axRect( 100,100,200,200),wa_Left) );

        //sb1->appendWidget( new myWidget(this,NULL_RECT,wa_Client) );
        //sb2->appendWidget( new myWidget(this,NULL_RECT,wa_Client) );

        wdgPanel* pan1;
        sb1->appendWidget( pan1 = new wdgPanel(this,axRect(0,300),wa_Top) );

        for (int i=0; i<50; i++)
        {
          int x = axRandomInt(199);
          int y = axRandomInt(199);
          int w = axRandomInt(100);
          int h = axRandomInt(100);
          pan1->appendWidget( new wdgButton(this,axRect(x,y,w,h),wa_StackedHoriz,false,"off","on") );
        }

        //sb1->appendWidget( b1 = new wdgButtons(this,axRect(0,200),wa_Top) );

        doRealign();
      }

    //----------

    //virtual ~myEditor(void)
    //  {
    //  }

    //----------

    //virtual void onChange(axWidget* aWidget)
    //  {
    //    //trace("myEditor.onChange");
    //    axEditor::onChange(aWidget);
    //  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axFormat
{
  private:
    myEditor* mEditor;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, AX_PLUG_DEFAULT)
      {
        axRandomize(418);
        describe("test_widgets","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(640,480);
      }

    //----------

    virtual ~myPlugin()
      {
      }

    //----------------------------------------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        //wtrace(":: doOpenEditor");
        mEditor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        mEditor->show();
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        //wtrace(":: doCloseEditor");
        mEditor->hide();
        delete mEditor;
        mEditor = NULL;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
