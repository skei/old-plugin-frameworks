#ifndef s3_widget_waveform_included
#define s3_widget_waveform_included
//----------------------------------------------------------------------

#include "common/s3_color.h"
#include "gui/s3_widget.h"

struct S3_WaveformMarker {
  int32     pos;
  S3_Color  color;
};

struct S3_WaveformArea {
  int32     pos;
  int32     size;
  S3_Color  color;
};

//typedef S3_Array<S3_WaveformMarker> S3_WaveformMarkers;
//typedef S3_Array<S3_WaveformArea> S3_WaveformAreas;

//----------

class S3_Widget_Waveform
: public S3_Widget {

  private:
    float*              MBuffer;
    int32               MBufferSize;
  protected:
    S3_Color            MBackColor;
    S3_Color            MWaveColor;
    S3_Color            MGridColor;
    bool                MDrawBackground;
    int32               MNumMarkers;
    int32               MNumAreas;
    S3_WaveformMarker*  MMarkers;
    S3_WaveformArea*    MAreas;
    int32               MNumGrid;
    bool                MMono;

  public:

    S3_Widget_Waveform(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName           = "S3_Widget_Waveform";
      MHint           = "waveform";
      MBuffer         = S3_NULL;
      MBufferSize     = 0;
      MBackColor      = S3_Black;      // S3_Grey;
      MWaveColor      = S3_LightGrey;
      MGridColor      = S3_Grey;
      MDrawBackground = true;
      MNumMarkers     = 0;
      MNumAreas       = 0;
      MMarkers        = S3_NULL;
      MAreas          = S3_NULL;
      MNumGrid        = 0;
      MMono           = false;
    }

    virtual ~S3_Widget_Waveform() {
      if (MMarkers) S3_Free(MMarkers);
      if (MAreas) S3_Free(MAreas);
    }

    //----------

    void setBuffer(float* ABuffer) {
      MBuffer = ABuffer;
    }

    void setBufferSize(int32 ASize) {
      MBufferSize = ASize;
    }

    void setBackColor(S3_Color AColor) {
      MBackColor = AColor;
    }

    void setWaveColor(S3_Color AColor) {
      MWaveColor = AColor;
    }

    void drawBackground(bool ADraw=true) {
      MDrawBackground = ADraw;
    }

    void setNumMarkers(int32 ANum) {
      MNumMarkers = ANum;
      //SetLength(MMarkers,ANum);
      if (MMarkers) S3_Free(MMarkers);
      MMarkers = (S3_WaveformMarker*)S3_Malloc(ANum * sizeof(S3_WaveformMarker));
    }

    void setNumAreas(int32 ANum) {
      MNumAreas = ANum;
      //SetLength(MAreas,ANum);
      if (MAreas) S3_Free(MAreas);
      MAreas = (S3_WaveformArea*)S3_Malloc(ANum * sizeof(S3_WaveformArea));
    }

    void setMarker(int32 AIndex, int32 APos, S3_Color AColor) {
      MMarkers[AIndex].pos = APos;
      MMarkers[AIndex].color = AColor;
    }

    void setMarkerPos(int32 AIndex, int32 APos) {
      MMarkers[AIndex].pos = APos;
    }

    void setMarkerColor(int32 AIndex, S3_Color AColor) {
      MMarkers[AIndex].color = AColor;
    }

    void setArea(int32 AIndex, int32 APos, int32 ASize, S3_Color AColor) {
      MAreas[AIndex].pos = APos;
      MAreas[AIndex].size = ASize;
      MAreas[AIndex].color = AColor;
    }

    void setAreaPos(int32 AIndex, int32 APos) {
      MAreas[AIndex].pos = APos;
    }

    void setAreaPosSize(int32 AIndex, int32 APos, int32 ASize) {
      MAreas[AIndex].pos = APos;
      MAreas[AIndex].size = ASize;
    }

    void setAreaSize(int32 AIndex, int32 ASize) {
      MAreas[AIndex].size = ASize;
    }

    void setAreaColor(int32 AIndex, S3_Color AColor) {
      MAreas[AIndex].color = AColor;
    }

    void setNumGrid(int32 ANum) {
      MNumGrid = ANum;
    }

    void setMono(bool AMono=true) {
      MMono = AMono;
    }

  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {

      //S3_Assert(MBufferSize>0);

      if (MRect.w > 0 ) {

        float h2 = (float)MRect.h * 0.5;
        int32 midy = MRect.y + (MRect.h >> 1);

        /* background */

        if (MDrawBackground) {
          APainter->setFillColor(MBackColor);
          APainter->fillRectangle(MRect);
        }

        if (MBufferSize > 0) {

          /* areas */

          for (int32 i=0; i<MNumAreas; i++) {
            if (MAreas[i].size > 0) {
              float x = (float)MAreas[i].pos / (float)MBufferSize;
              float w = (float)MAreas[i].size / (float)MBufferSize;
              int32 ix = MRect.x + ( S3_Trunc( x * MRect.w ) );
              int32 iw = ix + S3_Trunc( w * MRect.w );
              APainter->setFillColor(MAreas[i].color);
              APainter->fillRectangle(ix,MRect.y,iw,MRect.y2());
            }
          }

          /* waveform */

          float xadd = (float)MBufferSize / (float)MRect.w;
          if (MBuffer) {
            APainter->setDrawColor(MWaveColor);
            float x = 0;
            //APainter->drawLine( MRect.x, midy, MRect.x2(), midy );
            for (int32 i=0; i<MRect.w; i++) {
              int32 index = (int)x;
              float s;// = 0;
              if (MMono) s = MBuffer[index];
              else s = ( MBuffer[ index*2 ] + MBuffer[ index*2 + 1 ] ) * 0.5;
              s *=  h2;
              int32 ix = MRect.x + i;
              int32 iy = (int)s;
              APainter->drawLine( ix, midy, ix, midy-iy );
              x += xadd;
            }
          }

          /* markers */

          for (int32 i=0; i<MNumMarkers; i++) {
            float x = MMarkers[i].pos / (float)MBufferSize;
            //S3_Trace("x %f\n",x);
            if ((x>=0.0f) && (x<=1.0f)) {
              int32 ix = MRect.x + ( (int)( x * MRect.w ) );
              APainter->setDrawColor(MMarkers[i].color);
              APainter->drawLine( ix, MRect.y, ix, MRect.y2() );
            }
          }

        } // MBufferSize > 0

        /* grid */

        if (MNumGrid > 1) {
          float xadd = (float)MRect.w / (float)MNumGrid;
          float x = MRect.x + xadd;
          APainter->setDrawColor(MGridColor);
          for (int32 i=1; i<MNumGrid; i++) {
            int32 ix = S3_Trunc(x);
            APainter->drawLine( ix, MRect.y, ix, MRect.y2() );
            x += xadd;
          }
        }

      } // w>0

      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);

    }

};

//----------------------------------------------------------------------
#endif
