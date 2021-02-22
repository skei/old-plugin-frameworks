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

#ifndef axBitmapLoad_included
#define axBitmapLoad_included
//----------------------------------------------------------------------
/*
  TODO:

  rework this into a more generic decoder class.
  and specific decoders being sub-clases of this

*/

//#include "axBitmap.h"
//#include "axSurface.h"

#include "core/axStdlib.h"
#include "core/axFile.h"
#include "../extern/picopng.h"

//----------------------------------------------------------------------

class axBitmapLoader
{
  private:
    unsigned long   mWidth;
    unsigned long   mHeight;
    unsigned char*  mImage;

  public:

    axBitmapLoader()
      {
        mWidth    = 0;
        mHeight   = 0;
        mImage    = NULL;
      }

    //----------

    virtual ~axBitmapLoader()
      {
        if (mImage) axFree(mImage);
      }

    //----------

    inline int            getWidth(void)  { return mWidth; }
    inline int            getHeight(void) { return mHeight; }
    inline unsigned char* getImage(void)  { return mImage; } //&mImage[0]; }

    //--------------------------------------------------

    void* decodePng(unsigned char* buffer, unsigned int buffersize)
      {
        //trace("decodePng(), " << (void*)&buffer << ", " << buffersize);
        void* res = 0;
        //if (mPngInfo) axFree(mPngInfo);
        axPngInfo* png = axPngDecode(buffer, buffersize);
        //trace("decoded. png =" << (int)png);
        //trace("png error = " << PNG_error);
        res = (void*)png;
        mWidth  = png->width;
        mHeight = png->height;
        //mImage  = mPngInfo->image->data;
        // allocate our own chunk of data, and free all temporary png decoder things
        //trace("image size:  " << png->image->size ); // 256k
        //trace("image alloc: " << png->image->allocsize );  // 2mb
        mImage = (unsigned char*)axMalloc(png->image->size);
        axMemcpy(mImage,png->image->data,png->image->size);
        axPngFreeAll();                                                 // CHECK THIS !!!
        //axFree(png); // crash (already free?)
        //png->image->data[0] = 0;  // try to access image data
        return res; //(int)mPngInfo;
      }

    //--------------------------------------------------

   int decodeLoadPng(const char* file)
      {
        unsigned int size;
        axFile f;
        unsigned char* b = /*axFileRead*/f.read(file, size/*, 0*/);

        // check if the buffer contains a png (or a least partially)
        if (b[0] != 0x89 || b[1] != 0x50 || b[2] != 0x4E || b[3] != 0x47)
        {
          //trace("decodeLoadPng(), #ERR not a png: " << file);
          return 0;
        }
        else
        {
          //trace("decodeLoadPng(), " << file << ", " << size);
          // leave pico to clear the file buffer
          decodePng(b, size);
          return 1;
        }
      }

};

//----------------------------------------------------------------------
#endif

