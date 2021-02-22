#define AX_DEBUG
#include "core/axDebug.h"
#include "core/axMath.h"
#include "gui/axWidget.h"
#include "axFormat.h"
#include "axEditor.h"

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class myWidget : public axContainer
{
  private:
    bool  mDragging;
    int   mYclick;
    float mSensitivity;
    float mClickVal;

  public:

    myWidget(axWidgetListener* aListener, axRect aRect=NULL_RECT, int aAlignment=wa_None)
    : axContainer(aListener,aRect,aAlignment)
      {
        //mValue = 1;
        mDragging = false;
        mYclick = 0;
        mSensitivity = 0.005f;
        mClickVal = 0;
      }

    //virtual ~myWidget() {}

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        char buf[256];
        //if (mSkin)
        //{
          aCanvas->selectBrush(0);
          aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
          aCanvas->selectFont(0);
          if (mParameter) { mParameter->doGetDisplay(buf); }
          //else sprintf(buf,"%.3f",mValue);
          else axFtoa(buf,mValue);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),buf,ta_Center);
        //}
        axWidget::doPaint(aCanvas,aRect);
      }

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        trace("[" << this << "] doMouseDown x,y " << aXpos << "," << aYpos << " but " << aButton);
        //if (mValue>0.5) mValue=0; else mValue=1;
        mDragging = true;
        mYclick = aYpos;
        mClickVal = mValue;
        //mListener->onChange(this);
      }

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        trace("[" << this << "] doMouseUp x,y " << aXpos << "," << aYpos );
        mDragging = false;
        //mListener->onChange(this);
      }

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        trace("[" << this << "] doMouseMove x,y " << aXpos << "," << aYpos );
        if (mDragging)
        {
          float ydist = (float)(mYclick - aYpos);
          mValue = mClickVal + (ydist * mSensitivity);
          if (mValue>1) mValue=1;
          if (mValue<0) mValue=0;
          mListener->onChange(this);
        }
      }


};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    axFormat*   mPlugin;

    myWidget*   mWidget;
    //axDefSkin*  mSkin;
    myWidget*  mWidget2;
    //axSkin*     mSkin2;


  public:

    myEditor(axFormat* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin, aContext,aRect,aWinFlags)
      {
        mPlugin = aPlugin;
        appendWidget( mWidget   = new myWidget(this,axRect(10,10,200,100),wa_None) );
        mWidget->appendWidget( mWidget2  = new myWidget(this,axRect(20,20,100, 20),wa_None) );
        //trace("myEditor=" << this);
        //trace("mWidget=" << mWidget);
        //trace("mWidget2=" << mWidget2);
        connect(mWidget, mPlugin->param(0) );

        //mSkin = new axDefSkin(mCanvas);
        //this->setSkin(mSkin,true);
        //mSkin2 = new axSkin(mCanvas);
        //mSkin2->appendBrush( mCanvas->createBrush(AX_RED_DARK) );
        //mSkin2->appendFont( mCanvas->createFont("default",AX_YELLOW) );
        //mWidget2->setSkin(mSkin2);

        //mFlags.clearFlag(wf_Active);
      }

    //virtual ~myEditor()
    //  {
    //    // widgets are auto-deleted in axContainer
    //    delete mSkin;
    //  }

    virtual void doPaint(axCanvas* aCanvas,axRect aRect)
      {
        aCanvas->selectBrush(1);
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        axWidget::doPaint(aCanvas,aRect);
      }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axFormat
{
  private:
    myEditor*     mEditor;
    axParameter*  pGain;
    float         mGain;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext,AX_PLUG_DEFAULT)
      {
        mGain = 0.5;
        describe("axonlib2 build test","ccernn","axonlib build.cpp",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        setupEditor(512,384);
        appendParameter( pGain = new axParameter(this,"gain","db") );
        pGain->setValue(mGain);
        setupParameters();
      }

    //virtual ~myPlugin()
    //  {
    //    //parameters are auto-deleted in axFormatBase
    //  }

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        mEditor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        //mEditor->reparent((Window)aContxt->mWindow);
        mEditor->show();
        return mEditor;
      }

    virtual void doCloseEditor(void)
      {
        mEditor->hide();
        delete mEditor;
        mEditor = NULL;
      }

    //virtual void doIdleEditor(void) {}

    virtual void doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        trace("myPlugin.doSetParameter " << val);
        if (aParameter==pGain) mGain = val;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
//----------------------------------------------------------------------
