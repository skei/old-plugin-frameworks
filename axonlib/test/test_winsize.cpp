
#include "platform/axContext.h"
#include "axFormat.h"
#include "axEditor.h"

#include "gui/axWidget.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgSizer.h"

class myWidget : public axWidget
{
  public:
    myWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor( aCanvas->getColor(128,128,128) );
        aCanvas->fillRect( aRect.x, aRect.y, aRect.x2(), aRect.y2() );
        aCanvas->setPenColor( aCanvas->getColor(255,255,255) );
        aCanvas->drawLine(0,0,255,255);
        aCanvas->drawLine(255,0,0,255);
        aCanvas->setPenColor( aCanvas->getColor(0,255,0) );
        aCanvas->drawLine(255,0,255,255);
        aCanvas->drawLine(0,255,255,255);
        aCanvas->setPenColor( aCanvas->getColor(255,0,0) );
        aCanvas->drawLine(256,0,256,256);
        aCanvas->drawLine(0,256,256,256);

        axWidget::doPaint(aCanvas,aRect);
      }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axFormat,
                 public axWidgetListener
{
  private:
    axEditor* m_Editor;
    //wdgPanel* w_Main;
    myWidget* w_Main;
    wdgSizer* w_Sizer;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, AX_PLUG_DEFAULT)
      {
        describe("test_winsize","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(256,256);
      }

    virtual ~myPlugin()
      {
      }

    //--------------------------------------------------
    // do..
    //--------------------------------------------------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        m_Editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        m_Editor->appendWidget( w_Main = new myWidget(m_Editor,NULL_RECT,wa_Client) );
          w_Main->appendWidget( w_Sizer = new wdgSizer(m_Editor,axRect(10,10),wa_BottomRight, sm_Window) );
          w_Sizer->setTarget(m_Editor);
        m_Editor->doRealign();
        m_Editor->show();
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        m_Editor->hide();
        delete m_Editor;
        m_Editor = NULL;
      }

    //--------------------------------------------------
    // on..
    //--------------------------------------------------

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)

