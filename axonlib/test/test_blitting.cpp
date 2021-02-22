//#define AX_NO_MALLOC
//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_LOG  "test_gain_gui_skin.log"

#define AX_ALPHA
//#define BPP 24
#define BPP 32
// blending only with BPP=32 & AX_ALPHA

// frames per second, very, very, non-scientific,
// times measured by counting seconds from the sound on a wall-clock :-)

//---------- linux/ubuntu, ati mobility radeon
// - = xserver crash

// 64*64
//                    24    32
// drawBitmap         0.6   -
// drawImage          3.5   -
// renderImage        2.5   1
// stretchImage       1/13  1/13
//
// 32*32
//                    24    32
// drawBitmap         3     -
// drawImage          4.5   -
// renderImage        3     2
// stretchImage       1/11  1/12

//----------  wine (win32), same laptop

// 64*64
//                    24    32
// drawBitmap         0.2   0.2
// drawImage          3.5   1.5
// renderImage        *     0.2
// stretchImage       *     1/19
//
// ...

//---------- win7 same laptop
// * = black screen

// 64*64
//                    24    32
// drawBitmap         1     1.5
// drawImage          3     3
// renderImage        *     0.4
// stretchImage       *     1/10
//
// 32*32
//                    24    32
// drawBitmap
// drawImage
// renderImage
// stretchImage


#include "axFormat.h"
#include "axEditor.h"

//#include "gui/axBitmap.h"
//#include "gui/axSurface.h"
//#include "gui/axCanvas.h"

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    axBitmap*   m_Bitmap;
    axSurface*  m_Surface;
    axColor     m_Black;
  public:
    myEditor(axFormat* aFormat, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aFormat,aContext,aRect,aWinFlags)
      {
        axCanvas* canvas = getCanvas();
        m_Black = canvas->getColor(0,0,0);
        m_Bitmap = createBitmap(64,64,BPP);
        m_Bitmap->prepare();
        //int width = m_Bitmap->getWidth();
        //int height = m_Bitmap->getHeight();
        char* buffer = m_Bitmap->getBuffer();
        unsigned char* ptr = (unsigned char*)buffer;
        int x,y;
        for (y=0; y<64; y++)
        {
          for (x=0; x<64; x++)
          {
            *ptr++ = 0;           // b
            *ptr++ = x*4;         // g
            *ptr++ = x*4;         // r
            *ptr++ = (x*y) >> 4;  // a
          }
        }
        m_Bitmap->premultAlpha();
        m_Surface = createSurface(64,64,BPP);
        m_Surface->getCanvas()->drawBitmap(m_Bitmap, 0,0, 0,0,63,63);
        startTimer(40);   // 25 fps
      }

    virtual ~myEditor()
      {
        stopTimer();
        delete m_Bitmap;
        delete m_Surface;
      }

    virtual void doTimer(void)
      {
        redrawAll();
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(m_Black);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
        for (int i=0; i<10000; i++)
          {
            int x = axRandInt(256);
            int y = axRandInt(256);
            //aCanvas->drawBitmap(m_Bitmap, x,y, 0,0,63,63);                // 1.5 sec per frame
            //aCanvas->drawImage(m_Surface, x,y, 0,0,63,63);                // 3.5 frames per sec
            //aCanvas->renderImage(m_Surface, x,y, 0,0,63,63);              // 2.5 frames per sec
            aCanvas->stretchImage(m_Surface, x,y,x+31,y+31, 0,0,63,63);   // 14 sec per frame!!

          }
      }

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

class myPlugin : public axFormat
{
  private:

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, pf_HasEditor)
      {
        describe("test_blitting","ccernn","test_blitting",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(256,256);
      }

    virtual ~myPlugin()
      {
        //axDstdDestroy();
        //axDwinDestroy();
      }

    //----------------------------------------

  private:
    myEditor* m_Editor;

  public:

    // from axFormatVst.dispatcher  -  context: HWND
    // axFormatExe.main = main  -  context: instance, winname

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        myEditor* editor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        //axCanvas* canvas = editor->getCanvas();
        editor->show();
        m_Editor = editor;
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        m_Editor->hide();
        axEditor* editor = m_Editor;
        m_Editor = NULL;
        delete editor;
      }

    //----------

    //virtual void doIdleEditor()
    //  {
    //    m_Editor->idle();
    //  }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
