#ifndef s3_bitmap_included
#define s3_bitmap_included
//----------------------------------------------------------------------

/// 32-bit memory bitmap

/**
  ram/client bitmap, rgba (32bit)

  MBuffer[0] = b
  MBuffer[1] = g
  MBuffer[2] = r
  MBuffer[3] = a

*/

//----------------------------------------------------------------------

#ifndef S3_NO_BITMAP_PNG
  #define UPNG_NO_LOAD_FROM_FILE
  //#include "extern/upng/upng/upng.h"
  #include "extern/upng/upng.c.h"
#endif // S3_NO_BITMAP_PNG

//----------

#include "common/s3_color.h"
#include "common/s3_rect.h"

//----------------------------------------------------------------------

class S3_Bitmap {

  private:

    int32  MWidth;
    int32  MHeight;
    //uint32  MDepth;
    int32  MPixelSize;
    int32  MBufferSize;
    uint8*  MBuffer;
    bool    MBufferAllocated;

    upng_t* upng;

  //------------------------------
  //
  //------------------------------

  public:

    /// Create new bitmap with specified size

    /**
      Create new bitmap with specified size
      @param AWidth Width of bitmap
      @param AHeight Height of bitmap
    */

    S3_Bitmap(int32 AWidth, int32 AHeight/*, uint32 ADepth*/) {
      MWidth      = AWidth;
      MHeight     = AHeight;
      //MDepth      = ADepth;
      MPixelSize  = sizeof(uint32); // rgba/32bit
      MBufferSize = AWidth * AHeight * MPixelSize;
      MBuffer     = (uint8*)S3_Malloc(MBufferSize);
      MBufferAllocated = true;
    }

    //----------

    /// Create from existing buffer

    /**
      Create bitmap from size and buffer
      @param AWidth Width of bitmap
      @param AHeight Height of bitmap
      @param ABuffer Pointer to memory buffer
    */

    S3_Bitmap(int32 AWidth, int32 AHeight/*, uint32 ADepth*/, uint32* ABuffer) {
      MWidth      = AWidth;
      MHeight     = AHeight;
      //MDepth      = ADepth;
      MPixelSize  = sizeof(uint32);                               // rgba/32bit
      MBufferSize = AWidth * AHeight * MPixelSize;
      MBuffer     = (uint8*)ABuffer;
      MBufferAllocated = false;
    }

    //----------

    /// Create from in-memory png

    /**
      Creates bitmap from pointer to in-memory png
      @param buffer Pointer to buffer
      @param length Size of buffer (in bytes)
    */

    #ifndef S3_NO_BITMAP_PNG
    S3_Bitmap(const uint8* buffer, uint32 length) {
      upng = upng_new_from_bytes(buffer,length);
      if (upng) {
        upng_header(upng);
        MWidth = upng_get_width(upng);
        MHeight = upng_get_height(upng);
        MPixelSize  = sizeof(uint32);                             // rgba/32bit
        MBufferSize = MWidth * MHeight * MPixelSize;
        MBuffer     = (uint8*)S3_Malloc(MBufferSize);
        MBufferAllocated = true;
        upng_decode(upng);
        S3_Memcpy(MBuffer,upng_get_buffer(upng),upng_get_size(upng));
        upng_free(upng);
        upng = S3_NULL;
      }
    }
    #endif

    //----------

    virtual ~S3_Bitmap() {
      deleteBuffer();
    }

  //------------------------------
  //
  //------------------------------

  public:

    int32   width(void)  { return MWidth; }           ///< Returns width of bitmap (in pixels)
    int32   height(void) { return MHeight; }          ///< Returns height of bitmap (in pixels)
    //uint32  depth(void)  { return MDepth; }         ///< Returns depth of bitmap (bits per pixel))
    uint32* buffer(void) { return (uint32*)MBuffer; } ///< Returns pointer to memory buffer

  //------------------------------
  //
  //------------------------------

  public:

    /**
      Delete allocated buffer
    */

    void deleteBuffer(void) {
      if (MBufferAllocated && MBuffer) S3_Free(MBuffer);
      MBufferAllocated = false;
      MBuffer = S3_NULL;
    }

    //----------

    /**
      Set buffer pointer
      @param ABuffer Pointer to buffer
    */

    void setBuffer(void* ABuffer) {
      deleteBuffer();
      MBufferAllocated = false;
      MBuffer = (uint8*)ABuffer;
    }

    //----------

    /**
      Clear memory buffer with specified value
      @param AValue Value to fill buffer with (default = 0)
    */

    void clearBuffer(uint8 AValue=0) {
      int32 size = MWidth*MHeight*4;
      S3_Memset(MBuffer,AValue,size);
    }

    //----------

    /**
      Fill memory buffer with specified 32-bit value
      @param AValue 32-bit integer (rgba)
    */

    void fillBuffer(uint32 AValue) {
      int32 size = MWidth*MHeight;
      uint32* ptr = (uint32*)MBuffer;
      for (int32 i=0; i<size; i++) *ptr++ = AValue;
    }

    //----------

    /**
      Fill (only) alpha part of bitmap with specified value
      @param AValue Alpha value
    */

    void fillAlpha(uint8 AValue) {
      //uint32* ptr = (uint32*)MBuffer[3];
      uint8* ptr = (uint8*)&MBuffer[3]; // ptr to first alpha value (r,g,b,a)
      int32 size = MWidth*MHeight;
      for (int32 i=0; i<size; i++) {
        *ptr = AValue;
        ptr += 4;
      }
    }

    //----------

    /**
      Returns pixel at specified position
      @param x x pos
      @param y y pos
      @return pixel (32-bit rgba)
    */

    //inline
    uint32 getPixel(uint32 x, uint32 y) {
      //SAssert( x >= 0 );
      //SAssert( y >= 0 );
      //SAssert( x < (uint32)MWidth );
      //SAssert( y < (uint32)MHeight );
      if (x>=(uint32)MWidth) x = MWidth - 1;
      if (y>=(uint32)MHeight) y = MHeight - 1;
      uint32* ptr = (uint32*)MBuffer;
      return ptr[y*MWidth+x];
    }

    //----------

    /**
      Set pixel at specified position
      @param x x pos
      @param y y pos
      @param c color (32-bit rgba)
    */

    //inline
    void setPixel(uint32 x, uint32 y, uint32 c) {
      //SAssert( x >= 0 );
      //SAssert( y >= 0 );
      //SAssert( x < (uint32)MWidth );
      //SAssert( y < (uint32)MHeight );
      if (x>=(uint32)MWidth) x = MWidth - 1;
      if (y>=(uint32)MHeight) y = MHeight - 1;

      uint32* ptr = (uint32*)MBuffer;
      ptr[y*MWidth+x] = c;
    }

  //------------------------------
  //
  //------------------------------

  public:

    /**
      Converts bitmap from RGBA to BGRA (little endian)
    */

    void convertRgbaBgra(void) {
      if (MBuffer) {
        for (int32 y=0; y<MHeight; y++) {
          for (int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            MBuffer[pos+0] = b;
            MBuffer[pos+1] = g;
            MBuffer[pos+2] = r;
            MBuffer[pos+3] = a;
          } //for x
        } //for y
      } //mBuffer
    }

    //----------

    /**
      Pre-multiply colors with alpha, and speficied background color
      @param AR Red
      @param AG Green
      @param AB Blue
    */

    void setBackground(uint8 AR, uint8 AG, uint8 AB) {
      if (MBuffer) {
        for (int32 y=0; y<MHeight; y++) {
          for (int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            MBuffer[pos+0] = S3_AlphaColor(r,a) + S3_AlphaColor(AR,(255-a));
            MBuffer[pos+1] = S3_AlphaColor(g,a) + S3_AlphaColor(AG,(255-a));
            MBuffer[pos+2] = S3_AlphaColor(b,a) + S3_AlphaColor(AB,(255-a));
            MBuffer[pos+3] = a;
          } //for x
        } //for y
      } //mBuffer
    }

    //----------

    /**
      Pre-multiply colors with alpha, and specified bitmap
      @param ABitmap Bitmap to use as background
      @param ARect Area of ABitmap
    */

    //TODO:
    void setBackground(S3_Bitmap* ABitmap, S3_Rect ARect) {
      uint8* buf2 = (uint8*)ABitmap->buffer();
      uint32 h = ABitmap->height();
      //trace("aBitmap->getHeight() = " << h );
      if (MBuffer) {
        uint32 x2 = ARect.x;
        uint32 y2 = ARect.y;
        uint32 w2 = ARect.w;//x2();
        uint32 h2 = ARect.h;//y2();
        for (int32 y=0; y<MHeight; y++) {
          x2 = ARect.x;
          for(int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            uint32 pos2 = (y2*h + x2) * 4;
            //trace("pos2 = " << pos2 );
            uint8 r2 = buf2[pos2+0];
            uint8 g2 = buf2[pos2+1];
            uint8 b2 = buf2[pos2+2];
            //uint8 a2 = buf2[pos+3];

            MBuffer[pos+0] = S3_AlphaColor(r,a) + S3_AlphaColor(r2,(255-a));
            MBuffer[pos+1] = S3_AlphaColor(g,a) + S3_AlphaColor(g2,(255-a));
            MBuffer[pos+2] = S3_AlphaColor(b,a) + S3_AlphaColor(b2,(255-a));
            MBuffer[pos+3] = a;

            x2++;
            if (x2>=w2) x2 = ARect.x;
          } //for x
          y2++;
          if (y2>=h2) y2 = ARect.y;
        } //for y

      } //mBuffer
    }

    //----------

    /**
      Pre-multiply colors with alpha
    */

    void premultAlpha(void) {
      if (MBuffer) {
        for(int32 y=0; y<MHeight; y++) {
          for(int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            MBuffer[pos+0] = S3_AlphaColor(r,a);
            MBuffer[pos+1] = S3_AlphaColor(g,a);
            MBuffer[pos+2] = S3_AlphaColor(b,a);
            MBuffer[pos+3] = a;
          } //for x
        } //for y
      } //mBuffer
    }


    //----------

    /*
      swap colors around
      with multiply factor
    */

    /**
      matrix-multiply colors & buffer
    */

    void colorMatrix( float rr, float rg, float rb, float ra,
                      float gr, float gg, float gb, float ga,
                      float br, float bg, float bb, float ba,
                      float ar, float ag, float ab, float aa) {
      if (MBuffer) {
        //dtrace("swizzle");
        for (int32 y=0; y<MHeight; y++) {
          for (int32 x=0; x<MWidth; x++) {
            int32 pos = (y*MWidth + x) * 4;
            uint8 r = MBuffer[pos+0];
            uint8 g = MBuffer[pos+1];
            uint8 b = MBuffer[pos+2];
            uint8 a = MBuffer[pos+3];
            MBuffer[pos+0] = S3_ScaleColor(r,rr) + S3_ScaleColor(g,rg) + S3_ScaleColor(b,rb) + S3_ScaleColor(a,ra);
            MBuffer[pos+1] = S3_ScaleColor(r,gr) + S3_ScaleColor(g,gg) + S3_ScaleColor(b,gb) + S3_ScaleColor(a,ga);
            MBuffer[pos+2] = S3_ScaleColor(r,br) + S3_ScaleColor(g,bg) + S3_ScaleColor(b,bb) + S3_ScaleColor(a,ba);
            MBuffer[pos+3] = S3_ScaleColor(r,ar) + S3_ScaleColor(g,ag) + S3_ScaleColor(b,ab) + S3_ScaleColor(a,aa);
            //mBuffer[pos+3] = a;
          } //for x
        } //for y
      } //mBuffer
    }

  //------------------------------
  //
  //------------------------------

  public:

    /*
    void greyScale(void) {
      double r_scaler = 0.299;
      double g_scaler = 0.587;
      double b_scaler = 0.114;
      for (unsigned char* itr = data_; itr < (data_ + length_); ) {
        unsigned char gray_value = static_cast<unsigned char>((r_scaler * (*(itr + 2))) +
                                                              (g_scaler * (*(itr + 1))) +
                                                              (b_scaler * (*(itr + 0))) );
        *(itr++) = gray_value;
        *(itr++) = gray_value;
        *(itr++) = gray_value;
      }
    }
    */

    //----------
    //----------

    void flipHorizontal(void) {}
    void flipVertical(void) {}

    // http://lodev.org/cgtutor/filtering.html

  //------------------------------
  //
  //------------------------------

  public:

    /**
      Set pixel with alpha
      @param x x-pos
      @param y x-pos
      @param c color
      @param AAlpha alpha
    */

    void blendPixel(uint32 x, uint32 y, uint32 c, uint8 AAlpha) {
      //SAssert( x >= 0 );
      //SAssert( y >= 0 );
      //SAssert( x < (uint32)MWidth );
      //SAssert( y < (uint32)MHeight );
      if (x>=(uint32)MWidth) x = MWidth-1;
      if (y>=(uint32)MHeight) y = MHeight-1;
      int32 pos = (y*MWidth + x) * 4;
      uint8 bb = MBuffer[pos+0];
      uint8 bg = MBuffer[pos+1];
      uint8 br = MBuffer[pos+2];
      uint8 cr = (c >> 16) & 0xff;
      uint8 cg = (c >> 8 ) & 0xff;
      uint8 cb = (c      ) & 0xff;
      MBuffer[pos+0] = S3_AlphaColor(cb,AAlpha) + S3_AlphaColor(bb,(255-AAlpha));
      MBuffer[pos+1] = S3_AlphaColor(cg,AAlpha) + S3_AlphaColor(bg,(255-AAlpha));
      MBuffer[pos+2] = S3_AlphaColor(cr,AAlpha) + S3_AlphaColor(br,(255-AAlpha));
    }

  //------------------------------
  //
  //------------------------------

  public:

    //#dfine BRESENHAM_INCLUDE

    //#ifndef S3_NO_BITMAP_BRESENHAM
    #include "s3_bresenham.h"
    //#endif // S3_NO_BITMAP_BRESENHAM

  //------------------------------
  // scan conversion/rasterization
  //------------------------------

  private:

    int32   MLeftEdge[4096];  // 24.8 fp
    int32   MRightEdge[4096]; // 24.8 fp
    int32   min_y;
    int32   max_y;

  public:

    /*
      - line should be clipped
      - 24.8 fp coordinates
    */

    /**
      Reset scan buffers
    */

    void resetScanBuffer(void) {
      min_y =  S3_INT32_MAX;
      max_y = -S3_INT32_MAX;
    }

    /**
      Scan left side of shape
      @param x1 top x-pos
      @param y1 top y-pos
      @param x2 bottom x-pos
      @param y2 bottom y-pos
    */

    void scanLeftLine(int32 x1, int32 y1, int32 x2, int32 y2) {
      if (y1 < min_y) min_y = y1;
      if (y2 < min_y) min_y = y2;
      if (y1 > max_y) max_y = y1;
      if (y2 > max_y) max_y = y2;
      int32 w = x2-x1;
      int32 h = y2-y1;
      int32 x = x1 << 8;
      int32 d = (w<<8) / h;
      for (int32 y=y1; y<y2; y++) {
        MLeftEdge[y] = x;
        x += d;
      }
    }

    /**
      Scan right side of shape
      @param x1 top x-pos
      @param y1 top y-pos
      @param x2 bottom x-pos
      @param y2 bottom y-pos
    */

    void scanRightLine(int32 x1, int32 y1, int32 x2, int32 y2) {
      if (y1 < min_y) min_y = y1;
      if (y2 < min_y) min_y = y2;
      if (y1 > max_y) max_y = y1;
      if (y2 > max_y) max_y = y2;
      int32 w = x2-x1;
      int32 h = y2-y1;
      int32 x = x1 << 8;
      int32 d = (w<<8) / h;
      for (int32 y=y1; y<y2; y++) {
        MRightEdge[y] = x;
        x += d;
      }
    }

    /**
      Fill scanned lines with specified color, (horizontally) antialiased
      @param AColor color
    */

    void drawScanBuffer(S3_Color AColor) {
      for (int32 y=min_y; y<=max_y; y++) {
        uint32 color = S3_RGB(AColor);
        uint8 alpha = color >> 24;
        color &= 0xffffff;
        int32 x1 = MLeftEdge[y];
        int32 x2 = MRightEdge[y];
        uint8 leftBlend = x1 & 255;
        uint8 rightBlend = x2 & 255;
        x1 >>= 8;
        x2 >>= 8;
        // left pixel antialiasing
        uint32 c = S3_AlphaMix( getPixel(x1,y), color, alpha );
        blendPixel(x1,y,c,255-leftBlend);
        x1++;
        // right pixel antialiasing
        c = S3_AlphaMix( getPixel(x2,y), color, alpha );
        blendPixel(x2,y,c,rightBlend);
        x2--;
        // blend full-color pixels
        for (int32 x=x1; x<=x2; x++) {
          c = S3_AlphaMix( getPixel(x,y), color, alpha );
          setPixel(x,y,c);
        }
      }
    }

    //----------

    //TODO: curves..
};

//----------------------------------------------------------------------
#endif

