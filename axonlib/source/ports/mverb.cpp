/*
  axonlib port of mverb (by martin eastwood)
  see 'extern/mverb/MVerb.h' for details and license (gpl)!
*/

#define AX_ALPHA
//#define AX_NOGUI

#define AX_DEBUG_MEM
#define AX_DEBUG_NEW
#define AX_DEBUG_CONSOLE
#define AX_DEBUG_LOG      "mverb.log"

//----------

//#include "format/axFormat.h"
#include "base/axBase.h"
#include <math.h>
#include <memory.h>
#include "../../extern/mverb/MVerb.h"

//#ifndef AX_NOGUI
//  #include "gui/axEditor.h"
//  #include "gui/axBitmapLoader.h"
//  #include "skins/axSkinDef.h"
//  #include "wdg/wdgImage.h"
//  #include "wdg/wdgKnob.h"
//  #include "mverb_background.h"
//  #include "mverb_knob.h"
//#endif //gui

//----------------------------------------------------------------------

// this is bad! this is shared among all instances
// and when changing program, we dump the parameters into this...
float prog1[] = { 0.0, 0.5, 1.0, 0.5, 0.0, 0.5, 1.0, 0.15, 0.75 }; // Subtle
float prog2[] = { 0.0, 0.5,	1.0, 0.5, 0.0, 1.0, 1.0, 0.35, 0.75 }; // Stadium
float prog3[] = { 0.0, 0.5, 1.0, 0.5, 0.0, 0.25,1.0, 0.35, 0.75 }; // Cupboard
float prog4[] = { 0.9, 0.5, 0.1, 0.5, 0.0, 0.5, 1.0, 0.5,  0.75 }; // Dark
float prog5[] = { 0.5, 0.5, 0.5, 0.5, 0.5, 1.0, 0.5, 0.5,  0.75 }; // Halves

//----------------------------------------------------------------------

char* str_params[] =
{
  (char*)"dampfreq",
  (char*)"density",
  (char*)"bwfreq",
  (char*)"decay",
  (char*)"predelay",
  (char*)"size",
  (char*)"gain",
  (char*)"mix",
  (char*)"earlymix"
};

//----------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"mverb"; }
    virtual char*         getAuthor(void)           { return (char*)"martin eastwood"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib port"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumProgs(void)         { return 5; }
    virtual int           getNumParams(void)        { return 9; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
    virtual bool          hasEditor(void)           { return true; }
    virtual axRect        getEditorRect(void)       { return axRect(0,0,456,108); }

};

//----------------------------------------------------------------------
//
// skin
//
//----------------------------------------------------------------------
#ifndef AX_NOGUI

  #include "gui/axEditor.h"
  #include "gui/axBitmapLoader.h"
  //#include "skins/axSkinDef.h"
  #include "gui/axSkin.h"
  #include "wdg/wdgImage.h"
  #include "wdg/wdgKnob.h"
  #include "mverb_background.h"
  #include "mverb_knob.h"

  #define KNOB_TEXTYOFFSET 35

class mverb_skin : public axSkin
{
  protected:
    bool        mKnobSrfLoaded;
    axSurface*  mKnobSrf;
    int         mKnobWidth;
    int         mKnobHeight;
    int         mKnobCount;
    axColor     mKnobTextCol;
    axColor     mKnobValCol;

  public:

    mverb_skin(axCanvas* aCanvas)
      {
        mKnobSrfLoaded    = false;
        mKnobSrf          = NULL;
        mKnobWidth        = 32;
        mKnobHeight       = 32;
        mKnobCount        = 129;
        mKnobTextCol      = aCanvas->getColor(0,0,0);
        mKnobValCol       = aCanvas->getColor(255,255,255);
      }

    virtual ~mverb_skin()
      {
        if (mKnobSrfLoaded) delete mKnobSrf;
      }

    virtual axBitmap* loadBitmap(axEditor* aEditor, char* aBuffer, int aWidth, int aHeight, int aDepth)
    {
      axBitmap* bitmap = aEditor->createBitmap(aWidth,aHeight,aDepth);
      bitmap->createBuffer(aBuffer);
      bitmap->convertRgbaBgra();
      bitmap->premultAlpha();
      bitmap->prepare();
      return bitmap;
    }

    virtual void loadKnobBitmap(axEditor* aEditor, char* aBuffer, int aWidth, int aHeight, int aDepth, int aNum=65)
      {
        axBitmap* bitmap = loadBitmap(aEditor,aBuffer,aWidth,aHeight,aDepth);
        mKnobSrf = aEditor->createSurface(aWidth,aHeight,aDepth);
        mKnobSrf->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,aWidth,aHeight);
        mKnobSrfLoaded = true;
        delete bitmap;
      }

    //TODO drawPanel

    virtual void drawKnob(axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue, int aMode=0)
      {
        if (mKnobSrf)
        {
          int index = (int)axFloor(aValue*mKnobCount);
          index = axMinInt(index,mKnobCount-1);
          int ky = mKnobHeight * index;
          aCanvas->renderSurface(mKnobSrf,aRect.x,aRect.y, 0,ky,mKnobWidth,mKnobHeight);
          int y  = aRect.y + KNOB_TEXTYOFFSET;
          int th = aCanvas->textHeight("Xj");
          aCanvas->setTextColor(mKnobValCol);
          aCanvas->drawText(aRect.x,y,aRect.x2(),y+th,aDisp,ta_Center);
        }
      }
};

#endif //gui
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//class myPlugin : public axFormat
class myInstance : public AX_INSTANCE
{
  private:
    MVerb<float>    em_verb;
  #ifndef AX_NOGUI
  private:
    bool            m_GuiInitialized;
    axEditor*       m_Editor;
    mverb_skin*     m_Skin;
    wdgImage*       w_Panel;
    axBitmapLoader* m_BackLoader;
    axBitmapLoader* m_KnobLoader;
  #endif //gui

  public:

    //myPlugin(axContext* aContext, int aFormatFlags)
    //: axFormat(aContext,aFormatFlags)
    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        #ifndef AX_NOGUI
          m_GuiInitialized = false;
        #endif //gui
        //float prog1[] = { 0.0, 0.5, 1.0, 0.5, 0.0, 0.5, 1.0, 0.15, 0.75 }; // Subtle
        appendParameter( new axParameter(this,"damping freq", "",0.0 ) );
        appendParameter( new axParameter(this,"density",      "",0.5 ) );
        appendParameter( new axParameter(this,"bandwidthfreq","",1.0 ) );
        appendParameter( new axParameter(this,"decay",        "",0.5 ) );
        appendParameter( new axParameter(this,"predelay",     "",0.0 ) );
        appendParameter( new axParameter(this,"size",         "",0.5 ) );
        appendParameter( new axParameter(this,"gain",         "",1.0 ) );
        appendParameter( new axParameter(this,"mix",          "",0.15) );
        appendParameter( new axParameter(this,"earlymix",     "",0.75) );
        setupParameters();
        appendProgram( new axProgram("subtle",  9, prog1) );
        appendProgram( new axProgram("stadium", 9, prog2) );
        appendProgram( new axProgram("cupboard",9, prog3) );
        appendProgram( new axProgram("dark",    9, prog4) );
        appendProgram( new axProgram("halves",  9, prog5) );
        setupPrograms();
      }

    virtual ~myInstance()
      {
        #ifndef AX_NOGUI
        if (m_GuiInitialized)
        {
          delete m_BackLoader;
          delete m_KnobLoader;
        }
        #endif //gui
      }

    virtual void doPreProgram(int aProgram)
      {
        if (getCurrentProgram() != aProgram)    // should this be handled lower down, so we don't
          saveProgram( getCurrentProgram() );   // even get the the call if changing to same program?
      }                                         // could be handy, to allow "revert..."

    virtual void doStateChange(int aState)
      {
        switch (aState)
        {
          case is_Resume:
            //trace("--- resume ---");
            transferParameters();
            em_verb.setSampleRate(getSampleRate());
            break;
        }
      }

    virtual void doSetParameter(axParameter* aParameter)
      {
        //trace("doSetParameter");
        int   index = aParameter->getIndex();
        float value = aParameter->getValue();
        //trace("doSetParameter(" << index << ") = " << value);
        em_verb.setParameter(index,value);
        //trace(getSampleRate());
      }

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        //trace( aInputs[0] << "," << aInputs[1] << "," << aOutputs[0] << "," << aOutputs[1] << " - " << aSize);
        em_verb.process(aInputs,aOutputs,aSize);
        return true;
      }

#ifndef AX_NOGUI

    //virtual axWindow* doOpenEditor(axContext* aContext)
    virtual void* doOpenEditor(void* ptr)
      {
        if (!m_GuiInitialized)
        {
          m_BackLoader = new axBitmapLoader();
          m_BackLoader->decodePng((unsigned char*)mverb_background,mverb_background_size);
          m_KnobLoader = new axBitmapLoader();
          m_KnobLoader->decodePng((unsigned char*)mverb_knob,mverb_knob_size);
          m_GuiInitialized = true;
        }
        //axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        axEditor* editor = (axEditor*)mBase->getInterface()->createEditor(ptr,getEditorRect(),AX_WIN_DEFAULT);
        if (editor)
        {
          // skin
          axCanvas* canvas = editor->getCanvas();
          m_Skin = new mverb_skin(canvas);
          m_Skin->loadKnobBitmap(editor,(char*)m_KnobLoader->getImage(),32,(32*129),32);  // bitmap depth
          editor->applySkin(m_Skin);
          // background image
          editor->appendWidget( w_Panel = new wdgImage(editor,NULL_RECT,wa_Client) );
          w_Panel->loadBitmap(editor,(char*)m_BackLoader->getImage(),456,108, 24);        // screen depth
          // knobs
          // hopefully this is in order now :-)
          w_Panel->appendWidget( new wdgKnob( editor,axRect(336,40,40,64),wa_None) );
          w_Panel->appendWidget( new wdgKnob( editor,axRect(216,40,40,64),wa_None) );
          w_Panel->appendWidget( new wdgKnob( editor,axRect(256,40,40,64),wa_None) );
          w_Panel->appendWidget( new wdgKnob( editor,axRect(296,40,40,64),wa_None) );
          w_Panel->appendWidget( new wdgKnob( editor,axRect(96, 40,40,64),wa_None) );
          w_Panel->appendWidget( new wdgKnob( editor,axRect(176,40,40,64),wa_None) );
          w_Panel->appendWidget( new wdgKnob( editor,axRect(376,40,40,64),wa_None) );
          w_Panel->appendWidget( new wdgKnob( editor,axRect(56, 40,40,64),wa_None) );
          w_Panel->appendWidget( new wdgKnob( editor,axRect(136,40,40,64),wa_None) );
          // setup
          //for (int i=0; i<9; i++) editor->connect( w_Panel->getWidget(i), mParameters[i] );
          for (int i=0; i<9; i++) editor->connect( w_Panel->getWidget(i), getParameter(i) );
          editor->doRealign();
          editor->show();
        }
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
        delete m_Skin;
      }

#endif //gui

};

//AX_ENTRYPOINT(myPlugin)
AX_MAIN(myDescriptor,myInstance)
