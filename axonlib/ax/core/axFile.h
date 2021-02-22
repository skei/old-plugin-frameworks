/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axFile_included
#define axFile_included
//----------------------------------------------------------------------

// axFile class
// #### untested !

// possible #include issues here...

#include "axDefines.h"
#include "axStdlib.h" // ...
#include "axDebug.h"  // trace()
#include "axMalloc.h" // axMalloc
#include "axBasePath.h"

//#include "axStdlib.h"
#include <stdio.h>

// ccernn:
// this created a curcular-inclusion (if AX_DEBUG_MEM or something defined)
// axStdlib includes axMalloc, and axMalloc again, includes axDebug..
// a simple solution (for this single case) is to have this internal
// function inline:

/*
inline char* __axStrrchr(const char* s, int c)
{
  char* p = NULL;
  while (*s++) if (*s == c) p = (char*) s;
  return p;
}
*/

//----------

class axFile
{
  private:
    FILE* f;             // used by all

  public:
    unsigned int size;  // size of a buffer is stored here

    axFile()  { f = NULL; }
    ~axFile() {}

    // close
    void close(void)
    {
      if (f)
        fclose(f);
    }

    // file read
    unsigned char* read (const char* _file,
      const unsigned int mode = 0)
    {
      char filepath[AX_MAX_PATH] = "";
      char _path[AX_MAX_PATH] = "";
      const char* path = axGetBasePath(_path);
      axStrcat(filepath, (char*)path);
      axStrcat(filepath, (char*)_file);
      trace("axFileRead(" << mode << "): " << filepath);
      switch (mode)
      {
        case 0: f = fopen(filepath, "rb"); break;
        case 1: f = fopen(filepath, "r");  break;
      }
      if (!f)
      {
        trace("axFileRead, #ERR open(" << mode << "): " << filepath);
        return 0;
      }
      fseek(f, 0, SEEK_END);
      size = ftell(f);
      fseek(f, 0, SEEK_SET);
      if (!size)
      {
        trace("axFileRead, #ERR null sized: " << filepath);
        return 0;
      }
      unsigned char* b;// = (unsigned char*)axMalloc(size);
      unsigned int res = fread(b, size, 1, f);
      close();
      if (!res)
      {
        trace("axFileRead, #ERR read: " << filepath);
        return 0;
      }
      return b;
    }

    // write buffer 'b' with 'size' into 'file' using 'mode'
    unsigned int write (const char* _file,
      const char* b, const unsigned int mode = 0 )
    {
      char filepath[AX_MAX_PATH] = "";
      char _path[AX_MAX_PATH] = "";
      const char* path = axGetBasePath(_path);
      axStrcat(filepath, (char*)path);
      axStrcat(filepath, (char*)_file);
      trace("axFileWrite(" << mode << "): " << filepath);
      switch (mode)
      {
        case 0: f = fopen(filepath, "wb"); break;
        case 1: f = fopen(filepath, "w");  break;
        case 2: f = fopen(filepath, "ab"); break;
        case 3: f = fopen(filepath, "a");  break;
      }
      if (!f)
      {
        trace("axFileWrite, #ERR open(" << mode << "): " << filepath);
        return 0;
      }
      if (!size)
      {
        trace("axFileWrite, #ERR write 0 bytes: " << filepath);
        return 0;
      }
      unsigned int res = fwrite(b, sizeof(b[0]), size, f);
      close();
      if (!res)
      {
        trace("axFileWrite, #ERR write: " << filepath);
        return 0;
      }
      return 1;
    }
};


// ------------------------------------------------
// test
#if 0

// test_file_methods
void test_file_methods(void)
{
  // write bin file
  axFile tFile;
  const char* buffer1 = "hello";
  tFile.size = 5; // set size before write
  tFile.write("./myfile.32", buffer, 0);

  // read the same file
  char* buffer2 = tFile.read("./myfile.32", 0);
  trace("size of file is:" << tFile.size);

  // buffer2  has to be free-ed
  axFree(buffer2);

  // new instance...
  axFile tFile2;
  // ...
}

#endif // 0

#endif // axFile_included
