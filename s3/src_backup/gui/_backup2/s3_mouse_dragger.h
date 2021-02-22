#ifndef s3_mouse_dragger_included
#define s3_mouse_dragger_included
//----------------------------------------------------------------------

#if 0

#include "s3_window.h"

//----------

class S3_MouseDragger {

  private:

    S3_Window*  MWindow;
    bool        MDragging;
    int32       MStartX;
    int32       MStartY;
    int32       MPrevX;
    int32       MPrevY;
    bool        MHideCursor;

  public:

    S3_MouseDragger() {
      MWindow = S3_NULL;
      MDragging = false;
      MStartX = 0;
      MStartY = 0;
      MPrevX = 0;
      MPrevY = 0;
      MHideCursor = true;
    }

    //S3_MouseDragger(S3_Window* AWindow) {
    //  S3_MouseDragger();
    //  MWindow = AWindow;
    //}

    virtual ~S3_MouseDragger() {
    }

  public:

    void  window(S3_Window* AWindow)  { MWindow=AWindow; }
    bool  dragging(void)              { return MDragging; }
    void  hideCursor(bool AHide=true) { MHideCursor = AHide; }

  public:

    void startDrag(int32 AXpos, int32 AYpos) {
      if (!MDragging) {
        MStartX = AXpos;
        MStartY = AYpos;
        MPrevX = AXpos;
        MPrevY = AYpos;
        MDragging = true;
        //if (MWindow) {
        //  if (MHideCursor) MWindow->hideCursor();
        //}
        do_widgetCursor(this,s3_mc_hide);
      }
    }

    void endDrag(int32 AXpos, int32 AYpos) {
      if (MDragging) {
        MDragging = false;
        //if (MWindow) {
        //  MWindow->setCursorPos(MStartX,MStartY);
        //  if (MHideCursor) MWindow->showCursor();
        //}
        do_widgetCursorPos(this,MStartX,MStartY);
        do_widgetCursor(this,s3_mc_show);
      }
    }

    // call this every time the mouse moves, even if not dragging..

    /*
      every time the mouse moves, we first find out how much, and then set the
      mouse cursor back to where we clicked..
      when we move the cursor with the win32 api (setcursorpos), we get a
      WM_MOUSEMOVE event, and we can't differentiate between this mouse event,
      and a 'real' one coming from us moving the mouse.. so, from the code, it
      looks like we wiggle the mouse back and forth..
      so, we ignore the event if the x and y coordinate is the same as where we
      clicked..
      also, if the delta movement (difference from previous call) is 0,
      we do nothing..
    */

    bool drag(int32 AXpos, int32 AYpos, int32* ADeltaX, int32* ADeltaY) {
      int32 prevx = MPrevX;
      int32 prevy = MPrevY;
      MPrevX = AXpos;
      MPrevY = AYpos;
      if (!MDragging) return false;
      else {
        if ((AXpos == MStartX) && (AYpos == MStartY)) return false;
        int32 diffx = (AXpos-prevx);
        int32 diffy = (AYpos-prevy);
        if ((diffx == 0) && (diffy == 0)) return false;
        //S3_Trace("drag %i %i\n",diffx,diffy);
        *ADeltaX = diffx;
        *ADeltaY = diffy;
        if (MWindow) MWindow->setCursorPos(MStartX,MStartY);
        return true;
      }
    }

};

#endif // 0

//----------------------------------------------------------------------
#endif
