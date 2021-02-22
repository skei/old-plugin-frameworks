#ifndef s3_program_included
#define s3_program_included
//----------------------------------------------------------------------

// https://www.kvraudio.com/forum/viewtopic.php?p=755695#p755695



#include "common/s3_array.h"
class S3_Program;
typedef S3_Array<S3_Program*> S3_ProgramArray;

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Program {
  private:
    const char*   MName;
    S3_FloatArray MParamValues;
  public:
    S3_Program();
    S3_Program(const char* AName);
    virtual ~S3_Program();
  public:
    const char* getName(void) { return MName; }
    void        setName(const char* ANAme) {}
    void        appendValue(float AValue) { MParamValues.append(AValue); }
    float       getValue(uint32 AIndex) { return MParamValues[AIndex]; }
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Program::S3_Program() {
  MName = "default";
}

//----------

S3_Program::S3_Program(const char* AName) {
  MName = AName;
}

//----------

S3_Program::~S3_Program() {
}

//----------------------------------------------------------------------
#endif

/*
  https://www.kvraudio.com/forum/viewtopic.php?p=755695#p755695

  here are the example snippets from our VeloReMap VSTi, which also uses a
  graphical editor for drawing a curve with the mouse:
  Assuming, you have defined your "chunk" data as a complete program like this:
  Note: The identifiers and version thingys are optional, not required because
  we also use it for exporting/importing patches in our own format or version
  checking...

    class PluginProgram {
      friend class Plugin;
      public:
        PluginProgram();
      private:
        char id[4];
        char version[4];
        char name[24];
        float params[kNumParams];
        float table[128];
      };
      PluginProgram::PluginProgram() {
        id[0] = 'J';
        id[1] = 'H';
        id[2] = 'V';
        id[3] = 'R';
        version[0] = 1;
        version[1] = 0;
        version[2] = 0;
        version[3] = 0;
        // default program values
        ZeroMemory(name, sizeof(name));
        strcpy(name, "default curve");
        params[kAmount] = 1.f;
        /...
        for (int i = 0; i < 128; i++)
          table[i] = (1.f / 128) * i;
      }

  Then you can implement the chunk routines very comfortable this way:
  Please note, that we define the programs as a menber of the plugin class as follows:

    class Plugin : public AudioEffectX {
      public:
        Plugin(audioMasterCallback audioMaster);
        ~Plugin();
        //...
      private:
        //...
        PluginProgram programs[kNumPrograms];
        //...
    }

    long Plugin::getChunk(void** data, bool isPreset) {
      if (!data) return 0;
      if (isPreset) {
        *data = &programs[curProgram];
        return sizeof (PluginProgram);
      }
      else {
        *data = &programs;
        return sizeof(programs);
      }
    }

    long Plugin::setChunk(void* data, long byteSize, bool isPreset) {
      if (!data) return 0;
      if (isPreset) {
        if (byteSize != sizeof(PluginProgram))
          return 0;
        PluginProgram *preset = (PluginProgram*)data;
        if ((preset->id[0] == 'J') &&
            (preset->id[1] == 'H') &&
            (preset->id[2] == 'V') &&
            (preset->id[3] == 'R') &&
            (preset->version[0] == 1)) {
          memcpy(&programs[curProgram], preset, sizeof(PluginProgram));
          setProgram(curProgram);
        }
        else return 0;
        return 1;
      }
      else {
        if (byteSize == sizeof(programs)) {
          PluginProgram *myBank = (PluginProgram*)data;
          if ((myBank->id[0] == 'J') &&
              (myBank->id[1] == 'H') &&
              (myBank->id[2] == 'V') &&
              (myBank->id[3] == 'R') &&
              (myBank->version[0] == 1)) {
            memcpy((void*)&programs, myBank, sizeof(programs));
            setProgram(curProgram);
          }
          else return 0;
        }
        else return 0;
        return 1;
      }
    }

  Note, that in this example we work directly with the program's data inside
  the plugin, so that at each time the data is ACTUAL. This means, the GUI has
  direct access to the current program's data (the program chunk) for
  simplicity...
*/
