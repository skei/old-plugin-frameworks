#ifndef kode_bitmap_included
#define kode_bitmap_included
//----------------------------------------------------------------------

//#define KODE_BITMAP_BRESENHAM
//#define KODE_BITMAP_WU
//#define KODE_BITMAP_FONT
//#define KODE_BITMAP_VECTOR

//----------

#include "base/kode.h"
#include "base/kode_math.h"
#include "base/kode_math_int.h"
#include "base/kode_rgba.h"
#include "base/kode_rect.h"
#include "gui/kode_drawable.h"

//----------

// stb_image.h

#ifndef KODE_BITMAP_NO_DECODE
  #ifndef KODE_BITMAP_NO_FILE
    #define STB_IMAGE_IMPLEMENTATION
    #include "extern/stb/stb_image.h"
  #endif
#endif

// cairo

#ifdef KODE_CAIRO
  #include "gui/cairo/kode_cairo.h"
#endif

//----------------------------------------------------------------------

class KODE_Bitmap
: public KODE_Drawable {

protected:

  uint32_t  MWidth        = 0;
  uint32_t  MHeight       = 0;
  uint32_t  MDepth        = 32;
  uint32_t  MStride       = 0;
  uint32_t  MBufferSize   = 0;  // width * height * sizeof(uint32_t) // 0 = sub-bitmap (not allocated)
  uint8_t*  MBuffer       = nullptr;
  bool      MAllocated    = false;
  
  //uint32_t  MPalette[256] = {0};

  //#define KODE_BITMAP_RASTERIZER_MAX_HEIGHT 2048
  //#ifdef KODE_BITMAP_RASTERIZER
  //  int32_t   MScanLeftEdge[KODE_BITMAP_RASTERIZER_MAX_HEIGHT];  // 24.8 fp
  //  int32_t   MScanRightEdge[KODE_BITMAP_RASTERIZER_MAX_HEIGHT]; // 24.8 fp
  //  int32_t   MScanMinY =  KODE_INT32_MAX;;
  //  int32_t   MScanMaxY = -KODE_INT32_MAX;
  //#endif
  
  //#ifdef KODE_CAIRO
  //  cairo_surface_t* MCairoSurface = nullptr;
  //#endif

//------------------------------
public:
//------------------------------

  // todo: KODE_New KODE_Bitmap(..) = allocate new (potentially copy)
  //       getSubBitmap(x,y...) = sub..

  /**
    Creates a null bitmap (using default values)
  */

  KODE_Bitmap() {
  }

  // create new/empty

  KODE_Bitmap(uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32) {
    MWidth      = AWidth;
    MHeight     = AHeight;
    MDepth      = ADepth; // 32;
    MStride     = AWidth * 4;
    MBufferSize = MStride * MHeight;
    MBuffer     = (uint8_t*)KODE_Malloc(MBufferSize);
    MAllocated  = true;
    //#ifdef KODE_CAIRO
    //  MCairoSurface = createCairoSurface();
    //#endif
  }

  //----------

  /**
    Clones/copies another bitmap..
    new memory is allocated, and bitmap is copied into it.
  */

  KODE_Bitmap(KODE_Bitmap* ABitmap) {
    MWidth      = ABitmap->MWidth;
    MHeight     = ABitmap->MHeight;
    MDepth      = ABitmap->MDepth; // 32;
    MStride     = ABitmap->MStride;
    MBufferSize = ABitmap->MBufferSize;
    //if (ABitmap->MAllocated) {
      MBuffer     = (uint8_t*)KODE_Malloc(MBufferSize);
      KODE_Memcpy(MBuffer,ABitmap->MBuffer,MBufferSize);
      MAllocated  = true;
    //}
    //else {
    //}
    //#ifdef KODE_GUI_CAIRO
    //  MCairoSurface = createCairoSurface();
    //#endif
  }

  //----------

  /**
    Clones/copies a part of another bitmap..
    new memory is allocated, and bitmap is copied into it.
  */

  KODE_Bitmap(KODE_Bitmap* ABitmap, KODE_Rect ARect) {
    MWidth      = ARect.w;
    MHeight     = ARect.h;
    MDepth      = ABitmap->MDepth; // 32;
    MStride     = MWidth * 4;
    MBufferSize = MStride * MHeight;
    MBuffer     = (uint8_t*)KODE_Malloc(MBufferSize);
    MAllocated  = true;

    uint32_t* dst = (uint32_t*)MBuffer;
    for (uint32_t y=0; y<MHeight; y++) {
      uint32_t* src = (uint32_t*)ABitmap->getLinePtr(ARect.y + y);
      KODE_Memcpy(dst,src,MWidth*4);
      dst += MWidth;
    }

    //#ifdef KODE_GUI_CAIRO
    //  MCairoSurface = createCairoSurface();
    //#endif
  }

  //----------

  /**
    Decodes an image from memory (using stb_image)
  */

  #ifndef KODE_BITMAP_NO_DECODE

  KODE_Bitmap(const uint8_t* buffer, uint32_t length) {
    int x,y,n;
    unsigned char* data = stbi_load_from_memory(buffer,length,&x,&y,&n,4 /*0*/ );
    MWidth  = x;
    MHeight = y;
    // returned n is bytes per pixel in image
    // but we'force' 32 bit
    /*
    switch(n) {
      case 0: MDepth = 0;  break;
      case 1: MDepth = 8;  break;
      case 2: MDepth = 16; break;
      case 3: MDepth = 24; break;
      case 4: MDepth = 32; break;
    }
    */
    MStride     = MWidth  * 4; //n;
    MBufferSize = MStride * MHeight;
    MBuffer     = (uint8_t*)KODE_Malloc(MBufferSize);
    MAllocated  = true;
    KODE_Memcpy(MBuffer,data,MBufferSize);
    convertRgbaBgra();
    stbi_image_free(data);
  }

  #endif // KODE_NO_BITMAP_DECODE

  //----------

  /**
    Loads and decodes an image from a file (using stb_image)
  */

  #ifndef KODE_BITMAP_NO_DECODE
  #ifndef KODE_BITMAP_NO_FILE

  KODE_Bitmap(const char* AFilename) {
    int x,y,n;
    unsigned char *data = stbi_load(AFilename, &x, &y, &n, 4 /*0*/); // 4 0 we want 32-bit
    MWidth  = x;
    MHeight = y;
    // returned n is bytes per pixel in image
    // but we'force' 32 bit
    /*
    switch(n) {
      case 0: MDepth = 0;  break;
      case 1: MDepth = 8;  break;
      case 2: MDepth = 16; break;
      case 3: MDepth = 24; break;
      case 4: MDepth = 32; break;
    }
    */
    MStride     = MWidth  * 4; //n;
    MBufferSize = MStride * MHeight;
    MBuffer     = (uint8_t*)KODE_Malloc(MBufferSize);
    MAllocated  = true;
    KODE_Memcpy(MBuffer,data,MBufferSize);
    convertRgbaBgra();
    stbi_image_free(data);
  }

  #endif // KODE_NO_BITMAP_FILE
  #endif // KODE_NO_BITMAP_DECODE

  //----------

  /**
    Destroys the bitmap..
    frees allocated memory
  */

  virtual ~KODE_Bitmap() {
    //#ifdef KODE_CAIRO
    //  //cairo_surface_finish(MCairoSurface);
    //  if (MCairoSurface) cairo_surface_destroy(MCairoSurface);
    //#endif
    if (MAllocated && MBuffer) KODE_Free(MBuffer);
  }

//------------------------------
public: // sub-bitmap
//------------------------------

  /**
    Creates a sub-bitmap that shares the same memory, but have a different
    'view' into it.. different size and offset.. for things like clipping,
    processing specific parts, etc..
  */

  KODE_Bitmap* createSubBitmap(KODE_Rect ARect) {
    KODE_Bitmap* bitmap = new KODE_Bitmap();
    bitmap->MWidth      = ARect.w;
    bitmap->MHeight     = ARect.h;
    bitmap->MDepth      = MDepth;
    bitmap->MStride     = MStride;
    bitmap->MBufferSize = 0;
    bitmap->MBuffer     = getPixelPtr(ARect.x,ARect.y);
    bitmap->MAllocated  = false;
    return bitmap;
  }
  
  //----------

  /**
    Makes a sub-bitmap by filling in the member variables of the argument
    bitmap directly.. for speed, to avoid repeatedly allocating/freeing
    KODE_Bitmap classes if you need to repeatedly process parts of a bitmap.
    you can have a pool of sub-bitmaps, and call getSubBitmap to set them up..
  */

  KODE_Bitmap* getSubBitmap(KODE_Bitmap* ABitmap, KODE_Rect ARect) {
    ABitmap->MWidth      = ARect.w;
    ABitmap->MHeight     = ARect.h;
    ABitmap->MDepth      = MDepth;
    ABitmap->MStride     = MStride;
    ABitmap->MBufferSize = 0;
    ABitmap->MBuffer     = getPixelPtr(ARect.x,ARect.y);
    ABitmap->MAllocated  = false;
    return ABitmap;
  }
  
  //----------
  
  /*
    caller deallocates buffer memory
  */
  
  void setup(uint32_t AWidth, uint32_t AHeight, uint32_t ADepth, uint32_t AStride, uint8_t* ABuffer) {
    MWidth      = AWidth;
    MHeight     = AHeight;
    MDepth      = ADepth;
    MStride     = AStride;
    MBuffer     = ABuffer;
    MBufferSize = 0;
    MAllocated  = false;
  }
  

//------------------------------
public: // drawable
//------------------------------

  bool      isBitmap()  override { return true; }
  uint32_t  getWidth()  override { return MWidth; }
  uint32_t  getHeight() override { return MHeight; }
  uint32_t  getDepth()  override { return MDepth; }

  //#ifdef KODE_CAIRO
  //  bool              isCairo()         override { return true; }
  //  cairo_surface_t*  getCairoSurface() override { return MCairoSurface; }
  //#endif

//------------------------------
public:
//------------------------------

  int32_t   getStride()     { return MStride; }
  uint8_t*  getBuffer()     { return MBuffer; }
  int32_t   getBufferSize() { return MBufferSize; }
  
  //----------

  /**
    create cairo surface from bitmap
  */

  #ifdef KODE_CAIRO
    cairo_surface_t* createCairoSurface(void) {
      cairo_surface_t* surface = cairo_image_surface_create_for_data(
        (unsigned char*)MBuffer,  // unsigned char *data,
        CAIRO_FORMAT_ARGB32,      // cairo_format_t format,
        MWidth,                   // int width,
        MHeight,                  // int height,
        MStride                   // int stride);
      );
      //KODE_Trace("%s\n",cairo_status_to_string(cairo_surface_status(surface)));
      return surface;
    }
  #endif

//------------------------------
public: // ptr
//------------------------------

  /**
    get pointer to specified line
  */

  uint8_t* getLinePtr(uint32_t AYpos) {
    uint8_t* ptr = MBuffer + (MStride * AYpos);
    return ptr;
  }

  //----------

  /**
    get pointer to specified, plus next line
    (mainly for bilinear interpolation, etc?)
  */
  
  // do we need this? caller can call bitmap->getStride(),
  // potentially cache it, and do the multiplication itself..
  

  uint8_t* getLinePtr2(uint32_t AYpos, uint8_t** ptr2) {
    uint8_t* ptr = MBuffer + (MStride * AYpos);
    *ptr2 = ptr + MStride;
    return ptr;
  }

  //----------

  /**
    get pointer to specified pixel
  */

  uint8_t* getPixelPtr(uint32_t AXpos, uint32_t AYpos) {
    uint8_t* ptr = MBuffer + (MStride * AYpos) + (AXpos * 4);
    return ptr;
  }

//------------------------------
// pixel
//------------------------------

public:

  /**
    get specified pixel
  */

  uint32_t getPixel(uint32_t x, uint32_t y) {
    if (x >= (uint32_t)MWidth) return 0;  // 0xff000000;
    if (y >= (uint32_t)MHeight) return 0; // 0xff000000;
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    return *ptr;
  }

  uint32_t getPixelNoclip(uint32_t x, uint32_t y) {
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    return *ptr;
  }

  //----------

  /**
    get specified pixel
    wrap coordinates to fit width/height
  */

  uint32_t getPixelWrap(uint32_t x, uint32_t y) {
    //if (x >= (uint32_t)MWidth) return 0;//x = MWidth - 1;
    //if (y >= (uint32_t)MHeight) return 0;//y = MHeight - 1;
    x %= MWidth;
    y %= MHeight;
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    return *ptr;
  }

  //----------

  /**
    get bilinearly filtered pixel
    coordinates are 24.8 fixed point
  */

  uint32_t getPixelBilinear(uint32_t x, uint32_t y) {
    uint32_t fx = x & 0xff;
    uint32_t fy = y & 0xff;
    uint32_t ix = x >> 8;
    uint32_t iy = y >> 8;
    if (ix >= MWidth) return 0;
    if (iy >= MHeight) return 0;
    uint32_t* ptr  = (uint32_t*)getPixelPtr(ix,iy);
    // don't interpolate if we're on right/bottom edge..
    if ((ix==(MWidth-1)) || (iy==(MHeight-1))) { return *ptr; }
    uint32_t* ptr2 = ptr + (MStride >> 2); // >> 2 because we cast to uint32_t* (stride is in bytes)
    uint32_t c1 = *ptr++;
    uint32_t c2 = *ptr;
    uint32_t c3 = *ptr2++;
    uint32_t c4 = *ptr2;
    return KODE_RGBA_BilinearAlphaBlend(c1,c2,c3,c4,fx,fy);
  }

  //----------
  
  /**
    set pixel
  */

  void setPixel(uint32_t x, uint32_t y, uint32_t c) {
    if (x>=(uint32_t)MWidth) return;
    if (y>=(uint32_t)MHeight) return;
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    *ptr = c;
  }

  void setPixelNoclip(uint32_t x, uint32_t y, uint32_t c) {
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    *ptr = c;
  }

  //----------
  
  /**
    set pixel /w/alpha
  */

  //void setPixel(uint32_t x, uint32_t y, uint32_t c, uint8_t a) {
  //  if (x>=(uint32_t)MWidth) return;
  //  if (y>=(uint32_t)MHeight) return;
  //  uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
  //  *ptr = c;
  //}

  //----------
  
  /**
    get value in specified channel (rgba)
  */

  uint8_t getValue(uint32_t x, uint32_t y, uint32_t ch) {
    if (x>=(uint32_t)MWidth) return 0;
    if (y>=(uint32_t)MHeight) return 0;
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    return ptr[ch];
  }

  uint8_t getValueNoclip(uint32_t x, uint32_t y, uint32_t ch) {
    if (x>=(uint32_t)MWidth) return 0;
    if (y>=(uint32_t)MHeight) return 0;
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    return ptr[ch];
  }

  //----------

  /**
    set value in specified channel (rgba)
  */

  void setValue(uint32_t x, uint32_t y, uint8_t v, uint32_t ch) {
    if (x>=(uint32_t)MWidth) return;//x = MWidth - 1;
    if (y>=(uint32_t)MHeight) return;//y = MHeight - 1;
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    ptr[ch] = v;
  }

  void setValueNoclip(uint32_t x, uint32_t y, uint8_t v, uint32_t ch) {
    uint32_t* ptr = (uint32_t*)getPixelPtr(x,y);
    ptr[ch] = v;
  }

  //----------

  /**
    blend pixel with existing pixel
  */
  
  //uint8_t KODE_RGBA_Alpha(uint32_t c, uint32_t a) {
  //  uint32_t ret = (c*a) >> 8;
  //  return ret & 0xff;
  //}
  
  #define _alpha(c,a) (((c * a) >> 8) & 0xff)

  void blendPixel(uint32_t x, uint32_t y, uint32_t c, uint8_t AAlpha) {
    if (x>=(uint32_t)MWidth) return;//x = MWidth-1;
    if (y>=(uint32_t)MHeight) return;//y = MHeight-1;
    //int32_t pos = (y*MWidth + x) * 4; // todo, stride
    uint8_t* ptr = (uint8_t*)getPixelPtr(x,y);
    uint8_t bb = ptr[0];
    uint8_t bg = ptr[1];
    uint8_t br = ptr[2];
    uint8_t cr = (c >> 16) & 0xff;
    uint8_t cg = (c >> 8 ) & 0xff;
    uint8_t cb = (c      ) & 0xff;
    //*ptr++ = KODE_RGBA_Alpha(cb,AAlpha) + KODE_RGBA_Alpha(bb,(255-AAlpha));
    //*ptr++ = KODE_RGBA_Alpha(cg,AAlpha) + KODE_RGBA_Alpha(bg,(255-AAlpha));
    //*ptr++ = KODE_RGBA_Alpha(cr,AAlpha) + KODE_RGBA_Alpha(br,(255-AAlpha));
    *ptr++ = _alpha(cb,AAlpha) + _alpha(bb,(255-AAlpha));
    *ptr++ = _alpha(cg,AAlpha) + _alpha(bg,(255-AAlpha));
    *ptr++ = _alpha(cr,AAlpha) + _alpha(br,(255-AAlpha));
  }
  
  #undef _alpha
  
  void blendPixelF(uint32_t x, uint32_t y, uint32_t c, float AAlpha) {
    uint8_t a = AAlpha * 255.0f;
    blendPixel(x,y,c,a);
  }
  

//------------------------------
// channel
//------------------------------

public:

  /**
    fill background, using alpha
  */

  void fillBackground(uint32_t AColor) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint32_t* ptr = (uint32_t*)getLinePtr(y);
      //ptr += AChannel;
      for (uint32_t x=0; x<MWidth; x++) {
        uint32_t c = *ptr;
        *ptr++ = KODE_RGBA_AlphaBlend(AColor,c);
        //ptr += 4;
      }
    }
  }

  //----------
  
  /**
    fill entire channel (rgba) with specified value
  */

  void fillChannel(uint32_t AChannel, uint8_t AValue) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      ptr += AChannel;
      for (uint32_t x=0; x<MWidth; x++) {
        *ptr = AValue;
        ptr += 4;
      }
    }
  }

  //----------
  
  /**
    copy one channel (rgba) to another
  */

  void copyChannel(int32_t dst, int32_t src) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      for (uint32_t x=0; x<MWidth; x++) {
        uint8_t v = ptr[src];
        ptr[dst] = v;
        ptr += 4;
      }
    }
  }

  //----------

  /**
    scale values in one channel (rgba)
    alpha is 0..255
  */

  void scaleChannel(uint32_t AChannel, uint8_t AScale) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      ptr += AChannel;
      for (uint32_t x=0; x<MWidth; x++) {
        uint8_t v = *ptr;
        v = (v*AScale) >> 8;
        *ptr = v;
        ptr += 4;
      }
    }

  }

  //----------

  /**
    scale values in one channel (rgba)
    (float version, 0..1)
  */

  void scaleChannelF(uint32_t AChannel, float AScale) {
    scaleChannel(AChannel,AScale*255);
  }

  //----------
  
  /**
    invert values in one channel (rgba)
  */

  void invertChannel(uint32_t AChannel) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = (uint8_t*)getLinePtr(y);
      ptr += AChannel;
      for (uint32_t x=0; x<MWidth; x++) {
        uint8_t v = *ptr;
        *ptr = 255 - v;
        ptr += 4;
      }
    }
  }

//------------------------------
// buffer
//------------------------------

public:

  /**
    fill entire buffer with value
  */

  void fill(uint32_t AValue) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint32_t* ptr = (uint32_t*)getLinePtr(y);
      for (uint32_t x=0; x<MWidth; x++) *ptr++ = AValue;
    }
  }

  //----------

  /**
    invert values in entire buffer
  */

  void invert(void) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = getLinePtr(y);
      for (uint32_t x=0; x<MWidth*4; x++) {
        uint8_t v = *ptr;
        *ptr++ = 255 - v;
      }
    }
  }
  
  /**
    scale values in entire buffer
  */

  void scale(uint8_t AScale) {
    for (uint32_t y=0; y<MHeight; y++) {
      uint8_t* ptr = getLinePtr(y);
      for (uint32_t x=0; x<MWidth*4; x++) {
        uint8_t v = *ptr;
        *ptr++ = v * AScale;
      }
    }
  }
  
  /**
    convert buffer from rgba to bgra
  */

  void convertRgbaBgra(void) {
    if (MBuffer) {
      for (uint32_t y=0; y<MHeight; y++) {
        uint8_t* ptr = getLinePtr(y);
        for (uint32_t x=0; x<MWidth; x++) {
          uint8_t r = ptr[0];
          uint8_t g = ptr[1];
          uint8_t b = ptr[2];
          uint8_t a = ptr[3];
          *ptr++ = b;
          *ptr++ = g;
          *ptr++ = r;
          *ptr++ = a;
        } //for x
      } //for y
    } //mBuffer
  }

  //----------

  /**
    multiplies rgb values with a
  */
  
  //uint8_t KODE_RGBA_Alpha(uint32_t c, uint32_t a) {
  //  uint32_t ret = (c*a) >> 8;
  //  return ret & 0xff;
  //}
  
  #define _alpha(c,a) (((c * a) >> 8) & 0xff)

  void premultAlpha(void) {
    if (MBuffer) {
      for(uint32_t y=0; y<MHeight; y++) {
        uint8_t* ptr = getLinePtr(y);
        for(uint32_t x=0; x<MWidth; x++) {
          uint8_t b = ptr[0];
          uint8_t g = ptr[0];
          uint8_t r = ptr[2];
          uint8_t a = ptr[3];
          *ptr++ = _alpha(b,a);
          *ptr++ = _alpha(g,a);
          *ptr++ = _alpha(r,a);
          *ptr++ = a;
        } //for x
      } //for y
    } //mBuffer
  }
  
  #undef _alpha

  //----------
  
  /**
    matrix for color manipulation
    r = radd + r*rr + g*rg + b*rb + a*ra
    same for g, b, and a
    arguments from 0 to 1
  */
  
  //uint8_t KODE_RGBA_ScaleElement(uint8_t c, float n ) {
  //  float nc = n * (float)c;
  //  return min((uint8_t)255,(uint8_t)nc);
  //}

  #define _scale(c,n) KODE_MinI((uint8_t)255,uint8_t(n*(float)c))

  void colorMatrix( float radd, float rr, float rg, float rb, float ra,
                    float gadd, float gr, float gg, float gb, float ga,
                    float badd, float br, float bg, float bb, float ba,
                    float aadd, float ar, float ag, float ab, float aa) {
    if (MBuffer) {
      for (uint32_t y=0; y<MHeight; y++) {
        uint8_t* ptr = getLinePtr(y);
        for (uint32_t x=0; x<MWidth; x++) {
          uint8_t b = ptr[0];
          uint8_t g = ptr[1];
          uint8_t r = ptr[2];
          uint8_t a = ptr[3];
          //*ptr++ = (badd * 255) + KODE_RGBA_ScaleElement(r,br) + KODE_RGBA_ScaleElement(g,bg) + KODE_RGBA_ScaleElement(b,bb) + KODE_RGBA_ScaleElement(a,ba);
          //*ptr++ = (gadd * 255) + KODE_RGBA_ScaleElement(r,gr) + KODE_RGBA_ScaleElement(g,gg) + KODE_RGBA_ScaleElement(b,gb) + KODE_RGBA_ScaleElement(a,ga);
          //*ptr++ = (radd * 255) + KODE_RGBA_ScaleElement(r,rr) + KODE_RGBA_ScaleElement(g,rg) + KODE_RGBA_ScaleElement(b,rb) + KODE_RGBA_ScaleElement(a,ra);
          //*ptr++ = (aadd * 255) + KODE_RGBA_ScaleElement(r,ar) + KODE_RGBA_ScaleElement(g,ag) + KODE_RGBA_ScaleElement(b,ab) + KODE_RGBA_ScaleElement(a,aa);
          *ptr++ = (badd * 255) + _scale(r,br) + _scale(g,bg) + _scale(b,bb) + _scale(a,ba);
          *ptr++ = (gadd * 255) + _scale(r,gr) + _scale(g,gg) + _scale(b,gb) + _scale(a,ga);
          *ptr++ = (radd * 255) + _scale(r,rr) + _scale(g,rg) + _scale(b,rb) + _scale(a,ra);
          *ptr++ = (aadd * 255) + _scale(r,ar) + _scale(g,ag) + _scale(b,ab) + _scale(a,aa);
        } //for x
      } //for y
    } //mBuffer
  }
  
  #undef _scale

//------------------------------
//
//------------------------------

  bool clipLine(int32_t AXpos, int32_t ALength, int32_t* AXout, int32_t* ALout) {
    int32_t x = AXpos;
    int32_t l = ALength;
    int32_t w = (int32_t)MWidth;
    if ((x+l) <= 0) return false;
    if (x >= w) return false;
    if (x < 0) { l+=x; x=0; }
    if ((x+l) > w) {
      //KODE_Trace("x %i l %i w %i\n",x,l,w);
      l = w - x;
      //KODE_Trace("l %i\n",l);
    }
    if (l > 0) {
      *AXout = x;
      *ALout = l;
      return true;
    }
    else {
      *AXout = 0;
      *ALout = 0;
      return false;
    }
  }
  
  //----------

  bool clipLine2(int32_t AXpos, int32_t ALength, int32_t AXsrc, int32_t* AXout, int32_t* ALout, int32_t* AX2out) {
    int32_t x  = AXpos;
    int32_t x2 = AXsrc;
    int32_t l  = ALength;
    int32_t w  = (int32_t)MWidth;
    if ((x+l) <= 0) return false;
    if (x >= w) return false;
    if (x < 0) {
      l  += x;
      x2 -= x;
      x   = 0;
    }
    if ((x + l) > w) {
      l = w - x;
    }
    if (l > 0) {
      *AXout  = x;
      *ALout  = l;
      *AX2out = x2;
      return true;
    }
    else {
      //*AXout  = 0;
      //*ALout  = 0;
      //*AX2out = 0;
      return false;
    }
  }

  //----------

  /**
    fill a horizontal line with pixels
  */

  void fillLine(int32_t AXpos, int32_t AYpos, int32_t ALength, uint32_t AColor) {
    if (AYpos < 0) return;
    if (AYpos >= (int32_t)MHeight) return;
    int32_t X,L;
    if (clipLine(AXpos,ALength,&X,&L)) {
      uint32_t* dst = (uint32_t*)getPixelPtr(X,AYpos);
      for (int32_t x=0; x<L; x++) {
        *dst++ = AColor;
      }
    }
  }
  
  void fillLineNoclip(int32_t AXpos, int32_t AYpos, int32_t ALength, uint32_t AColor) {
    uint32_t* dst = (uint32_t*)getPixelPtr(AXpos,AYpos);
    for (int32_t x=0; x<ALength; x++) {
      *dst++ = AColor;
    }
  }
  
  //----------

  /**
    copy a horizontal line of pixels from another bitmap
  */

  void copyLine(int32_t AXpos, int32_t AYpos, int32_t ALength, KODE_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc) {
    if (AYpos < 0) return;
    if (AYpos >= (int32_t)MHeight) return;
    if (AYsrc < 0) return;
    if (AYsrc >= (int32_t)ABitmap->MHeight) return;
    int32_t X,L,X2;
    if (clipLine2(AXpos,ALength,AXsrc,&X,&L,&X2)) {

      uint32_t* dst = (uint32_t*)getPixelPtr(X,AYpos);
      uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(X2,AYsrc);
      for (int32_t x=0; x<L; x++) {
        *dst++ = *src++;
      }
      
    }
  }

  void copyLineNoclip(int32_t AXpos, int32_t AYpos, int32_t ALength, KODE_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc) {
    uint32_t* dst = (uint32_t*)getPixelPtr(AXpos,AYpos);
    uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(AXsrc,AYsrc);
    for (int32_t x=0; x<ALength; x++) {
      *dst++ = *src++;
    }
  }

  //----------

  /**
    blend a horizontal line of pixels from another bitmap
  */

  void blendLine(int32_t AXpos, int32_t AYpos, int32_t ALength, KODE_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc) {
    if (AYpos < 0) return;
    if (AYpos >= (int32_t)MHeight) return;
    if (AYsrc < 0) return;
    if (AYsrc >= (int32_t)ABitmap->MHeight) return;
    int32_t X,L,X2;
    if (clipLine2(AXpos,ALength,AXsrc,&X,&L,&X2)) {
      uint32_t* dst = (uint32_t*)getPixelPtr(X,AYpos);
      uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(X2,AYsrc);
      for (int32_t x=0; x<L; x++) {
        uint32_t  c1 = *dst;
        uint32_t  c2 = *src++;
        uint8_t   a  = c2 >> 24;
        uint32_t  c  = KODE_RGBA_AlphaBlend(c1,c2,a);
        *dst++ = c;
      }
    }
  }

  void blendLineNoclip(int32_t AXpos, int32_t AYpos, int32_t ALength, KODE_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc) {
    uint32_t* dst = (uint32_t*)getPixelPtr(AXpos,AYpos);
    uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(AXsrc,AYsrc);
    for (int32_t x=0; x<ALength; x++) {
      uint32_t  c1 = *dst;
      uint32_t  c2 = *src++;
      uint8_t   a  = c2 >> 24;
      uint32_t  c  = KODE_RGBA_AlphaBlend(c1,c2,a);
      *dst++ = c;
    }
  }

  //----------

  /**
    blend a horizontal line of pixels from another bitmap
  */

  void blendLine(int32_t AXpos, int32_t AYpos, int32_t ALength, KODE_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc, uint8_t AAlpha) {
    if (AYpos < 0) return;
    if (AYpos >= (int32_t)MHeight) return;
    if (AYsrc < 0) return;
    if (AYsrc >= (int32_t)ABitmap->MHeight) return;
    int32_t X,L,X2;
    if (clipLine2(AXpos,ALength,AXsrc,&X,&L,&X2)) {
      uint32_t* dst = (uint32_t*)getPixelPtr(X,AYpos);
      uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(X2,AYsrc);
      for (int32_t x=0; x<L; x++) {
        uint32_t  c1 = *dst;
        uint32_t  c2 = *src++;
        uint32_t  c  = KODE_RGBA_AlphaBlend(c1,c2,AAlpha);
        *dst++ = c;
      }
    }
  }

  void blendLineNoclip(int32_t AXpos, int32_t AYpos, int32_t ALength, KODE_Bitmap* ABitmap, int32_t AXsrc, int32_t AYsrc, uint8_t AAlpha) {
    uint32_t* dst = (uint32_t*)getPixelPtr(AXpos,AYpos);
    uint32_t* src = (uint32_t*)ABitmap->getPixelPtr(AXsrc,AYsrc);
    for (int32_t x=0; x<ALength; x++) {
      uint32_t  c1 = *dst;
      uint32_t  c2 = *src++;
      uint32_t  c  = KODE_RGBA_AlphaBlend(c1,c2,AAlpha);
      *dst++ = c;
    }
  }

  //----------
  /*
  
  todo:
  * the below is not very efficient, clipping is done for every line
  * clip src/dst rectangles first, then call these
  
  */
  //----------
  
  /**
    copies another bitmap into this bitmap
  */

  void copyBitmap(int32_t AXpos, int32_t AYpos, KODE_Bitmap* ABitmap) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      copyLine(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y);
    }
  }

  void copyBitmapNoclip(int32_t AXpos, int32_t AYpos, KODE_Bitmap* ABitmap) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      copyLineNoclip(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y);
    }
  }

  //----------

  /**
    blends another bitmap on top of this bitmap
    alpha taken from ABitmap
  */

  void blendBitmap(int32_t AXpos, int32_t AYpos, KODE_Bitmap* ABitmap) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      blendLine(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y);
    }
  }

  void blendBitmapNoclip(int32_t AXpos, int32_t AYpos, KODE_Bitmap* ABitmap) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      blendLineNoclip(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y);
    }
  }

  //----------

  /**
    blends another bitmap on top of this bitmap
    AAlpha = transparency (for entire bitmap)
  */

  void blendBitmap(int32_t AXpos, int32_t AYpos, KODE_Bitmap* ABitmap, uint8_t AAlpha) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      blendLine(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y,AAlpha);
    }
  }

  void blendBitmapNoclip(int32_t AXpos, int32_t AYpos, KODE_Bitmap* ABitmap, uint8_t AAlpha) {
    for (uint32_t y=0; y<ABitmap->MHeight; y++) {
      blendLineNoclip(AXpos,AYpos+y,ABitmap->MWidth,ABitmap,0,y,AAlpha);
    }
  }

  //----------
  
  // https://hornet.org/code/effects/rotozoom/

  /**
    rotates and zooms a bitmap
  */

  void rotozoom(KODE_Bitmap* ABitmap, float AAngle, float AZoom, float ADstX=0, float ADstY=0, float ASrcX=0, float ASrcY=0, bool AWrap=false) {
    if (AZoom <= 0.0f) return;
    float zoom = 1.0f / AZoom;
    float ca = cosf(AAngle) * zoom;
    float sa = sinf(AAngle) * zoom;
    for (int32_t y=0; y<(int32_t)MHeight; y++) {
      uint32_t* ptr = (uint32_t*)getLinePtr(y);
      for (int32_t x=0; x<(int32_t)MWidth; x++) {
        float xc = (float)(x - ADstX - (int32_t)(MWidth / 2));
        float yc = (float)(y - ADstY - (int32_t)(MHeight / 2));
        float tx = ( (xc * ca) - (yc * sa) ) + (float)(ABitmap->MWidth / 2) + ASrcX;
        float ty = ( (xc * sa) + (yc * ca) ) + (float)(ABitmap->MHeight / 2) + ASrcY;
        uint32_t pixel;
        if (AWrap) {
          pixel = ABitmap->getPixelWrap(tx,ty);
        }
        else {
          pixel = ABitmap->getPixel(tx,ty);
        }
        *ptr++ = pixel;
      }
    }
  }
  
  //----------
  
  // http://www.drdobbs.com/architecture-and-design/fast-bitmap-rotation-and-scaling/184416337

  /*
    src           = source bitmap (to rotate/scale)
    dstCX, dstCY  = src bitmap rotation center
    srcCX, srcCY  = position of center in dst bitmap
    angle         = rotation angle (0..PI2, counter-clockwise, 0 = right
    scale         = scaling
    col           = color to fill empty areas
  */

  void rotate(KODE_Bitmap* src, float dstCX, float dstCY, float srcCX, float srcCY, float angle, float scale, uint32_t col=0xff000000) {
    float duCol = (float)sin(-angle) * (1.0f / scale);
    float dvCol = (float)cos(-angle) * (1.0f / scale);
    float duRow = dvCol;
    float dvRow = -duCol;
    float startingu = srcCX - (dstCX * dvCol + dstCY * duCol);
    float startingv = srcCY - (dstCX * dvRow + dstCY * duRow);
    float rowu = startingu;
    float rowv = startingv;
    for (uint32_t y=0; y<MHeight; y++) {
      float u = rowu;
      float v = rowv;
      uint32_t *pDst = (uint32_t*)getLinePtr(y);
      for (uint32_t x=0; x<MWidth ; x++) {
        //todo:
        //- mode: outside: none, color, texwrap
        //- blend with background
        if (u>0 && v>0 && u<src->getWidth() && v<src->getHeight()) {
          uint32_t *pSrc = (uint32_t*)src->getPixelPtr(u,v);
          *pDst++ = *pSrc++; // blend?
        }
        else {
          *pDst++ = col; // blend?
        }
        u += duRow;
        v += dvRow;
      }
      rowu += duCol;
      rowv += dvCol;
    }
  }

  //----------
  
  //void setPaletteEntry(uint32_t AIndex, uint32_t AColor) {
  //  MPalette[AIndex] = AColor;
  //}
  
  //void setPaletteEntry(uint32_t AIndex, KODE_Color AColor) {
  //  MPalette[AIndex] = KODE_BGRA(AColor);
  //}
  
  void convertFrom8bit(uint8_t* ABuffer, uint32_t* APalette, uint32_t AWidth, uint32_t AHeight) {
    for (uint32_t y=0; y<AHeight; y++) {
      uint32_t* dst = (uint32_t*)getLinePtr(y);
      uint8_t* src = ABuffer + (y * AWidth);
      for (uint32_t x=0; x<AWidth; x++) {
        uint8_t c = *src++;
        *dst++ = APalette[c];
      }
    }
  }
  
  //----------
  
//------------------------------
//
//------------------------------

public:

  //#ifdef KODE_BITMAP_BRESENHAM
  //  #include "gfx/kode_bitmap_bresenham.h"
  //#endif
  
  //#ifdef KODE_BITMAP_WU
  //  #include "gfx/kode_bitmap_wu.h"
  //#endif

  //#ifdef KODE_BITMAP_FONT
  //  #include "gfx/kode_bitmap_font.h"
  //#endif
  
  //#ifdef KODE_BITMAP_VECTOR
  //  #include "gfx/kode_bitmap_vector.h"
  //#endif
  
  //----------
  
  //TODO: move..
  
  // Gupta-Sproull
  /*
  void drawLineAA_GS(int x0, int y0, int x1, int y1, uint32_t c) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int d = 2 * dy - dx;
    int increment_E = 2 * dy;
    int increment_NE = 2 * (dy - dx);
    int dx2 = 0;
    int x = x0;
    int y = y0;
    double inv_denominator = 1.0 / (2.0 * sqrt(dx * dx + dy * dy));
    double inv_denominator_dx2 = 2.0 * dx * inv_denominator;
    blendPixelF(x,y,  c,0);
    blendPixelF(x,y+1,c,inv_denominator_dx2);
    blendPixelF(x,y-1,c,inv_denominator_dx2);
    for (; x < x1;) {
      if (d < 0) {
        dx2 = d + dx;
        d = d + increment_E;
      }
      else {
        dx2 = d - dx;
        d = d + increment_NE;
        y = y + 1;
      }
      x = x + 1;
      blendPixelF(x,y,  c,dx2 * inv_denominator);
      blendPixelF(x,y+1,c,inv_denominator_dx2 - dx2 * inv_denominator);
      blendPixelF(x,y-1,c,inv_denominator_dx2 - dx2 * inv_denominator);
    }
  }
  */

};

//----------------------------------------------------------------------
#endif