#include "format/axFormat.h"
#include "par/parInteger.h"
#include "par/parFloat.h"

#include "gui/axEditor.h"
#include "gui/axBitmapLoader.h"
#include "wdg/wdgBitmap.h"

#include "core/axCompress.h"

#define AX_ALPHA
#define TEST_COMPRESSION
#define NUMSHIFT    7
#define BUFFERSIZE  (1024*1024*4)
#define WIDTH       256
#define HEIGHT      256
#define FILENAME    "img/ccernn2.png"

unsigned char buf1[BUFFERSIZE];
unsigned char buf2[BUFFERSIZE];
unsigned char buf3[BUFFERSIZE];

//---------------------------------------------------------------------

class test_dmc : public axFormat
{
  private:
  private:
    axEditor*       w_Editor;
    wdgBitmap*      w_Bitmap;
    axBitmap*       m_Bitmap;
    axBitmapLoader  m_Png;
//    #ifdef TEST_COMPRESSION
//    axCompress      m_Dmc;
//    #endif

  //--------------------------------------------------

  public:

    test_dmc(axContext* aContext, int aFlags)
    : axFormat(aContext)
      {
        w_Editor = NULL;
        describe("test_dmc","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        //setupEditor(256,256);
        setupEditor(WIDTH,HEIGHT);

        int res = m_Png.decodeLoadPng(FILENAME);
        trace("decode = " << res);
        trace("width: "   << m_Png.getWidth());
        trace("height: "  << m_Png.getHeight());
        trace("image: "   << hex << (unsigned int)m_Png.getImage() << dec);

        //--------------------------------------------------
        #ifdef TEST_COMPRESSION
        axCompress m_Dmc;
        char* img = (char*)m_Png.getImage();
        int size  = m_Png.getWidth() * m_Png.getHeight() * 4;

        trace("original size: " << size);
        axMemcpy(buf1,img,size);
        //m_Dmc.fill_layer(   (char*)buf1,size,3,255);
        m_Dmc.shift_layers( (char*)buf1,size,-NUMSHIFT);
        m_Dmc.split_layers( (char*)buf1,size);
        m_Dmc.delta_encode( (char*)buf1,size);
        //int rle = m_Dmc.rle_encode(buf2,buf1,size);       trace("rle: " << rle);
        int csize = m_Dmc.compress(buf3,buf1,size);        trace("compress: " << csize);

//        axFileWrite("img/ccernn2.dmc",(char*)buf3,csize);

        m_Dmc.expand(    buf2,buf3,csize);
        //m_Dmc.rle_decode(buf3,buf2,rle);
        axMemcpy(buf3,buf2,size);
        m_Dmc.delta_decode((char*)buf3,size);
        m_Dmc.join_layers( (char*)buf3,size);
        m_Dmc.shift_layers( (char*)buf3,size,NUMSHIFT);

        #endif
        //--------------------------------------------------

      }

    virtual ~test_dmc()
      {
      }

    //----------------------------------------
    // gui
    //----------------------------------------

  public:

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        editor->appendWidget( w_Bitmap = new wdgBitmap(editor,NULL_RECT,wa_Client));
        //editor->setBorders(10,10,5,5);
        editor->doRealign();
        //transferParameters();

        //m_Bitmap = editor->createBitmap(256,256,24);    // !!!!!
        m_Bitmap = editor->createBitmap(WIDTH,HEIGHT,24);    // !!!!!

        #ifdef TEST_COMPRESSION
        m_Bitmap->createBuffer((char*)buf3);
        #else
        m_Bitmap->createBuffer((char*)m_Png.getImage());
        #endif
        m_Bitmap->convertRgbaBgra();
        m_Bitmap->setBackground(255,255,255);
        //m_Bitmap->setBackground(0,0,0);
        m_Bitmap->prepare();

        w_Bitmap->setBitmap(m_Bitmap);
        editor->show();
        w_Editor = editor;
        return editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        axEditor* editor = w_Editor;
        w_Editor->hide();
        w_Editor = NULL;
        delete editor;
        delete m_Bitmap;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(test_dmc)
