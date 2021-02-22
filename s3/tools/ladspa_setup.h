
#include <stdlib.h> // system

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//float powf(float base, float exponent) { return 0; }
//float cosf(float arg) { return 0;}
//float expf(float arg) { return 0; }
//void sincosf(float x, float *sin, float *cos) { *sin=0; *cos=0; }

// -ldl

#include <dlfcn.h>
#include <dirent.h>
#include "../src/extern/ladspa/ladspa.h"

//----------

//const char*                 ladspa_path             = "/usr/lib/ladspa/";
bool                        lib_loaded              = false;
void*                       lib_handle              = NULL;
LADSPA_Descriptor_Function  lad_descriptor_function = NULL;
const LADSPA_Descriptor*    lad_descriptor          = NULL;
LADSPA_Handle               lad_handle;

const char*                 ladspa_path             = "";
const char*                 vst_path                = "";
const char*                 wrapper_file            = "";

//----------

/*
  system()
  The value returned is -1 on error (e.g., fork(2) failed), and the return
  status of the command otherwise. This latter return status is in the format
  specified in wait(2). Thus, the exit code of the command will be
  WEXITSTATUS(status). In case /bin/sh could not be executed, the exit status
  will be that of a command that does exit(127).

  If the value of command is NULL, system() returns nonzero if the shell is
  available, and zero if not.
*/

//----------

void create_wrapper(const char* filename, int index, const char* name, const char* label, int num_ports) {
  int result = 0;
  //char buffer[256];
  //char int2str[256];
  char temp[256];
  printf("creating wrapper: filename '%s' index %i -> '%s.so'\n",filename,index,label);
  //printf("    filename '%s' index %i name '%s' label '%s' ports %i'\n",filename,index,name,label,num_ports);
  //printf("    -> '%s.so'\n",label);

  //buffer[0] = '\0';
  //strcpy(buffer,"cp ladspa_wrapper.bin ");
  //strcat(buffer,label);
  //strcat(buffer,".so");
  //result = system(buffer);
  //if (result == -1) {
  //  printf("error copying ladspa_wrapper.bin to %s.so\n",label);
  //  exit(-1);
  //}

  //sprintf(temp,"cp ladspa_wrapper.bin %s%s.so",vst_path,label);
  sprintf(temp,"cp %s %s%s.so",wrapper_file,vst_path,label);
  result = system(temp);
  if (result == -1) { printf("error creating %s%s.so\n",vst_path,label); exit(-2); }

  //buffer[0] = '\0';
  //strcpy(buffer,"echo filename \"");
  //strcat(buffer,filename);
  //strcat(buffer,"\" > ");
  //strcat(buffer,label);
  //strcat(buffer,".cfg");
  //result = system(buffer);
  //if (result == -1) {
  //  printf("error creating %s.cfg\n",label);
  //  exit(-2);
  //}
  //buffer[0] = '\0';
  //sprintf(int2str,"%i",index);
  //strcat(buffer,"echo index ");
  //strcat(buffer,int2str);
  //strcat(buffer," >> ");
  //strcat(buffer,label);
  //strcat(buffer,".cfg");
  //result = system(buffer);
  //if (result == -1) {
  //  printf("error creating %s.cfg\n",label);
  //  exit(-2);
  //}

  sprintf(temp,"echo filename '\"%s\"' > %s%s.cfg",filename,vst_path,label);
  result = system(temp);
  if (result == -1) { printf("error while creating %s%s.cfg\n",vst_path,label); exit(-2); }
  sprintf(temp,"echo index %i >> %s%s.cfg",index,vst_path,label);
  result = system(temp);
  if (result == -1) { printf("error while creating %s%s.cfg\n",vst_path,label); exit(-2); }
}

//----------

int load_ladspa_plugin(const char* filename) {
  char buffer[256];
  strcpy(buffer,ladspa_path);
  strcat(buffer,filename);
//  printf("loading ladspa plugin '%s'\n",buffer);
  //lib_handle = dlopen(buffer,RTLD_NOW);
  //lib_handle = dlopen(buffer,RTLD_NOW | RTLD_LOCAL);
  lib_handle = dlopen(buffer,RTLD_LAZY | RTLD_LOCAL);
  if (!lib_handle) {
    printf("error: couldn't open '%s'\n",buffer);
    printf("(%s)\n",dlerror());
    return -1;
  }
  lib_loaded = true;
  lad_descriptor_function = (LADSPA_Descriptor_Function)dlsym(lib_handle,(char*)"ladspa_descriptor");
  if (!lad_descriptor_function) {
    printf("error: 'ladspa_descriptor' function not found\n");
    return -2;
  }
  int num_plugins = 0;
  const LADSPA_Descriptor* descr = lad_descriptor_function(num_plugins);
  while (descr) {
    int index = num_plugins;
    const char* name = descr->Name;
    const char* label = descr->Label;
    int num_ports = descr->PortCount;
    create_wrapper(buffer,index,name,label,num_ports);
    num_plugins++;
    descr = lad_descriptor_function(num_plugins);
  }
  return 0;
}

//----------

int main (int argc, char **argv) {

	if (argc!=4)
	{
		fprintf(stderr,"usage: ladspa_setup ladspa_directory vst_directory wrapper_binary\n");
		return 1;
	}

  ladspa_path = argv[1];
  vst_path = argv[2];
  wrapper_file = argv[3];

  //dlopen(NULL,RTLD_NOW|RTLD_GLOBAL);

  //FILE  *in_file, *out_file;
  DIR   *in_dir;
  struct dirent *ent;
  if ((in_dir = opendir(ladspa_path)) != NULL) {
    while ((ent = readdir(in_dir)) != NULL) {
      if (ent->d_name[0] != '.') { load_ladspa_plugin(ent->d_name); }
    }
    closedir(in_dir);
  } else {
    printf("error opening directory");
    return -1; //EXIT_FAILURE;
  }
  return 0;
}
