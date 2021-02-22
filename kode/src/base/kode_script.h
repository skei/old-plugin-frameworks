#ifndef kode_script_included
#define kode_script_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_tcc.h"

//----------------------------------------------------------------------

class KODE_Compiler {
  
//------------------------------
private:
//------------------------------
  
  KODE_Tcc* MTcc = KODE_NULL;
  
//------------------------------
public:
//------------------------------

  KODE_Compiler() {
    MTcc = KODE_New KODE_Tcc();
    MTcc->setOutputType(TCC_OUTPUT_MEMORY);
    //KODE_Memset(scripts,0,sizeof(scripts));
    //addIncludePath("./data/scripts");
    //setLibPath("");
    //addSysIncludePath("");
    //addLibraryPath("");
    //addLibrary("");
    //setOptions("");
    //defineSymbol("K64","1");
  }
  
  //----------
  
  virtual ~KODE_Compiler() {
    if (MTcc) KODE_Delete MTcc;
    //if (relocated_buffer) KODE_Free(relocated_buffer);
  }
  
public:

  virtual void addFile(const char* APath) {}
  virtual void addScript(const char* ACode) {}
  virtual void compile(void) {}
  virtual void execute() {}

};

//----------------------------------------------------------------------
#endif




#if 0

    ~KODE_Compiler() {
    }

    uint32 loadIndexedScripts(const char* path) {
      uint32 count = 0;
      addIncludePath(path);
      char num_buffer[16];
      char path_buffer[256];
      char name[64];
      DIR *d;
      dirent *dir;
      d = opendir(path);
      if (d) {
        while ((dir = readdir(d)) != NULL) {
          if (dir->d_type == DT_REG) {

            char* filename = dir->d_name;

            // if filename starts with a number, parse it
            if ( is_digit(filename[0])) {

              count += 1;
              num_buffer[0] = 0;
              int p1 = 0;
              int p2 = 0;
              int num = -1;

              while (is_digit(filename[p1])) {
                num_buffer[p1] = filename[p1];
                p1++;
              }
              num_buffer[p1] = '\0';
              num = str2int(num_buffer);
              p1++; // skip '_'
              // function name
              while (filename[p1] != '.') { // copy everything until first '.'
                name[p2++] = filename[p1++];
              }
              name[p2] = '\0';
              if (num>=0) {
                KODE_Strncpy( scripts[num].name, name, MAX_SCRIPT_NAME_SIZE );
              }
              // construct full path
              path_buffer[0] = 0;
              KODE_Strcpy(path_buffer,path);
              KODE_Strcat(path_buffer,"/");
              KODE_Strcat(path_buffer,filename);

              //printf("adding script: %s (%i %s)\n",path_buffer, num, name);

              addFile(path_buffer,TCC_FILETYPE_C) ;
            }
            //k64_dtrace("%s (%i %s)\n",filename,num,name);
          }
        }
        closedir(d);
      }
      //printf("%i scripts found\n",count);
      return count;
    }

    void initIndexedScripts(void) {
      char buffer[256];
      //k64_dtrace("*** initializing scripts:\n");
      for (int i=0; i<128; i++) {
        script_t* script = &scripts[i];
        char* name = script->name;
        if (name[0] != '\0') {
          //printf("initializing script %i '%s'\n",i,name);
          KODE_Strcpy(buffer,name);
          KODE_Strcat(buffer,"_create");
          script->create = (script_create_t)getSymbol(buffer);
          //if (script->create) printf("%s found\n",buffer);
          KODE_Strcpy(buffer,name);
          KODE_Strcat(buffer,"_destroy");
          script->destroy = (script_destroy_t)getSymbol(buffer);
          //if (script->destroy) printf("%s found\n",buffer);
          KODE_Strcpy(buffer,name);
          KODE_Strcat(buffer,"_start");
          script->start = (script_start_t)getSymbol(buffer);
          //if (script->start) printf("%s found\n",buffer);
          KODE_Strcpy(buffer,name);
          KODE_Strcat(buffer,"_stop");
          script->stop = (script_stop_t)getSymbol(buffer);
          //if (script->stop) printf("%s found\n",buffer);
          KODE_Strcpy(buffer,name);
          KODE_Strcat(buffer,"_render");
          script->render = (script_render_t)getSymbol(buffer);
          //if (script->render) printf("%s found\n",buffer);
          KODE_Strcpy(buffer,name);
          KODE_Strcat(buffer,"_control");
          script->control = (script_control_t)getSymbol(buffer);
          //if (script->control) printf("%s found\n",buffer);
          KODE_Strcpy(buffer,name);
          KODE_Strcat(buffer,"_parameter");
          script->parameter = (script_parameter_t)getSymbol(buffer);
          //if (script->parameter) printf("%s found\n",buffer);
          script->self = script->create();
        }
      }
      return;
    }

    void createIndexedScripts(void) {
      for (int i=0; i<128; i++) {
        script_t* script = &scripts[i];
        script->self = script->create();
      }
    }

    void destroyIndexedScripts(void) {
      for (int i=0; i<128; i++) {
        script_t* script = &scripts[i];
        void* self = script->self;
        if (self) script->destroy(self);
      }
    }

    void startScript(uint32 AIndex, uint32 AIntensity, float ATime) {
      script_t* script = &scripts[AIndex];
      if (script) script->start(script->self,AIntensity,ATime);
    }

    void stopScript(uint32 AIndex) {
      script_t* script = &scripts[AIndex];
      if (script) script->stop(script->self);
    }

    void renderScript(uint32 AIndex, bitmap_t* ABitmap, float ATime) {
      script_t* script = &scripts[AIndex];
      if (script) script->render(script->self,ABitmap,ATime);
    }

    void controlScript(uint32 AScriptIndex, uint32 AIndex, uint32 AValue) {
      script_t* script = &scripts[AScriptIndex];
      if (script) script->control(script->self,AIndex,AValue);
    }

    void parameterScript(uint32 AScriptIndex, uint32 AIndex, float AValue) {
      script_t* script = &scripts[AScriptIndex];
      if (script) script->parameter(script->self,AIndex,AValue);
    }

#endif // 0
