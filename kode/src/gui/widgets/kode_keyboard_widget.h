#ifndef kode_keyboard_widget_included
#define kode_keyboard_widget_included
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

#include "gui/kode_widget.h"

//----------

// ofs,type

//static
const int32_t kode_keyboard_internal_note_data[12*2] = {
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


class KODE_KeyboardWidget
: public KODE_Widget {

  private:

    bool        MActiveMap[12];
    int32_t     MNoteRects1[12*4];
    int32_t     MNoteRects2[12*4];

  protected:

    int32_t     MDist;
    float       MRatio;
    KODE_Color  MBlackColor;
    KODE_Color  MWhiteColor;
    KODE_Color  MBlackActiveColor;
    KODE_Color  MWhiteActiveColor;

  public:

    KODE_KeyboardWidget(KODE_Rect ARect)
    : KODE_Widget(ARect) {
      MName = "KODE_KeyboardWidget";
      MHint = "keyboard";
      MDist = 2;
      MRatio = 0.7;
      MWhiteColor       = KODE_LightGrey;
      MWhiteActiveColor = KODE_White;
      MBlackColor       = KODE_DarkGrey;
      MBlackActiveColor = KODE_Black;
      for (int32_t i=0; i<12; i++) MActiveMap[i] = false;
      //recalc();
      MCursor = KODE_CURSOR_FINGER;
    }

    virtual ~KODE_KeyboardWidget() {
    }

  public:

    void setColors(KODE_Color AWhite, KODE_Color ABlack, KODE_Color AWhiteActive, KODE_Color ABlackActive) {
      MWhiteColor       = AWhite;
      MWhiteActiveColor = AWhiteActive;
      MBlackColor       = ABlack;
      MBlackActiveColor = ABlackActive;
    }

    void setDist(int32_t ADist) {
      MDist = ADist + 1;
    }

    void setRatio(float ARatio) {
      MRatio = ARatio;
    }

    void activate(int32_t num) {
      MActiveMap[num] = true;
    }

    void deactivate(int32_t num) {
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

      //KODE_Trace("MRect.w %i\n",(int)MRect.w);

      int32_t width = MRect.w;
      float step1 = (float)width / 21.0;
      float step2 = step1 * 2;
      float step3 = step1 * 3;
      float x1    = 0;
      int32_t y1    = floorf((float)MRect.h * MRatio);
      int32_t y2    = MRect.h - 1;
      for (int32_t note=0; note<12; note++) {
        int32_t n2    = note+note;
        int32_t n4    = n2+n2;
        int32_t iofs  = kode_keyboard_internal_note_data[note*2];
        float ofs   = (float)iofs * step1;
        x1         += ofs;
        float x1s1  = x1 + step1;
        float x1s2  = x1 + step2;
        float x1s3  = x1 + step3;
        int32_t typ   = kode_keyboard_internal_note_data[(note*2)+1];
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
        } // caseKWidget_Keyboard
      } // for note
    }

    //----------

public:
  
    //void setSize(float w, float h) override {
    //  KODE_Widget::setSize(w,h);
    //  recalc();
    //}

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      
      recalc();

      //KODE_TRACE;

      for (int32_t note=0; note<12; note++) {
        int32_t n2 = note*2;
        int32_t n4 = note*4;
        bool active = MActiveMap[note];
        int32_t typ = kode_keyboard_internal_note_data[n2+1];
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

    void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
    //var
    //  hit,note,n2,n4,typ : longint;
    //  rec : KRect;
      if (AButton == KODE_BUTTON_LEFT) {
        KODE_Rect rec;
        int32_t hit = -1;
        for (int32_t note=0; note<12; note++) {
          int32_t n4 = note*4;
          rec.x =  MRect.x + MNoteRects1[n4  ];
          rec.y =  MRect.y + MNoteRects1[n4+1];
          rec.w = (MRect.x + MNoteRects1[n4+2]) - rec.x;
          rec.h = (MRect.y + MNoteRects1[n4+3]) - rec.y;
          if (rec.contains(AXpos,AYpos)) {
            hit = note;
            if (MActiveMap[note]) deactivate(note);
            else activate(note);
            do_update(this);
            do_redraw(this,MRect/*,krs_opaque*/);
          } // contain
          if (hit < 0) { // check lower part?
            int32_t n2 = note*2;
            int32_t typ = kode_keyboard_internal_note_data[n2+1];
            if (typ < 3) { //0,1,2 = white keys
              rec.x =  MRect.x + MNoteRects2[n4  ];
              rec.y =  MRect.y + MNoteRects2[n4+1];
              rec.w = (MRect.x + MNoteRects2[n4+2]) - rec.x;
              rec.h = (MRect.y + MNoteRects2[n4+3]) - rec.y;
              if (rec.contains(AXpos,AYpos)) {
                if (MActiveMap[note]) deactivate(note);
                else activate(note);
                do_update(this);
                do_redraw(this,MRect/*,krs_opaque*/);
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

