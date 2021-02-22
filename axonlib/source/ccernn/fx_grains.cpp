// we need alpha-blending of bitmaps/surfaces
#define AX_ALPHA

#include "base/axBase.h"
#include "gui/axEditor.h"
#include "core/axRand.h"
#include "par/parInteger.h"
#include "par/parFloat.h"
//#include "wdg/wdgPanel.h"
#include "wdg/wdgImage.h"
#include "wdg/wdgKnob.h"

#include "skins/axSkinDef.h"
#include "gui/axBitmapLoader.h"
#include "img/fx_grains_back.h"
#include "img/fx_grains_knob.h"

//#include "format/axReaperExt.h"

//----------------------------------------------------------------------

#define BUFFER_SIZE 4000000
#define MAX_GRAINS  256

char* str_onoff[] =
{
  (char*)"off",
  (char*)"on"
};

struct GRAIN
{
  int   active;
  float pos;
  float start;
  float end;
  float duration;
  float speed;
  float phase;
  float phase_add;
  float phase2;
  float phase2_add;
};

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class myVisual : public axWidget
{
  public:
    axColor   aBackColor;
    axColor   aIndexColor;
    axColor   aGrainColor;
    //myPlugin* plug;
  public:
    // setup in idle
    int     buffersize;
    int     index;
    int     numgrains;
    GRAIN*  grains;
  public:
    myVisual(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
    {
      grains = NULL;
      buffersize = 0;
      index = 0;
      numgrains = 0;
    }

//  /*

  // this is using quite a bit of cpu
  virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    {
      aCanvas->setBrushColor(aBackColor);
      aCanvas->fillRect(mRect.x, mRect.y, mRect.x2(), mRect.y2());
      if (grains && (buffersize>0))
      {
        float inv = 1.0f/(float)buffersize;
        aCanvas->setPenColor(aGrainColor);
        for ( int i=0; i<numgrains; i++ )
        {
          if( grains[i].active==1 )
          {
            float x = mRect.x + ( mRect.w * (grains[i].pos*inv) );
            aCanvas->drawLine((int)x, mRect.y, (int)x, mRect.y2());
          }
        }
        float x = mRect.x + ( mRect.w * ((float)index*inv) );
        aCanvas->setPenColor(aIndexColor);
        aCanvas->drawLine((int)x, mRect.y, (int)x, mRect.y2());
      }
    }

//    */

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

char* str_params[] = {
  (char*)"master", (char*)"grains", (char*)"bufsize", (char*)"freeze",
  (char*)"gr.dist", (char*)"gr.size", (char*)"gr.dur", (char*)"gr.pitch",
  (char*)"env", (char*)"gr.env",
  (char*)"dist.jit", (char*)"pitc.jit", (char*)"size.jit", (char*)"dur.jit" };

//----------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"fx_grains"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 14; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
    virtual bool          hasEditor(void)           { return true; }
    virtual axRect        getEditorRect(void)       { return axRect(0,0,340,325); }
};


class myInstance : public AX_INSTANCE
{
  friend class myVisual;
  private:
  // process
    float*      BUFFER;
    GRAIN       GRAINS[MAX_GRAINS];
    int         index;
    int         countdown;
  //'internal' parameters
    float       m_Master;
    int         m_NumGrains;
    float       m_BufferSize;
    float       m_Freeze;
    float       m_GrainDist;
    float       m_GrainSize;
    float       m_GrainDur;
    float       m_GrainPitch;
    float       m_Env;
    float       m_GrainEnv;
    float       m_StartJit;
    float       m_PitchJit;
    float       m_SizeJit;
    float       m_DurJit;
    float        _BufferSize;
    float        _GrainDist;
    float        _GrainSize;
    float        _GrainDur;

    axRand rand;
  //gui
    axEditor*       w_Editor;
    //wdgPanel*     w_Panel;
    wdgImage*       w_Panel;
    myVisual*       vis;
    bool            gui_initialized;
    axSkinDef*      m_Skin;
    axBitmapLoader* backloader;
    axBitmapLoader* knobloader;
    //axReaperExt*  reaper;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        axRand(418);
        BUFFER = new float[BUFFER_SIZE];
        axMemset(BUFFER,0,BUFFER_SIZE*sizeof(float));
        axMemset(GRAINS,0,MAX_GRAINS*sizeof(GRAIN));
        gui_initialized = false;
        index     = 0;
        countdown = 0;
        //describe("fx_grains","ccernn","axonlib example",0,AX_MAGIC+0x1004);
        //setupAudio(2,2);
        //setupEditor(340,325);
        appendParameter( new parFloatPow( this,"master volume",     "",   1,    0, 2,0,  2   ) );
        appendParameter( new parInteger(  this,"number of grains",  "",   10,   1, MAX_GRAINS ) );
        appendParameter( new parFloatPow( this,"buffer size",       "ms", 32,   1,64,0, 2 ) );
        appendParameter( new parInteger(  this,"freeze",            "",   0,    0, 1, str_onoff ) );
        appendParameter( new parFloatPow( this,"grain distance",    "ms", 2,    1,16,0, 2  ) );
        appendParameter( new parFloatPow( this,"grain size",        "ms", 4,    1, 8,0, 2   ) );
        appendParameter( new parFloatPow( this,"grain duration",    "ms", 16,   1,32,0, 2  ) );
        appendParameter( new parFloatPow( this,"grain pitch",       "",   1,    0, 2,0, 2   ) );
        appendParameter( new parFloat(    this,"envelope",          "",   1   ) );
        appendParameter( new parFloat(    this,"grain envelope",    "",   1   ) );
        appendParameter( new parFloatPow( this,"distance jitter",   "",   0.2,  0, 1,0, 3 ) );
        appendParameter( new parFloatPow( this,"pitch jitter",      "",   0.2,  0, 1,0, 3 ) );
        appendParameter( new parFloatPow( this,"size jitter",       "",   0.2,  0, 1,0, 3 ) );
        appendParameter( new parFloatPow( this,"duration jitter",   "",   0.2,  0, 1,0, 3 ) );
        setupParameters();
        setupPrograms();

/*

  this will probably crash non-reaper hosts, and standalone exe..
  haven't added much safeguarding yet.. and need to setup a
  getHostId/Name or something to find out which host we are currently
  running inside, so we can do host-specific things
  and, will probably expand this into a axHostVst class,

  thirde, it's called in the wrong place (constructor). a better place
  would be doProcessState, open or resume... so we're not calling
  this when the host is enumerating available plugins, only when
  actually inserted into a project..

  the first timw you try it, it might do weird tings with the Help menu :-)
  the second (and onward) time, it will add a separate 'axonlib' menu entry
  instead of modifying the 'Help'...

  just for testing!!!

*/

/* commented, so that it won't make any problems for non-reaper hosts

//--- test reaper extensions
audioMasterCallback audio = (audioMasterCallback)aContext->mAudio;
reaper = new axReaperExt(audio);
char temp[256];
reaper->GetProjectPath(temp,255);
trace( "reaper project path: " << temp );
reaper->AddCustomizableMenu("File","Axonlib_was_here!",NULL,true);
reaper->ShowMessageBox("testing reaper sdk extensions","axonlib",3);

// the 'axonlib' menu will remain after deletiong the plugin!

*/

      }

    //----------

    virtual ~myInstance()
      {
        delete BUFFER;
        if (gui_initialized)
        {
          delete backloader;
          delete knobloader;
        }

        //delete reaper;  // !!
      }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    virtual void* doOpenEditor(void* ptr)
      {
        if (!gui_initialized)
        {
          backloader = new axBitmapLoader();
          backloader->decodePng((unsigned char*)fx_grains_back,fx_grains_back_size);
          knobloader = new axBitmapLoader();
          knobloader->decodePng((unsigned char*)fx_grains_knob,fx_grains_knob_size);
          gui_initialized = true;
        }
        axEditor* editor = (axEditor*)mBase->getInterface()->createEditor(ptr,getEditorRect(),AX_WIN_DEFAULT);
        if (editor)
        {
          //axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
          axCanvas* canvas = editor->getCanvas();
          m_Skin = new axSkinDef(canvas);
          m_Skin->loadKnobBitmap(editor,(char*)knobloader->getImage(),32,(32*65),32);
          editor->applySkin(m_Skin);
          // widgets
          editor->appendWidget( w_Panel = new wdgImage(editor,NULL_RECT,wa_Client) );
          wdgKnob* k1;
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(230,255,100,32),wa_None,"master") );
          w_Panel->appendWidget(k1=new wdgKnob(editor,axRect( 10,155,100,32),wa_None,"num grains") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect( 10, 50,100,32),wa_None,"buf size") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect( 10, 85,100,32),wa_None,"freeze") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(120, 50,100,32),wa_None,"grain dist") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(120, 85,100,32),wa_None,"grain size") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(120,120,100,32),wa_None,"grain dur") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(120,155,100,32),wa_None,"grain pitch") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect( 10,255,100,32),wa_None,"dur env") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(120,255,100,32),wa_None,"grain env") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(230, 50,100,32),wa_None,"dist jit") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(230,155,100,32),wa_None,"pitch jit") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(230, 85,100,32),wa_None,"size jit") );
          w_Panel->appendWidget(   new wdgKnob(editor,axRect(230,120,100,32),wa_None,"dur jit") );
          k1->setSensitivity(0.001);
          for (int i=0; i<w_Panel->getNumWidgets(); i++)
            editor->connect( w_Panel->getWidget(i), getParameter(i) );
          w_Panel->loadBitmap(editor,(char*)backloader->getImage(),340,325, 24); // 32?
          // 'grain painter'
          w_Panel->appendWidget( vis = new myVisual(editor,axRect(11,203,319,34),wa_None)  );
          vis->aBackColor = canvas->getColor(64,64,64);
          vis->aIndexColor = canvas->getColor(255,0,0);
          vis->aGrainColor = canvas->getColor(128,128,128);
          vis->grains = GRAINS;
          editor->doRealign();
          w_Editor = editor;
        }
        editor->show();
        return editor;
      }

    virtual void doCloseEditor(void)
      {
        axEditor* editor = w_Editor;
        w_Editor = NULL;
        delete editor;
        delete m_Skin;
      }

    virtual void doIdleEditor(void)
      {
        //trace("fx_grains.doIdleEditor");
        vis->buffersize = m_BufferSize;
        vis->index = index;
        vis->numgrains = m_NumGrains;
        w_Editor->redrawWidget(vis);
        axInstance::doIdleEditor();
      }

    //--------------------------------------------------
    // plugin
    //--------------------------------------------------

    virtual void doSetParameter(axParameter* aParameter)
      {
        int idx = aParameter->getIndex();
        float v = aParameter->getValue();
        //trace("idx: " << idx << "v: " << v);
        //todo: do this:
        //mPlugin->setPar(idx,val);
        //instead of this:
        switch (idx)
        {
          case 0:   m_Master      = v;      break;
          case 1:   m_NumGrains   = (int)v; break;
          case 2:    _BufferSize  = v;      break;
          case 3:   m_Freeze      = v;      break;
          case 4:    _GrainDist   = v;      break;
          case 5:    _GrainSize   = v;      break;
          case 6:    _GrainDur    = v;      break;
          case 7:   m_GrainPitch  = v;      break;
          case 8:   m_Env         = v;      break;
          case 9:   m_GrainEnv    = v;      break;
          case 10:  m_StartJit    = v;      break;
          case 11:  m_PitchJit    = v;      break;
          case 12:  m_SizeJit     = v;      break;
          case 13:  m_DurJit      = v;      break;
        }
      }

    //----------

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        float ms  = getSampleRate() * 0.001;
        m_BufferSize = axMax(1,(_BufferSize*ms));;
        m_GrainDist  = _GrainDist  * ms;
        m_GrainSize  = _GrainSize  * ms;
        m_GrainDur   = _GrainDur   * ms;
        while (index>=(int)m_BufferSize) index -= (int)m_BufferSize;
        return false;
      }

    //----------

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        float spl0 = *aInputs[0];
        float spl1 = *aInputs[1];
        float in0 =  spl0;
        float in1 =  spl1;
        float out0 = 0;
        float out1 = 0;
        GRAIN* newgrain = NULL;
        GRAIN* G = GRAINS;
        // ----- update currently playing grains
        for( int i=0; i<m_NumGrains; i++ )
        {
          if( G->active==1 )
          {
            G->pos += G->speed;
            if (G->pos >= G->end) G->pos = G->start;
            if (G->pos >= m_BufferSize) G->pos -= m_BufferSize;
            //if( G->pos < 0 ) G->pos += m_BufferSize; // if we allow negative speeds
            G->phase += ( G->phase_add * 2 );
            if (G->phase >= 2) G->phase -= 2;
            G->phase2 += ( G->phase2_add * 2 );
            if (G->phase2 >= 2) G->phase2 -= 2;
            G->duration -= 1;
            if (G->duration <= 0) G->active = 0;
            float gvol = G->phase  * ( 2 - axAbs(G->phase ) );
            float dvol = G->phase2 * ( 2 - axAbs(G->phase2) );
            gvol = gvol*m_GrainEnv + (1-m_GrainEnv);
            dvol = dvol*m_Env      + (1-m_Env);
            int gpos = (int)G->pos * 2;
            out0 += BUFFER[gpos  ] * dvol * gvol;
            out1 += BUFFER[gpos+1] * dvol * gvol;
          } //on
          else if (!newgrain) newgrain = G;
          G += 1;//sizeof(GRAIN);
        } //for, numgrains
        // ----- time for a new grain?
        if( countdown <= 0 )
        {
          countdown = m_GrainDist;
          if( newgrain )
          {
            float startrnd = 1 + (m_StartJit   * rand.randSigned() ); // 0..2
            float pitchrnd = 1 + (m_PitchJit   * rand.randSigned() );
            float sizernd  = 1 + (m_SizeJit    * rand.randSigned() );
            float durrnd   = /*1 +*/ (m_DurJit * rand.randSigned() );
            float siz = sizernd * m_GrainSize;
            siz = axMax(1,siz);
            //float st = index + startrnd*m_BufferSize;
            float st = index + startrnd * m_GrainDur;//m_BufferSize;
            if (st>=m_BufferSize) st -= m_BufferSize;
            if (st< 0) st += m_BufferSize;
            float en = st + siz;
            if (en >= m_BufferSize ) en = m_BufferSize; // clamp
            if (en<0) en = 0;
            float du = m_GrainDur + durrnd * m_GrainDur;
            du = axMax(1,du);
            newgrain->active      = 1;
            newgrain->pos         = st;
            newgrain->start       = st;
            newgrain->end         = en;
            newgrain->duration    = du;
            newgrain->speed       = pitchrnd * m_GrainPitch;
            newgrain->phase       = 0;
            newgrain->phase_add   = 1 / siz;
            newgrain->phase2      = 0;
            newgrain->phase2_add  = 1 / du;
          } //newgrain
        } //countdwn
        countdown -= 1;
        // ----- house-keeping & buffering
        int index2 = index*2;
        if (m_Freeze<0.5) BUFFER[index2  ] = in0;
        if (m_Freeze<0.5) BUFFER[index2+1] = in1;
        index += 1;
        if( index >= m_BufferSize ) index -= m_BufferSize;
        float mmm = m_Master*m_Master*m_Master;
        *aOutputs[0] = out0 * mmm;
        *aOutputs[1] = out1 * mmm;
      }

};

//----------------------------------------------------------------------
AX_MAIN(myDescriptor,myInstance)
