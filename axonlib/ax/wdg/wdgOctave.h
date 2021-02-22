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

#ifndef wdgOctave_included
#define wdgOctave_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

/*
  mind-storm:
    black or white, black = 1 rect, white = 2 rect
    only 2 numbers per rectangle is unique
    use rectangles for hit testing (mouse click)
    precalc them in setup(scale,dist,y1,y2) ?
    array with 12 elememnts, one for each note in octave
    1 find octave
    octave width = mScale*3*7
    so, a key could be
    - int black/white
    -  num1,num2
    -  num1,num2


*/

//
//  |  |4| | |  |
//  |  |_| |_|  |
//  |1  |2  |3  |
//  |___|___|___|
//

// ofs,type
static int internal_note_data[12*2] =
{
   0, 0,
   2, 3,
   1, 1,
   2, 3,
   1, 2,
   3, 0,
   2, 3,
   1, 1,
   2, 3,
   1, 1,
   2, 3,
   1, 2
};

//----------------------------------------------------------------------

class wdgOctave : public axWidget
{
  private:
    bool    mActiveMap[12];
    int     mNoteRects1[12*4];
    int     mNoteRects2[12*4];
  protected:
    int     mDist;
    float   mRatio;
    axColor mBlackCol;
    axColor mWhiteCol;
    axColor mBlackColActive;
    axColor mWhiteColActive;

  public:

    wdgOctave(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        mDist   = 2;
        mRatio  = 0.7;
        for (int i=0;i<12;i++) mActiveMap[i]=false;
        recalc();
      }

    inline void setDist(int aDist)      { mDist=aDist+1; }
    inline void setRatio(float aRatio)  { mRatio=aRatio; }
    inline void activate(int num)       { mActiveMap[num]=true; }
    inline void deactivate(int num)     { mActiveMap[num]=false; }

    //--------------------------------------------------

    void setColors(axColor aWhite, axColor aBlack, axColor aWhiteActive, axColor aBlackActive)
      {
        mWhiteCol       = aWhite;
        mWhiteColActive = aWhiteActive;
        mBlackCol       = aBlack;
        mBlackColActive = aBlackActive;
      }

    //----------

    void recalc()
      {
        int width = mRect.w;
        float step1 = (float)width / 21.0f;
        float step2 = step1*2;
        float step3 = step1*3;
        float x1    = 0;
        int   y1    = mRect.h * mRatio;
        int   y2    = mRect.h - 1;
        for (int note=0;note<12;note++)
        {
          int n2 = note+note;
          int n4 = n2+n2;
          int iofs = internal_note_data[note*2];
          float ofs = (float)iofs * step1;
          x1 += ofs;
          float x1s1 = x1 + step1;
          float x1s2 = x1 + step2;
          float x1s3 = x1 + step3;
          int type = internal_note_data[(note*2)+1];
          switch(type)
          {
            case 0: // C
              mNoteRects1[n4+0] = axFloor(x1);            mNoteRects1[n4+1] = 0;
              mNoteRects1[n4+2] = axFloor(x1s2) - mDist;  mNoteRects1[n4+3] = y1-1;
              mNoteRects2[n4+0] = axFloor(x1);            mNoteRects2[n4+1] = y1;
              mNoteRects2[n4+2] = axFloor(x1s3) - mDist;  mNoteRects2[n4+3] = y2;
              break;
            case 1: // D
              mNoteRects1[n4+0] = axFloor(x1s1);          mNoteRects1[n4+1] = 0;
              mNoteRects1[n4+2] = axFloor(x1s2) - mDist;  mNoteRects1[n4+3] = y1-1;
              mNoteRects2[n4+0] = axFloor(x1);            mNoteRects2[n4+1] = y1;
              mNoteRects2[n4+2] = axFloor(x1s3) - mDist;  mNoteRects2[n4+3] = y2;
              break;
            case 2: // E
              mNoteRects1[n4+0] = axFloor(x1s1);          mNoteRects1[n4+1] = 0;
              mNoteRects1[n4+2] = axFloor(x1s3) - mDist;  mNoteRects1[n4+3] = y1-1;
              mNoteRects2[n4+0] = axFloor(x1);            mNoteRects2[n4+1] = y1;
              mNoteRects2[n4+2] = axFloor(x1s3) - mDist;  mNoteRects2[n4+3] = y2;
              break;
            case 3: // black
              mNoteRects1[n4+0] = axFloor(x1);            mNoteRects1[n4+1] = 0;
              mNoteRects1[n4+2] = axFloor(x1s2) - mDist;  mNoteRects1[n4+3] = y1 - mDist;
              break;
          } //switch type
        } //for note
      }

    //--------------------------------------------------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        axWidget::doSetSize(aWidth,aHeight);
        recalc();
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {

        // test/debug
        mWhiteCol       = aCanvas->getColor(255,255,255);
        mWhiteColActive = aCanvas->getColor(192,192,192);
        mBlackCol       = aCanvas->getColor(  0,  0,  0);
        mBlackColActive = aCanvas->getColor( 96, 96, 96);

        for (int note=0; note<12; note++)
        {
          int n2 = note*2;
          int n4 = note*4;
          bool active = mActiveMap[note];
          int type = internal_note_data[n2+1];
          if (type==3)
          {
            if (active) aCanvas->setBrushColor(mBlackColActive);
            else aCanvas->setBrushColor(mBlackCol);
            aCanvas->fillRect( mRect.x + mNoteRects1[n4],  mRect.y + mNoteRects1[n4+1],
                               mRect.x + mNoteRects1[n4+2],mRect.y + mNoteRects1[n4+3] );
          }
          else
          {
            if (active) aCanvas->setBrushColor(mWhiteColActive);
            else aCanvas->setBrushColor(mWhiteCol);
            aCanvas->fillRect( mRect.x + mNoteRects1[n4  ], mRect.y + mNoteRects1[n4+1],
                               mRect.x + mNoteRects1[n4+2], mRect.y + mNoteRects1[n4+3] );
            aCanvas->fillRect( mRect.x + mNoteRects2[n4  ], mRect.y + mNoteRects2[n4+1],
                               mRect.x + mNoteRects2[n4+2], mRect.y + mNoteRects2[n4+3] );
          } //type=0,1,2
        } //for note
      }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left)
        {
          int hit = -1;
          axRect rec;
          for (int note=0;note<12;note++)
          {
            int n4 = note*4;
            rec.x =  mRect.x + mNoteRects1[n4  ];
            rec.y =  mRect.y + mNoteRects1[n4+1];
            rec.w = (mRect.x + mNoteRects1[n4+2]) - rec.x;
            rec.h = (mRect.y + mNoteRects1[n4+3]) - rec.y;
            if (rec.contains(aXpos,aYpos))
            {
              hit = note;
              if (mActiveMap[note])
              {
                deactivate(note);
                mListener->onRedraw(this);
              }
              else
              {
                activate(note);
                mListener->onRedraw(this);
              }
            } //contains
            if (hit<0) // check lower part?
            {
              int n2 = note*2;
              int type = internal_note_data[n2+1];
              if (type<3) //0,1,2 = white keys
              {
                rec.x =  mRect.x + mNoteRects2[n4  ];
                rec.y =  mRect.y + mNoteRects2[n4+1];
                rec.w = (mRect.x + mNoteRects2[n4+2]) - rec.x;
                rec.h = (mRect.y + mNoteRects2[n4+3]) - rec.y;
                if (rec.contains(aXpos,aYpos))
                {
                  if (mActiveMap[note])
                  {
                    deactivate(note);
                    mListener->onChange(this);
                  }
                  else
                  {
                    activate(note);
                    mListener->onChange(this);
                  }
                } //contains
              } //type 0,1,2
            } //hit
          } // for note
        }
        //axWidget::doMouseDown(aXpos,aYpos,aButton);
      }

    virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_Finger); }
    virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif
