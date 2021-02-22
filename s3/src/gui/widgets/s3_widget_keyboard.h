#ifndef s3_widget_keyboard_included
#define s3_widget_keyboard_included
//----------------------------------------------------------------------

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

#include "gui/s3_widget.h"

//----------

// ofs,type

//static
const int32 s3_keyboard_internal_note_data[12*2] = {
  0,0,
  2,3,
  1,1,
  2,3,
  1,2,
  3,0,
  2,3,
  1,1,
  2,3,
  1,1,
  2,3,
  1,2
};


class S3_Widget_Keyboard
: public S3_Widget {

  private:

    bool    MActiveMap[12];
    int32   MNoteRects1[12*4];
    int32   MNoteRects2[12*4];

  protected:

    int32     MDist;
    float     MRatio;
    S3_Color  MBlackColor;
    S3_Color  MWhiteColor;
    S3_Color  MBlackActiveColor;
    S3_Color  MWhiteActiveColor;

  public:

    S3_Widget_Keyboard(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_Keyboard";
      MHint = "keyboard";
      MDist = 2;
      MRatio = 0.7;
      MWhiteColor       = S3_LightGrey;
      MWhiteActiveColor = S3_White;
      MBlackColor       = S3_DarkGrey;
      MBlackActiveColor = S3_Black;
      for (int32 i=0; i<12; i++) MActiveMap[i] = false;
      recalc();
      MCursor = s3_mc_finger;
    }

    virtual ~S3_Widget_Keyboard() {
    }

  public:

    void setColors(S3_Color AWhite, S3_Color ABlack, S3_Color AWhiteActive, S3_Color ABlackActive) {
      MWhiteColor       = AWhite;
      MWhiteActiveColor = AWhiteActive;
      MBlackColor       = ABlack;
      MBlackActiveColor = ABlackActive;
    }

    void setDist(int32 ADist) {
      MDist = ADist + 1;
    }

    void setRatio(float ARatio) {
      MRatio = ARatio;
    }

    void activate(int32 num) {
      MActiveMap[num] = true;
    }

    void deactivate(int32 num) {
      MActiveMap[num] = false;
    }

    //----------

    void recalc(void) {
    /*var
      width : longint;
      step1,step2,step3 : single;
      x1 : single;
      y1,y2 : longint;
      note,n2,n4,iofs : longint;
      ofs,x1s1,x1s2,x1s3:single;
      typ:longint;*/

      int32 width = MRect.w;
      float step1 = (float)width / 21.0;
      float step2 = step1 * 2;
      float step3 = step1 * 3;
      float x1    = 0;
      int32 y1    = floorf((float)MRect.h * MRatio);
      int32 y2    = MRect.h - 1;
      for (int32 note=0; note<12; note++) {
        int32 n2    = note+note;
        int32 n4    = n2+n2;
        int32 iofs  = s3_keyboard_internal_note_data[note*2];
        float ofs   = (float)iofs * step1;
        x1         += ofs;
        float x1s1  = x1 + step1;
        float x1s2  = x1 + step2;
        float x1s3  = x1 + step3;
        int32 typ   = s3_keyboard_internal_note_data[(note*2)+1];
        switch(typ) {
          case 0: // C
            MNoteRects1[n4+0] = floorf(x1);            MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s2) - MDist;  MNoteRects1[n4+3] = y1-1;
            MNoteRects2[n4+0] = floorf(x1);            MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 1: // D
            MNoteRects1[n4+0] = floorf(x1s1);          MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s2) - MDist;  MNoteRects1[n4+3] = y1-1;
            MNoteRects2[n4+0] = floorf(x1);            MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 2: // E
            MNoteRects1[n4+0] = floorf(x1s1);          MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s3) - MDist;  MNoteRects1[n4+3] = y1-1;
            MNoteRects2[n4+0] = floorf(x1);            MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 3: // black
            MNoteRects1[n4+0] = floorf(x1);            MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s2) - MDist;  MNoteRects1[n4+3] = y1 - MDist;
            break;
        } // case
      } // for note
    }

    //----------

  public:

    //virtual
    void on_widgetSetSize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {
      S3_Widget::on_widgetSetSize(AWidget,AWidth,AHeight);
      recalc();
    }

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      for (int32 note=0; note<12; note++) {
        int32 n2 = note*2;
        int32 n4 = note*4;
        bool active = MActiveMap[note];
        int32 typ = s3_keyboard_internal_note_data[n2+1];
        if (typ == 3) {
          if (active) APainter->setFillColor(MBlackActiveColor);
          else APainter->setFillColor(MBlackColor);
          APainter->fillRectangle( MRect.x + MNoteRects1[n4],  MRect.y + MNoteRects1[n4+1],
                                   MRect.x + MNoteRects1[n4+2],MRect.y + MNoteRects1[n4+3] );
        }
        else {
          if (active) APainter->setFillColor(MWhiteActiveColor);
          else APainter->setFillColor(MWhiteColor);
          APainter->fillRectangle( MRect.x + MNoteRects1[n4  ], MRect.y + MNoteRects1[n4+1],
                                   MRect.x + MNoteRects1[n4+2], MRect.y + MNoteRects1[n4+3] );
          APainter->fillRectangle( MRect.x + MNoteRects2[n4  ], MRect.y + MNoteRects2[n4+1],
                                   MRect.x + MNoteRects2[n4+2], MRect.y + MNoteRects2[n4+3] );
        } //type=0,1,2
      } //for note;
    }

    //virtual
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //var
    //  hit,note,n2,n4,typ : longint;
    //  rec : KRect;
      if (AButton == s3_mb_left) {
        S3_Rect rec;
        int32 hit = -1;
        for (int32 note=0; note<12; note++) {
          int32 n4 = note*4;
          rec.x =  MRect.x + MNoteRects1[n4  ];
          rec.y =  MRect.y + MNoteRects1[n4+1];
          rec.w = (MRect.x + MNoteRects1[n4+2]) - rec.x;
          rec.h = (MRect.y + MNoteRects1[n4+3]) - rec.y;
          if (rec.contains(AXpos,AYpos)) {
            hit = note;
            if (MActiveMap[note]) deactivate(note);
            else activate(note);
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_normal);
          } // contain
          if (hit < 0) { // check lower part?
            int32 n2 = note*2;
            int32 typ = s3_keyboard_internal_note_data[n2+1];
            if (typ < 3) { //0,1,2 = white keys
              rec.x =  MRect.x + MNoteRects2[n4  ];
              rec.y =  MRect.y + MNoteRects2[n4+1];
              rec.w = (MRect.x + MNoteRects2[n4+2]) - rec.x;
              rec.h = (MRect.y + MNoteRects2[n4+3]) - rec.y;
              if (rec.contains(AXpos,AYpos)) {
                if (MActiveMap[note]) deactivate(note);
                else activate(note);
                do_widgetUpdate(this);
                do_widgetRedraw(this,MRect,s3_pm_normal);
              } //contains
            } //type 0,1,2
          } //hit
        } // for note
      }
      //axWidget::doMouseDown(aXpos,aYpos,aButton);
    }

  //

};

//----------------------------------------------------------------------
#endif

