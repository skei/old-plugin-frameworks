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

#ifndef axSkinDef_included
#define axSkinDef_included
//----------------------------------------------------------------------

#include "gui/axSkin.h"
//#include "gui/axBitmapLoader.h"
//#include "img/skin1.h"
//#include "img/knob1.h"

//----------


//----------

class axSkinDef : public axSkin
{
  protected:
    bool        mSkinSrfLoaded;
    axSurface*  mSkinSrf;
    axColor     mPanelBackCol;
    axColor     mTextColor;
    axRect      mButtonOffRect;
    axRect      mButtonOnRect;
    bool        mKnobSrfLoaded;
    axSurface*  mKnobSrf;
    int         mKnobWidth;
    int         mKnobHeight;
    int         mKnobCount;
    axColor     mKnobTextCol;
    axColor     mKnobValCol;
    axPoint     mKnobTextOffset;
    axRect      mSliderBackRect;
    axRect      mSliderThumbRect;

  public:

    axSkinDef(axCanvas* aCanvas)
    //: axSkin(aCanvas)
      {
        mSkinSrfLoaded    = false;
        mKnobSrfLoaded    = false;
        mSkinSrf          = NULL;
        mKnobSrf          = NULL;
        mPanelBackCol     = aCanvas->getColor(0x5c,0x64,0x6a);
        mTextColor        = aCanvas->getColor(AX_WHITE);
        mButtonOffRect    = axRect(16, 0,16,16);
        mButtonOnRect     = axRect(16,16,16,16);
        mKnobWidth        = 32;
        mKnobHeight       = 32;
        mKnobCount        = 65;
        mKnobTextCol      = aCanvas->getColor(0,0,0);
        mKnobValCol       = aCanvas->getColor(255,255,255);
        mSliderBackRect   = axRect( 0,  0,16,64);
        mSliderThumbRect  = axRect( 0, 64,16,11);
      }

    //----------

    axSkinDef(axSkinDef* aSkin)
    //: axSkin(aCanvas)
      {
        copyFrom(aSkin);
      }

    //----------

    virtual ~axSkinDef()
      {
        if (mSkinSrfLoaded) delete mSkinSrf;
        if (mKnobSrfLoaded) delete mKnobSrf;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    inline void setupKnob(int mNumImages) { mKnobCount = mNumImages; }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    // be careful about this.... be sure we copy all fields!!!
    // perhaps we should have somethingf better... a proper copy-constructor?

    void copyFrom(axSkinDef* aSkin)
      {
        // set to false, so we don't try to delete the surface in the desctructor
        // (will be done in the 'original' skin)
        mSkinSrfLoaded     = false;
        mKnobSrfLoaded     = false;
        mSkinSrf           = aSkin->mSkinSrf; // 'borrow' pointer
        mKnobSrf           = aSkin->mKnobSrf; // 'borrow' pointer
        mPanelBackCol      = aSkin->mPanelBackCol;
        mTextColor         = aSkin->mTextColor;
        mButtonOffRect     = aSkin->mButtonOffRect;
        mButtonOnRect      = aSkin->mButtonOnRect;
        mKnobWidth         = aSkin->mKnobWidth;
        mKnobHeight        = aSkin->mKnobHeight;
        mKnobCount         = aSkin->mKnobCount;
        mKnobTextCol       = aSkin->mKnobTextCol;
        mKnobValCol        = aSkin->mKnobValCol;
        mSliderBackRect    = aSkin->mSliderBackRect;
        mSliderThumbRect   = aSkin->mSliderThumbRect;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual axBitmap* loadBitmap(axEditor* aEditor, char* aBuffer, int aWidth, int aHeight, int aDepth)
    {
      axBitmap* bitmap = aEditor->createBitmap(aWidth,aHeight,aDepth);
      bitmap->createBuffer(aBuffer);
      bitmap->convertRgbaBgra();
      bitmap->premultAlpha();
      bitmap->prepare();
      return bitmap;
    }

    //--------------------------------------------------

    virtual void loadSkinBitmap(axEditor* aEditor, char* aBuffer, int aWidth, int aHeight, int aDepth)
      {
        axBitmap* bitmap = loadBitmap(aEditor,aBuffer,aWidth,aHeight,aDepth);//256,256);
        mSkinSrf = aEditor->createSurface(aWidth,aHeight,aDepth);//256,256,32);
        mSkinSrf->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,aWidth,aHeight);//256,256 );
        mSkinSrfLoaded = true;
        delete bitmap;
      }

    //----------

    virtual void loadKnobBitmap(axEditor* aEditor, char* aBuffer, int aWidth, int aHeight, int aDepth, int aNum=65)
      {
        axBitmap* bitmap = loadBitmap(aEditor,aBuffer,aWidth,aHeight,aDepth);//32,(32*65));
        mKnobSrf = aEditor->createSurface(aWidth,aHeight,aDepth);//32,(32*65),32);
        mKnobSrf->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,aWidth,aHeight);//32,(32*65) );
        mKnobSrfLoaded = true;
        delete bitmap;
      }

    //--------------------------------------------------
    //
    // draw...
    //
    //--------------------------------------------------

    virtual void drawPanel(axCanvas* aCanvas, axRect aRect, int aMode=0)
      {
        aCanvas->setBrushColor(mPanelBackCol);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
      }

    //----------

    //virtual void drawLabel(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign, int aMode=0)
    //  {
    //    aCanvas->setTextColor(m_TextCol);
    //    aCanvas->drawText(aRect.x,aRect.y,aRect.x2(),aRect.y2(),aText,aTextAlign);
    //  }

    //----------

    virtual void drawButton(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign, bool aState, int aMode=0)
      {
        //switch (aMode)
        //{
        //  case 0:
//            if (aState) aCanvas->renderImage(mSkinSrf, aRect.x,aRect.y, mButtonOnRect.x, mButtonOnRect.y, mButtonOnRect.w, mButtonOnRect.h );
//                   else aCanvas->renderImage(mSkinSrf, aRect.x,aRect.y, mButtonOffRect.x,mButtonOffRect.y,mButtonOffRect.w,mButtonOffRect.h);
            if (aState) aCanvas->renderSurface(mSkinSrf, aRect.x,aRect.y, mButtonOnRect.x, mButtonOnRect.y, mButtonOnRect.w, mButtonOnRect.h );
                   else aCanvas->renderSurface(mSkinSrf, aRect.x,aRect.y, mButtonOffRect.x,mButtonOffRect.y,mButtonOffRect.w,mButtonOffRect.h);
        //    break;
        //  case 1:
        //    if (aState) aCanvas->renderImage(m_SkinSrf, aRect.x,aRect.y, m_Button2OnRect.x, m_Button2OnRect.y, m_Button2OnRect.w, m_Button2OnRect.h );
        //           else aCanvas->renderImage(m_SkinSrf, aRect.x,aRect.y, m_Button2OffRect.x,m_Button2OffRect.y,m_Button2OffRect.w,m_Button2OffRect.h);
        //    break;
        //}
      }

    //----------

    virtual void drawSlider(axCanvas* aCanvas, axRect aRect, float aValue, axString aText1, axString aText2, bool aVertical, int aMode=0)
      {
        float h;
        int y;
        //trace("drawSlider");
        if (aVertical)
        {
          //switch (mVariation)
          //switch (aMode)
          //{
          //  case 0:
              h = (float)(aRect.h - mSliderThumbRect.h);
              y   = aRect.y + (int)(h*(1-aValue));
//              aCanvas->renderImage(mSkinSrf, aRect.x, aRect.y, mSliderBackRect.x, mSliderBackRect.y, mSliderBackRect.w, mSliderBackRect.h );
//              aCanvas->renderImage(mSkinSrf, aRect.x, y,       mSliderThumbRect.x,mSliderThumbRect.y,mSliderThumbRect.w,mSliderThumbRect.h);
              aCanvas->renderSurface(mSkinSrf, aRect.x, aRect.y, mSliderBackRect.x, mSliderBackRect.y, mSliderBackRect.w, mSliderBackRect.h );
              aCanvas->renderSurface(mSkinSrf, aRect.x, y,       mSliderThumbRect.x,mSliderThumbRect.y,mSliderThumbRect.w,mSliderThumbRect.h);
          //    break;
          //  case 1:
          //    h = (float)(aRect.h - m_Slider2ThumbRect.h);
          //    y   = aRect.y + (int)(h*(1-aValue));
          //    aCanvas->renderImage(m_SkinSrf, aRect.x, aRect.y, m_Slider2BackRect.x, m_Slider2BackRect.y, m_Slider2BackRect.w, m_Slider2BackRect.h );
          //    aCanvas->renderImage(m_SkinSrf, aRect.x, y,       m_Slider2ThumbRect.x,m_Slider2ThumbRect.y,m_Slider2ThumbRect.w,m_Slider2ThumbRect.h);
          //    break;
          //}
        }
        else
        {
        }
      }

    //----------

    virtual void drawKnob(axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue, int aMode=0)
      {
        if (mKnobSrf)
        {
          // bitmap
          int index = (int)axFloor(aValue*mKnobCount);
          index = axMinInt(index,mKnobCount-1);
          int ky = mKnobHeight * index;
          //aCanvas->renderImage(mKnobSrf,aRect.x,aRect.y, 0,ky,mKnobWidth,mKnobHeight);
          aCanvas->renderSurface(mKnobSrf,aRect.x,aRect.y, 0,ky,mKnobWidth,mKnobHeight);
          int x  = aRect.x;
          int y  = aRect.y;
          int size = axMinInt(aRect.w,aRect.h);
          int th = aCanvas->textHeight("Xj");
          aCanvas->setTextColor(mKnobTextCol);
          //aCanvas->setTextSize(14);
          aCanvas->drawText(x+size+8,y,   aRect.x2(),aRect.y2(),aName,ta_Top|ta_Left);
          aCanvas->setTextColor(mKnobValCol);
          //aCanvas->setTextSize(8);
          aCanvas->drawText(x+size+8,y+th,aRect.x2(),aRect.y2(),aDisp,ta_Top|ta_Left);
        }
      }
};

//----------------------------------------------------------------------
#endif

//#include "../extern/stb_truetype.h"
//
//#if 0
//
//char            ttf_buffer[1<<20];
//stbtt_fontinfo  ttf_font;
//unsigned char   ttf_screen[20][79];
//
////int i,j, pos=0;
////unsigned char *bitmap;
//
//memset(ttf_screen,0,sizeof(ttf_screen));
//
////fread(ttf_buffer,1,1000000,fopen("c:/windows/fonts/arialbd.ttf","rb"));
//stbtt_InitFont(&ttf_font,ttf_buffer,0);
//float scale = stbtt_ScaleForPixelHeight(&ttf_font,16);
//
//int baseline = 13;
//char* text = "Heljo World!";
//int   pos = 0;
//while (*text)
//{
//  int advance, lsb;
//  int x0,y0,x1,y1;
//
//  stbtt_GetCodepointHMetrics(&ttf_font,*text,&advance,&lsb);
//  stbtt_GetCodepointBitmapBox(&ttf_font,*text,scale,scale,&x0,&y0,&x1,&y1);
//
//  int newpos = pos + (int)(lsb*scale) + x0;
//  stbtt_MakeCodepointBitmap(&ttf_font, &ttf_screen[baseline+y0][newpos],x1-x0,y1-y0,79,scale,scale,*text);
//  // note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
//  // because this API is really for baking character bitmaps into textures
//  pos += (int)(advance*scale);
//  ++text;
//}
//
////for (j=0; j < 20; ++j)
////{
////  for (i=0; i < 79; ++i)
////    putchar(" .:ioVM@"[screen[j][i]>>5]);
////  putchar('\n');
////}
//
//#endif
