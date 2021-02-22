#ifndef s3_widget_timeline_included
#define s3_widget_timeline_included
//----------------------------------------------------------------------

#include "audio/s3_timeline.h"
#include "gui/s3_widget.h"
#include "audio/s3_timeline.h"

//----------

#define TRACK_HEIGHT      20
#define TRACK_WIDTH       100
#define PIXELS_PER_SECOND 10
#define SECONDS_PER_PIXEL (1/PIXELS_PER_SECOND)
#define DRAG_HANDLE_SIZE  5

//----------

class S3_Widget_Timeline
: public S3_Widget {

  private:
    float                 FZoom;
    float                 FStartPos;
    int32                 FPrevMouseX;
    int32                 FPrevMouseY;
  private:
    S3_Timeline_Track*    FHoverTrack;
    S3_Timeline_Segment*  FHoverSegment;
    float                 FHoverTime;
    bool                  FHoverLeft;
    bool                  FHoverRight;
  private:
    S3_Timeline_Segment*  FDraggingSegment;
    bool                  FDraggingLeft;
    bool                  FDraggingRight;
  protected:
    S3_Timeline*          FTimeline;
    S3_Color              FBackColor;
    S3_Color              FTrackBackColor;
    S3_Color              FTrackTextColor;
    S3_Color              FSegmentBackColor;
    S3_Color              FSegmentTextColor;
    S3_Color              FBorderColor;
    S3_Color              FTextColor;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Widget_Timeline(S3_Rect ARect, S3_Timeline* ATimeline, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName             = "S3_Widget_Timeline";
      FTimeline         = ATimeline;
      FStartPos         = 0;
      FBackColor        = S3_Grey;
      FTrackBackColor   = S3_DarkGrey;
      FTrackTextColor   = S3_LightGrey;
      FSegmentBackColor = S3_LightGrey;
      FSegmentTextColor = S3_DarkGrey;
      FBorderColor      = S3_Black;
      FZoom             = 1;
      FStartPos         = 0;
      FPrevMouseX       = 0;
      FPrevMouseY       = 0;
      //
      FHoverTrack       = S3_NULL;
      FHoverSegment     = S3_NULL;
      FHoverTime        = -1;
      FHoverLeft        = false;
      FHoverRight       = false;
      //
      FDraggingSegment  = S3_NULL;
      FDraggingLeft     = false;
      FDraggingRight    = false;
    }

    //----------

    virtual ~S3_Widget_Timeline() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void  zoom(float AZoom)     { FZoom = AZoom; }
    float zoom(void)            { return FZoom; }
    void  startpos(float APos)  { FStartPos = APos; }
    float startpos(void)        { return FStartPos; }

  //------------------------------
  //
  //------------------------------

  private:

    float calcLength(int32 APixels) {
      float time = (float)( APixels * SECONDS_PER_PIXEL );
      return time / FZoom;
    }

    //----------

    int32 calcPixels(int32 ATime) {
      float pixel = ATime * PIXELS_PER_SECOND;
      return S3_Trunc( pixel*FZoom );
    }

    //----------

    float calcTime(int32 AXpos) {
      if (FZoom <= 0) return -1;
      float time = (float)( AXpos - TRACK_WIDTH );
      if (time < 0) return -1;
      time *= SECONDS_PER_PIXEL;
      return  (FStartPos + time) / FZoom;
    }

    //----------

    float calcTimeDiff(int32 AXpos, int32 APrev) {
      if (FZoom <= 0) return 0;
      float diff = (float)( AXpos - APrev );
      //if time < 0 then exit(0);
      diff *= SECONDS_PER_PIXEL;
      return diff / FZoom;
    }

    //----------

    int32 calcXpos(float ATime) {
      if (ATime >= 0) {
        float pixel = ((ATime*FZoom) * PIXELS_PER_SECOND);
        return  TRACK_WIDTH + S3_Trunc(pixel);
      }
      return -1;
    }

    //----------

    S3_Timeline_Track* findTrack(int32 AYpos) {
      int32 y = AYpos - MRect.y;
      if (y < 0) return S3_NULL;
      int32 t = y / TRACK_HEIGHT;
      //result = nil;
      S3_ListNode* node = FTimeline->tracks()->head();
      while (node) {
        if (t == 0) return (S3_Timeline_Track*)node;
        t -= 1;
        node = node->next();
      }
      return S3_NULL;
    }

    //----------

    S3_Timeline_Segment* findSegment(S3_Timeline_Track* ATrack, float ATime) {
      //result := nil;
      S3_ListNode* node = ATrack->segments()->head();
      while (node) {
        S3_Timeline_Segment* segment = (S3_Timeline_Segment*)node;
        if ( (ATime >= segment->start()) && (ATime <= segment->end()) ) return segment;
        node = node->next();
      }
      return S3_NULL;
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      bool changed = false;
      if (AButton == s3_mb_left) {
        FPrevMouseX = AXpos;
        FPrevMouseY = AYpos;
        if (!FHoverSegment) return;
        FDraggingSegment = FHoverSegment;
        FDraggingLeft = FHoverLeft;
        FDraggingRight = FHoverRight;
      }
      if (changed) do_widgetRedraw(this,MRect,s3_pm_normal);
      //S3_Widget::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton,AState);
    }

    //----------

    virtual
    void on_widgetMouseUp(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      bool changed = false;
      if (AButton == s3_mb_left) {
        //if (!FDraggingSegment) return;
        FDraggingSegment = S3_NULL;
        FDraggingLeft = false;
        FDraggingRight = false;
      }
      if (changed) do_widgetRedraw(this,MRect,s3_pm_normal);
      //S3_Widget::on_widgetMouseUp(AWidget,AXpos,AYpos,AButton,AState);
    }

    //----------

    virtual
    void on_widgetMouseMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AState) {
      bool changed = false;
      S3_Timeline_Track* track = findTrack(AYpos);
      S3_Timeline_Segment* segment = S3_NULL;
      FHoverTime = calcTime(AXpos);
      if (FDraggingSegment) {
        float mintime = calcLength(10); // minimum 10 pixels
        float diff = calcTimeDiff(AXpos,FPrevMouseX);
        float st = FDraggingSegment->start();
        float en = FDraggingSegment->end();
        if (FDraggingLeft) {
          st += diff;
          st = S3_Max(st,0);
          if (st > (en-mintime)) st = (en-mintime);
        }
        else if (FDraggingRight) {
          en += diff;
          if (en <  (st+mintime)) en = (st+mintime);
        }
        else {
          // dragging segment
          float le = en - st;
          st += diff;
          st = S3_Max(st,0);
          en = st + le;
        }
        S3_Timeline_Segment* prv = (S3_Timeline_Segment*)FDraggingSegment->prev();
        if (prv) {
          if (st <= prv->end()) {
            float le = en - st;
            st = prv->end(); //+ 0.0001;
            if (!FDraggingLeft) en = st + le;
          }
        }
        S3_Timeline_Segment* nxt = (S3_Timeline_Segment*)FDraggingSegment->next();
        if (nxt) {
          if (en >= nxt->start()) {
            float le = en - st;
            en = nxt->start(); //- 0.0001;
            if (!FDraggingRight) st = en - le;
          }
        }
        FDraggingSegment->start(st);
        FDraggingSegment->end(en);
        changed = true;
      } // dragging

      else { // not dragging
        bool hoverleft = false;
        bool hoverright = false;
        if (track) {
          segment = findSegment(track,FHoverTime);
          if (segment) {
            int32 leftx  = calcXpos(segment->start());
            int32 rightx = calcXpos(segment->end());
            if (AXpos < (leftx+DRAG_HANDLE_SIZE)) hoverleft = true;
            if (AXpos >= (rightx-DRAG_HANDLE_SIZE)) hoverright = true;
          }
        }
        if (track != FHoverTrack) {
          FHoverTrack = track;
          changed = true;
        }
        if (segment != FHoverSegment) {
          FHoverSegment = segment;
          changed = true;
        }
        if (hoverleft != FHoverLeft) {
          FHoverLeft = hoverleft;
          changed = true;
        }
        if (hoverright != FHoverRight) {
          FHoverRight = hoverright;
          changed = true;
        }
      } // not dragging
      if (changed) do_widgetRedraw(this,MRect,s3_pm_normal);
      FPrevMouseX = AXpos;
      FPrevMouseY = AYpos;
      //S3_Widget::on_widgetMouseMove(AWidget,AXpos,AYpos,AState);
    }

    //----------

    virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode) {
      APainter->setFillColor(FBackColor);
      APainter->fillRectangle(MRect.x,MRect.y,MRect.x2(),MRect.y2());
      APainter->setDrawColor(FBorderColor);
      APainter->drawLine(MRect.x,MRect.y,MRect.x2(),MRect.y);
      int32 x1 = MRect.x;
      //int32 x2 := FRect.x2;
      int32 y1 = MRect.y;
      S3_ListNode* tnode = FTimeline->tracks()->head();
      while (tnode) {
        S3_Timeline_Track* track = (S3_Timeline_Track*)tnode;
        int32 x2 = x1 + TRACK_WIDTH - 1;
        int32 y2 = y1 + TRACK_HEIGHT - 1;
        // { track background }
        if (track == FHoverTrack) APainter->setFillColor( S3_Color(0.3,0.3,0.3) );
        else APainter->setFillColor(FTrackBackColor);
        APainter->fillRectangle(x1,y1,x2,y2);
        // { track name }
        APainter->setTextColor(FTrackTextColor);
        APainter->drawText(x1+2,y1,x2-2,y2,track->name(), s3_ta_center);
        // { track border }
        APainter->setDrawColor(FBorderColor);   // below
        APainter->drawLine(x1,y2,MRect.x2(),y2);  // below
        APainter->drawLine(x2,y1,x2,y2);        // right
        // { track segments }
        S3_ListNode* snode = track->segments()->head();
        while (snode) {
          S3_Timeline_Segment* segment = (S3_Timeline_Segment*)snode;
          float ss = segment->start() * FZoom * PIXELS_PER_SECOND;
          float se = segment->end() * FZoom * PIXELS_PER_SECOND;
          // { back }
          if (segment == FHoverSegment) APainter->setFillColor( S3_Color(0.7,0.7,0.7) );
          else APainter->setFillColor(FSegmentBackColor);
          APainter->fillRectangle(x2+1+trunc(ss),y1,x2+1+trunc(se),y2-1);
          // { name }
          APainter->setTextColor(FSegmentTextColor);
          APainter->drawText(x2+1+S3_Trunc(ss),y1,x2+1+S3_Trunc(se),y2,segment->name(),s3_ta_center);
          // { border }
          APainter->setDrawColor(FBorderColor);
          //APainter->drawRect(x2+1+trunc(ss),y1,x2+1+trunc(se),y2);
          APainter->drawLine(x2+1+S3_Trunc(ss),y1,x2+1+S3_Trunc(ss),y2);
          APainter->drawLine(x2+1+S3_Trunc(se),y1,x2+1+S3_Trunc(se),y2);
          // { resize indicators }
          if (segment == FHoverSegment) {
            APainter->setFillColor( S3_DarkGrey );
            if (FHoverLeft)   APainter->fillRectangle(x2+1+S3_Trunc(ss),y1,x2+1+S3_Trunc(ss)+(DRAG_HANDLE_SIZE-1),y2);
            if (FHoverRight)  APainter->fillRectangle(x2+1+S3_Trunc(se)-(DRAG_HANDLE_SIZE-1),y1,x2+1+S3_Trunc(se),y2);
          }
          snode = snode->next();
        }
        y1 += TRACK_HEIGHT;
        tnode = tnode->next();
      }
      float cur = FTimeline->cursor() * FZoom * PIXELS_PER_SECOND;
      APainter->setDrawColor( S3_LightRed );
      APainter->drawLine( MRect.x + S3_Trunc(cur), MRect.y, MRect.x + S3_Trunc(cur), MRect.y2() );
      //S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

    virtual
    void on_widgetLeave(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
      bool changed = false;
      if (FHoverTrack) {
        FHoverTrack = S3_NULL;
        changed = true;
      }
      if (FHoverSegment) {
        FHoverSegment = S3_NULL;
        changed = true;
      }
      if (changed) do_widgetRedraw(this,MRect,s3_pm_normal);
      //S3_Widget::on_widgetLeave(AWidget, int32 AXpos, int32 AYpos);
    }

  //----------

};

//----------

#undef TRACK_HEIGHT
#undef TRACK_WIDTH
#undef PIXELS_PER_SECOND
#undef SECONDS_PER_PIXEL
#undef DRAG_HANDLE_SIZE

//----------------------------------------------------------------------
#endif
