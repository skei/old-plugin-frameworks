#define AX_NOGUI

//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_NEW
//#define AX_DEBUG_PNG
//#define AX_DEBUG_LOG "plugin.log"

#include "base/axBase.h"
#include "par/parFloat.h"
#include "par/parInteger.h"

//----------

#define MAX_SRATE     192000
#define MAX_SECONDS   1
#define MAX_BUFSIZE   (MAX_SECONDS*MAX_SRATE*2)

//----------------------------------------------------------------------

char* str_params[] = { (char*)"buffer", (char*)"size", (char*)"speed", (char*)"start", (char*)"freeze", (char*)"loopmode" };

//----------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"fx_freeze"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 6; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
};

//----------------------------------------------------------------------
char* str_freeze[] = { (char*)"off", (char*)"on" };
char* str_loopmode[] = { (char*)"wrap", (char*)"bidi" };

//----------

class myInstance : public AX_INSTANCE
{
  private:

    float*  BUFFER;
    int     index;
    float   pos;

    int     bufsize;
    float   size;
    float   speed;
    float   start;
    int     freeze;
    int     mode;

    float   srate;
    float   _bufsize;
    float   _size;
    float   _start;


  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        BUFFER = new float[MAX_BUFSIZE];
        index  = 0;
        pos    = 0;
        //describe("fx_freeze","ccernn","axonlib example",0,AX_MAGIC+0x1009);
        //setupAudio(2,2);
        appendParameter( new parFloat(  this,"buffersize","ms", 1000, 1, 1000 ));
        appendParameter( new parFloat(  this,"size",      "",   1,    0, 1 ));
        appendParameter( new parFloat(  this,"speed",     "",   1,    0, 2 ));
        appendParameter( new parFloat(  this,"start",     "",   0,    0, 1 ));
        appendParameter( new parInteger(this,"freeze",    "",   0,    0, 1, str_freeze ));
        appendParameter( new parInteger(this,"loop mode", "",   1,    0, 1, str_loopmode ));
        setupParameters();
      }

    virtual ~myInstance()
      {
        delete[] BUFFER;
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        //if (aParameter->getIndex()==0) m_Gain = aParameter->getValue();
        int  id = aParameter->getIndex();
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: _bufsize = f*0.001; break;
          case 1: _size    = f;       break;
          case 2:  speed   = f;       break;
          case 3: _start   = f;       break;
          case 4:  freeze  = f;       break;
          case 5:  mode    = f;       break;
        }
      }

    // bah! TOTO

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        float srate = getSampleRate();
        //trace("srate " << srate);
        bufsize = axMax(1,(_bufsize*srate));
        size    = axMax(1,(_size*bufsize));
        start   = _start * bufsize;
        while (index>=bufsize) index -= bufsize;
        //while (index<0) index += bufsize;
        return false;
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        float spl0 = *aInputs[0];
        float spl1 = *aInputs[1];

        int p2;
        if (freeze==0)
        {
          p2 = index*2;
          BUFFER[p2  ] = spl0;
          BUFFER[p2+1] = spl1;
        }
        index += 1;
        if (index>=bufsize) index = 0;
        //out = BUFFER[pos];
        pos += speed;
        if( mode==0) // wraparound
        {
          if (pos>=size) pos -= size;
          if (pos<0) pos += size;
        }
        else if (mode==1) // bidi looping
        {
          if (pos>=size)
          {
            pos = size - (pos-size);
            speed = -speed;
          }
          else if (pos<0)
          {
            pos = -pos;
            speed = -speed;
          }
        }
        p2 = start + pos;
        /*if (p>=bufsize)*/ while (p2>=bufsize) p2-=bufsize;
        /*if (p<0)*/ while (p2<0) p2+=bufsize;
        p2 *= 2;
        spl0 = BUFFER[p2];
        spl1 = BUFFER[p2+1];

        *aOutputs[0] = spl0;
        *aOutputs[1] = spl1;
      }

};

AX_MAIN(myDescriptor,myInstance)
