//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG  "test_render.log"

#define AX_ALPHA

#include "format/axFormat.h"
#include "gui/axEditor.h"
#include "core/axRand.h"
#include "gui/axBitmapRender.h"

//----------------------------------------------------------------------

// in release-build, i can render around 200.000 lines per second
// on my laptop (in win7)

#define NUM_LINES  1000
#define NUM_POLYS  10
#define POLY_ALPHA 128

class myEditor : public axEditor
{
  private:
    axBitmap*         m_Bitmap;
    axBitmap*         m_Texture;
    axBitmapRender*   m_Render;
    axPolyVertex      VTX[4];
  public:
    myEditor(axFormat* aFormat, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aFormat,aContext,aRect,aWinFlags)
      {
        axRand(418);
        //m_Bitmap = createBitmap(256,256,32);
        m_Bitmap = createBitmap(256,256,24);
        m_Bitmap->createBuffer();
        m_Bitmap->prepare();
        m_Render = new axBitmapRender();
        m_Render->setup(m_Bitmap);

        m_Texture = createBitmap(64,64,32);
        m_Texture->createBuffer();
        m_Texture->clearBuffer();
        //m_Texture->fillBuffer(0x20ff7f00);
        for (int x=0; x<64; x++)
        {
          for (int y=0; y<64; y++)
          {
            unsigned int col = m_Texture->makeColor(x*4,0,0,255-(y*4)/*POLY_ALPHA*/); // r,g,b,a
            m_Texture->setPixel(x,y,col);
          }
        }
        VTX[0].z=1; VTX[0].u=1;  VTX[0].v=1;  VTX[0].next=&VTX[1];
        VTX[1].z=1; VTX[1].u=62; VTX[1].v=1;  VTX[1].next=&VTX[2];
        VTX[2].z=1; VTX[2].u=62; VTX[2].v=62; VTX[2].next=NULL;
      }
    virtual ~myEditor()
      {
        delete m_Render;
        delete m_Bitmap;
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        // bitmap size!
        int w = 256-1;//aRect.w;
        int h = 256-1;//aRect.h;
        int i;
        m_Bitmap->clearBuffer();
        for (i=0; i<NUM_LINES; i++)
        {
          unsigned int col = m_Texture->makeColor(axRandInt(255),axRandInt(255),axRandInt(255),axRandInt(255));
          m_Render->drawLineAA(axRandInt(w),axRandInt(h),axRandInt(w),axRandInt(h),col);
        }

        //unsigned long long startValue = axRdtsc();
        for (i=0; i<NUM_POLYS; i++)
        {
          VTX[0].x=axRandInt(w); VTX[0].y=axRandInt(h);
          VTX[1].x=axRandInt(w); VTX[1].y=axRandInt(h);
          VTX[2].x=axRandInt(w); VTX[2].y=axRandInt(h);
          m_Render->drawPolyTex(VTX,m_Texture,true);
        }
        aCanvas->drawBitmap(m_Bitmap,0,0, 0,0,256,256);
        //unsigned long long endValue = axRdtsc();
        //unsigned long long diff = endValue - startValue;
        //char temp[32];
        //axItoa(temp,diff,8);
        //aCanvas->setTextColor( aCanvas->getColor(255,255,255));
        //aCanvas->drawText(10,10,temp);
      }
};

//----------------------------------------------------------------------

class myPlugin : public axFormat
{

    //----------------------------------------
    // plugin
    //----------------------------------------

  private:
    myEditor* m_Editor;

  public:

    myPlugin(axContext* aContext, int aFlags)
    : axFormat(aContext/*, pf_HasEditor*/)
      {
        describe("test_render","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2/*,false*/);
        setupEditor(256,256);
        setupParameters();
      }

    virtual ~myPlugin()
      {
      }

    //----------------------------------------
    // editor
    //----------------------------------------

  public:

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
        myEditor* editor = m_Editor;
        m_Editor = NULL;
        delete editor;
      }

    //----------

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
