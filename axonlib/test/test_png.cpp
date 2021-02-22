//#define AX_NO_MALLOC
//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_LOG  "test_png.log"

#include "axFormat.h"
#include "axEditor.h"

#include "wdg/wdgImage.h"
//#include "../extern/picopng.h"
#include "gui/axBitmapLoader.h"

//#include "testpng.h"
#include "testpng2.h"

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axFormat,
                 public axWidgetListener
{
  private:
    //axPngInfo* pnginfo;
    //axEditor*   m_Editor;
    //wdgImage*   w_Image;
    //axSurface*  surface;
    //axBitmap*   bitmap;
  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, pf_None/*AX_PLUG_DEFAULT*/)
      {
        describe("test_winsize","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        //setupEditor(256,256);
        axBitmapLoader loader;
        //loader.decodePng((unsigned char*)testpng,testpng_size);
        loader.decodePng((unsigned char*)testpng2,testpng2_size);
        //pnginfo = axPngDecode(testpng,testpng_size);

      }

    virtual ~myPlugin()
      {
        axPngFreeAll();
      }

    //--------------------------------------------------
    // do..
    //--------------------------------------------------

/*

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        m_Editor  = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        int   w   = pnginfo->width;
        int   h   = pnginfo->height;
        char* buf = (char*)pnginfo->image->data;
        bitmap = m_Editor->createBitmap(w,h);
        bitmap->createBuffer(buf);                    // create bitmap buffer & copy data
        bitmap->convertRgbaBgra();                    // -> bgr.a
        bitmap->prepare();                            // prepare bitmap for blitting
        surface = m_Editor->createSurface(w,h);
        axCanvas* canvas = surface->getCanvas();
        canvas->drawBitmap(bitmap,0,0,0,0,w,h);   // upload to surface
        delete bitmap;

        m_Editor->appendWidget( w_Image = new wdgImage(m_Editor,axRect(0,0,256,256),wa_None,surface) );
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
        delete surface;
        //delete bitmap;
      }

*/

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)

