unit z_window_linux;
{$include zenith.inc}

{$define Z_THREAD_TIMEOUT:=5000}
{$define Z_CAIROBUFFERDEPTH:=32}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  cthreads, // completely freezes desktop without this one!
  X, Xlib, //Xutil,
  z_gui_base,
  z_rect,
  z_cairo;
  //Cairo, CairoXlib;

type

  ZWindowLinux = class(ZWindowBase)
  protected
    FWinCairo   : ZCairo;
    FWinSurface : ZCairoSurface;
    FBackWidth  : LongInt;
    FBackHeight : LongInt;
  private
    FDisplay    : PDisplay;
    FParentWin  : TWindow;
    FWindow     : TWindow;
    FScreenNum  : longint;
    FVisual     : PVisual;
    FColormap   : TColormap;
    FBmpNodata  : TPixmap;
    FBlack      : TXColor;
    FDeleteAtom : TAtom;
    FCustomAtom : TAtom;
    {$ifdef Z_LIB}
    FEventThr   : TThreadID; // PtrUInt
    {$endif}
    FTimerThr   : TThreadID;
    FTimerRun   : boolean;
    FTimerSlp   : LongInt;
    FDepth      : longint;
    FWinCursor  : LongInt;
  //FInvCursor  : TCursor;
  //FSurface    : ZSurface;
  //FBackCairo  : ZCairo;
  //FBackSurface: ZCairoSurface;
  public
  //property    _backCairo  : ZCairo read FBackCairo;
    //property    _colormap   : TColormap read FColormap;
    //property    _depth      : longint   read FDepth;
    //property    _display    : PDisplay  read FDisplay;
    //property    _screenNum  : longint   read FscreenNum;
    //property    _visual     : PVisual   read FVisual;
    //property    _window     : TWindow   read FWindow;
  private
    procedure   linux_eventHandler(AEvent:PXEvent);
    function    linux_remapKey(AState:LongInt) : LongInt;
    function    linux_remapButton(AButton:LongInt) : LongInt;
    procedure   linux_removeDecoration;
    procedure   linux_wantQuitEvent;
  public
    constructor create(ARect:ZRect; AParent:Pointer=nil);
    destructor  destroy; override;
    //
    procedure   beginPaint; override;
    procedure   endPaint; override;
    procedure   eventLoop; override;
    procedure   flush; override;
    procedure   grabCursor; override;
    procedure   hide; override;
    procedure   hideCursor; override;
    procedure   invalidate(AXpos,AYpos,AWidth,AHeight:LongInt); override;
    procedure   lock; override;
    procedure   releaseCursor; override;
    procedure   reparent(AParent:Pointer); override;
    procedure   resetCursor; override;
    procedure   sendEvent(AValue:longword); override;
    procedure   setCursor(ACursor:LongInt); override;
    procedure   setCursorPos({%H-}AXpos,{%H-}AYpos:LongInt); override;
    procedure   setPos(AXpos,AYpos:LongInt); override;
    procedure   setSize(AWidth,AHeight:LongInt); override;
    procedure   setTitle({%H-}ATitle:PChar); override;
    procedure   show; override;
    procedure   showCursor; override;
    procedure   startTimer(ms:LongInt); override;
    procedure   stopTimer; override;
    procedure   sync; override;
    procedure   unlock; override;
  end;

  ZWindowImpl = ZWindowLinux;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  SysUtils,   // sleep
  z_const,
  z_util,
  z_cairo_linux;

type
  ZPropMotifWmHints = record
    flags       : longword;
    functions   : longword;
    decorations : longword;
    inputMode   : longint;
    status      : longword;
  end;

const
  z_blank_cursor : array[0..7] of byte = (0,0,0,0,0,0,0,0);

//----------------------------------------------------------------------
// thread proc
//----------------------------------------------------------------------

function z_linux_threadproc(data:pointer) : ptrint;
var
  win : ZWindowLinux;
  ev  : TXEvent;
  cev : PXClientMessageEvent;
  dta : LongWord;
begin
  win := ZWindowLinux(data);
  if Assigned(win) then
  begin
    while true do
    begin
      XNextEvent(win.FDisplay,@ev); // _display
      if (ev._type = ClientMessage) then
      begin
        cev := PXClientMessageEvent(@ev);
        dta := ev.xclient.data.l[0];
        if (cev^.message_type = win.FCustomAtom) then
        begin
          if (dta=zts_Kill) then EndThread(0)
          else win.linux_eventHandler(@ev);
        end;
      end //ClientMessage
      else win.linux_eventHandler(@ev);
    end;
  end;
  result := 0;
end;

//----------------------------------------------------------------------
// timer proc
//----------------------------------------------------------------------

function z_linux_timerproc(data:pointer) : ptrint;
var
  win : ZWindowLinux;
begin
  win := ZWindowLinux(data);
  if Assigned(win) then
  begin
    while win.FTimerRun do
    begin
      win.sendEvent(zts_timer);
      Sleep(win.FTimerSlp); // usleep(win->mTimerSleep*1000); //ms*1000;
    end;
  end;
  result := 0;
end;

//----------------------------------------------------------------------
// Window
//----------------------------------------------------------------------

constructor ZWindowLinux.create(ARect:ZRect; AParent:Pointer);
var
  eventmask : longword;
  swa       : TXSetWindowAttributes;
  swamask   : longword;
  //screen  : PScreen;
begin
  inherited create(ARect,zwa_none,nil); // (widget) rect,alignment,painter
  //FRect       := ARect;
  FDisplay    := XOpenDisplay(nil);

  // Hint: Conversion between ordinals and pointers is not portable
  if Assigned(AParent) then FParentWin := {%H-}TWindow(AParent)
  else FParentWin := XDefaultRootWindow(FDisplay);

  FScreenNum  := XDefaultScreen(FDisplay);
  FVisual     := XDefaultVisual(FDisplay,FScreenNum);
  FColormap   := XCreateColormap(FDisplay,FParentWin,FVisual,AllocNone);
  FDepth      := XDefaultDepth(FDisplay,FScreenNum);

  eventmask   := ExposureMask or StructureNotifyMask or PropertyChangeMask or ButtonPressMask or ButtonReleaseMask or PointerMotionMask or KeyPressMask or KeyReleaseMask or ClientMessage;
  swa.event_mask        := eventmask;
  swa.background_pixmap := None;
  swa.border_pixel      := 0;
  swa.colormap          := FColormap;
  swamask := CWEventMask or CWBackPixmap or CWBorderPixel or CWColormap;

  FWindow := XCreateWindow( FDisplay,
                            FParentWin,
                            FRect.x, FRect.y,
                            FRect.w, FRect.h,
                            0,
                            FDepth,
                            InputOutput,
                            FVisual,
                            swamask,
                            @swa );

  {$ifdef Z_LIB}
  linux_removeDecoration;
  {$else}
  linux_wantQuitEvent;
  {$endif}
  FBmpNodata    := XCreateBitmapFromData(FDisplay,FWindow,@z_blank_cursor,8,8);
  FWinCursor    := -1;
  FBlack.red    := 0;
  FBlack.green  := 0;
  FBlack.blue   := 0;
  FBlack.flags  := (DoRed or DoGreen or DoBlue);
  XAllocColor(FDisplay,XDefaultColormap(FDisplay,0),@FBlack);

  FTimerThr     := 0;
  FTimerRun     := false;
  FTimerSlp     := 30;

  FWinSurface   := ZCairoSurface( ZCairoSurfaceLinux.create(FDisplay,FWindow,FVisual,FRect.w,FRect.h) );
  FWinCairo     := ZCairo.create(FWinSurface);

  //TODO: Pixmap ??
  // off-screen, vram, so we can have hardware acceleration???

  //FBackSurface  := ZCairoSurface.create(FRect.w,FRect.h,Z_CAIROBUFFERDEPTH);
  //FBackSurface  := ZCairoSurface.create(FWinSurface,FRect.w,FRect.h);
  //FBackCairo    := ZCairo.create(FBackSurface);
  FBackWidth    := FRect.w;
  FBackHeight   := FRect.h;

  {$ifdef Z_LIB}
  FEventThr := BeginThread(@z_linux_threadproc,self);
  {$endif}


  writeln('(3-5) and 7 = ',(3-5) and 7);

end;

//----------

destructor  ZWindowLinux.destroy;
begin
  //ZTrace('ZWindowLinux.destroy');
  if (FTimerRun) then
  begin
    FTimerRun := false;
    WaitForThreadTerminate(FTimerThr,Z_THREAD_TIMEOUT);
  end;
  {$ifdef Z_LIB}
    sendEvent(zts_Kill);
    WaitForThreadTerminate(FEventThr,Z_THREAD_TIMEOUT);
  {$endif}

  FWinCairo.destroy;
  FWinSurface.destroy;

  //FBackCairo.destroy;
  //FBackSurface.destroy;

  XFreePixmap(FDisplay,FBmpNodata);
  if (FWinCursor>=0) then XFreeCursor(FDisplay,FWinCursor);
  XDestroyWindow(FDisplay,FWindow);
  //XFlush(FDisplay);
  XCloseDisplay(FDisplay);
  inherited;
end;

//----------------------------------------------------------------------
// eventHandler
//----------------------------------------------------------------------

procedure ZWindowLinux.linux_eventHandler(AEvent:PXEvent);
var
  x,y,b,s,w,h,i,k : longint;//integer;
  rec : ZRect;
  //atomname : PChar;
  bw,bh : longint;
  //srf : ZCairoSurface;
  //cai : ZCairo;
begin
  case (AEvent^._type) of

    ButtonPress:
      begin
        x := AEvent^.xbutton.x;
        y := AEvent^.xbutton.y;
        b := linux_remapButton(AEvent^.xbutton.button);
        s := linux_remapKey(AEvent^.xbutton.state);
        on_mouseDown(x,y,b,s);
      end;

    ButtonRelease:
      begin
        x := AEvent^.xbutton.x;
        y := AEvent^.xbutton.y;
        b := linux_remapButton(AEvent^.xbutton.button);
        s := linux_remapKey(AEvent^.xbutton.state);
        on_mouseUp(x,y,b,s);
      end;

    ClientMessage:
      begin
        i := AEvent^.xclient.data.l[0];
        if i = zts_Timer then on_timer;
      end;

    ConfigureNotify:
      begin
        w := AEvent^.xconfigure.width;
        h := AEvent^.xconfigure.height;
        while XCheckTypedWindowEvent(FDisplay, AEvent^.xconfigure.window, ConfigureNotify, AEvent) do
        begin
          w := AEvent^.xconfigure.width;
          h := AEvent^.xconfigure.height;
        end;
        if ((w<>FRect.w) or (h<>FRect.h)) then
        begin
          on_setSize(w,h);
          FWinSurface.setSize(w,h); // TODO
          //FBackSurface.setSize(w,h);
          //resizeBuffer(w,h);
          // only grow, never shrink...
          bw := FBackWidth;
          bh := FBackHeight;
          while w > FBackWidth do FBackWidth := FBackWidth * 2;
          while h > FBackHeight do FBackHeight := FBackHeight * 2;
          //while w < (FBackWidth div 4) do FBackWidth := FBackWidth div 2;
          //while h < (FBackHeight div 4) do FBackHeight := FBackHeight div 2;
          if (bw <> FBackWidth) or (bh <> FBackHeight) then
          begin
            //srf := ZCairoSurface.create(FBackWidth,FBackHeight,Z_CAIROBUFFERDEPTH);
            //srf := ZCairoSurface.create(FWinSurface,FBackWidth,FBackHeight);
            //cai := ZCairo.create(srf);
            //cai.setSourceSurface(FBackSurface.surface,0,0);
            //cai.setSourceColor(0,0,0,0);
            //cai.rectangle(0,0,bw,bh);
            //cai.fill;
            //FBackSurface.destroy;
            //FBackCairo.destroy;
            //FBackSurface := srf;
            //FBackCairo := cai;
          end;
          FRect.w := w;
          FRect.h := h;
          if ZHasFlag(FFlags,zwf_align) then
          begin
            on_align;
            //redrawAll;
          end;
          //redrawAll;
        end;
      end;

    Expose:
      begin
        rec.x := AEvent^.xexpose.x;
        rec.y := AEvent^.xexpose.y;
        rec.w := AEvent^.xexpose.width;
        rec.h := AEvent^.xexpose.height;
        while ( XCheckTypedWindowEvent(FDisplay, AEvent^.xexpose.window,Expose,AEvent)) do
        begin
          rec.combine(AEvent^.xexpose.x, AEvent^.xexpose.y, AEvent^.xexpose.width, AEvent^.xexpose.height );
        end;
        //ZTrace('Expose:   ',[rec.x,',',rec.y,',',rec.w,',',rec.h]);
        beginPaint;
        //surface := ZCairoSurface.create(FDisplay,FWindow,FVisual,(rec.x+rec.w),(rec.y+rec.h));
        //cairo := ZCairo.create(surface);
        if ZHasFlag(FFlags,zwf_clip) then
        begin
          FWinCairo.rectangle( rec.x, rec.y, rec.w, rec.h);
          FWinCairo.clip;
        end;

//----------
(*
        FWinCairo.setSourceColor(0.5,0,0);
        FWinCairo.rectangle(rec.x,rec.y,rec.w,rec.h);
        FWinCairo.fill;

        w := FRect.w div 3;
        h := FRect.h div 2;

        FWinCairo.setSourceColor(1,1,1);
        FWinCairo.moveTo( w*0,h*1 );
        FWinCairo.curveTo( w*1,h*0, w*2,h*2, w*3,h*1);
        FWinCairo.setLineWidth( (w+h) div 20);
        FWinCairo.stroke;
*)
//----------

        on_paint(FWinCairo,rec);
        if ZHasFlag(FFlags,zwf_clip) then FWinCairo.resetClip;

        (*
        //FBackCairo.resetClip;
        if (FFlags and zwf_clip) <> 0 then
        begin
          FBackCairo.rectangle( rec.x, rec.y, rec.w, rec.h);
          FBackCairo.clip;
        end;
        on_paint(FBackCairo,rec);
        if (FFlags and zwf_clip) <> 0 then FBackCairo.resetClip;
        *)

        {
          Note that the paint operation will copy the entire surface. If you'd
          like to instead copy some (width, height) rectangle from (source_x,
          source_y) to some point (dest_x, dest_y) on the destination you would
          instead compute a new position for the source surface origin and then
          use cairo_fill instead of cairo_paint:
            cairo_set_source_surface (cr, source, dest_x - source_x, dest_y - source_y)
            cairo_rectangle (cr, dest_x, dest_y, width, height);
            cairo_fill (cr);
        }
        //cairo.destroy;
        //surface.destroy;
        endPaint;
      end;

    KeyPress:
      begin
        k := AEvent^.xkey.keycode;
        s := AEvent^.xkey.state;
        on_keyDown(k,s);
      end;

    KeyRelease:
      begin
        k := AEvent^.xkey.keycode;
        s := AEvent^.xkey.state;
        on_keyUp(k,s);
      end;

    MotionNotify:
      begin
        x := AEvent^.xbutton.x;
        y := AEvent^.xbutton.y;
        s := AEvent^.xbutton.state;
        on_mouseMove(x,y,s);
      end;

    //PropertyNotify:
    //  begin
    //    atomname := XGetAtomName(FDisplay,AEvent^.xproperty.atom);
    //    writeln('PropertyNotify: ',atomname);
    //  end;

    //else ZTrace('unhandled event: ',[AEvent^._type]);

  end; // case
end;

//----------------------------------------------------------------------
// internal
//----------------------------------------------------------------------

// want WM_DELETE_WINDOW ClientMessage
procedure ZWindowLinux.linux_wantQuitEvent;
begin
  FDeleteAtom := XInternAtom(FDisplay,'WM_DELETE_WINDOW',True);
  XSetWMProtocols(FDisplay,FWindow,@FDeleteAtom,1);
end;

//----------

function ZWindowLinux.linux_remapButton(AButton:LongInt) : LongInt;
begin
  result := zbu_none;
  case AButton of
    1: result := zbu_left;
    2: result := zbu_middle;
    3: result := zbu_right;
  end;
end;

//----------

function ZWindowLinux.linux_remapKey(AState:LongInt) : LongInt;
begin
  result := zbu_none;
  if (AState and 1) <> 0 then result := result or zbu_shift;
              // 2 = capslock
  if (AState and 4) <> 0 then result := result or zbu_ctrl;
  if (AState and 8) <> 0 then result := result or zbu_alt;
              // 128 = alt gr
end;

//----------

{$define MWM_HINTS_DECORATIONS:=(1 shl 1)}
{$define PROP_MOTIF_WM_HINTS_ELEMENTS:=5}

// remove title-bar, borders
procedure ZWindowLinux.linux_removeDecoration;
var
  hints : ZPropMotifWmHints;
  prop  : TAtom;
begin
  hints.flags := MWM_HINTS_DECORATIONS;
  hints.decorations := 0;
  prop := XInternAtom(FDisplay,'_MOTIF_WM_HINTS',true);
  XChangeProperty(FDisplay,FWindow,prop,prop,32,PropModeReplace,@hints,PROP_MOTIF_WM_HINTS_ELEMENTS);
end;

{$undef MWM_HINTS_DECORATIONS}
{$undef PROP_MOTIF_WM_HINTS_ELEMENTS}

//----------------------------------------------------------------------
// window
//----------------------------------------------------------------------

procedure ZWindowLinux.beginPaint;
begin
end;

//----------

procedure ZWindowLinux.endPaint;
begin
  XFlush(FDisplay);
end;

//----------

procedure ZWindowLinux.eventLoop;
var
  event : TXEvent;
  data  : longword;
begin
  while (true) do
  begin
    XNextEvent(FDisplay,@event);
    data := event.xclient.data.l[0];
    if (event._type = ClientMessage) and (data = FDeleteAtom) then break
    else linux_eventHandler(@event);
  end; // while
end;

//----------

procedure ZWindowLinux.flush;
begin
  XFlush(FDisplay);
end;

//----------

procedure ZWindowLinux.grabCursor;
var
  which : LongInt;
begin
  which := ButtonPressMask or ButtonReleaseMask or PointerMotionMask;
  XGrabPointer(FDisplay,FWindow,false,which,GrabModeSync,GrabModeAsync,FWindow,{cursor}None,CurrentTime);
end;

//----------

procedure ZWindowLinux.hide;
begin
  XUnmapWindow(FDisplay,FWindow);
  XFlush(FDisplay);
end;

//----------

procedure ZWindowLinux.hideCursor;
begin
  if (FWinCursor>=0) then resetCursor;
  FWinCursor := XCreatePixmapCursor(FDisplay,FBmpNodata,FBmpNodata,@FBlack,@FBlack,0,0);
  XDefineCursor(FDisplay,FWindow,FWinCursor);
end;

//----------

procedure ZWindowLinux.invalidate(AXpos,AYpos,AWidth,AHeight:longint);
var
  ev : TXExposeEvent; // static
begin
  ev._type    := Expose;
  ev.display  := FDisplay;
  ev.window   := FWindow; // mParent;
  ev.x        := AXpos;
  ev.y        := AYpos;
  ev.width    := AWidth;
  ev.height   := AHeight;
  ev.count    := 0;
  XSendEvent(FDisplay,FWindow,false,ExposureMask,PXEvent(@ev));
  XFlush(FDisplay);
end;

//----------

procedure ZWindowLinux.lock;
begin
  XLockDisplay(FDisplay);
end;

//----------

procedure ZWindowLinux.releaseCursor;
begin
  XUngrabPointer(FDisplay,CurrentTime);
end;

//----------

procedure ZWindowLinux.reparent(AParent:Pointer); // 64-bit !!!
begin
  // Hint: Conversion between ordinals and pointers is not portable
  FParentWin := {%H-}TWindow(AParent);
  XReparentWindow(FDisplay,FWindow,FParentWin,0,0);
  XFlush(FDisplay);
end;

//----------

procedure ZWindowLinux.resetCursor;
begin
  XUndefineCursor(FDisplay,FWindow);
  XFreeCursor(FDisplay,FWinCursor);
  FWinCursor := -1;
end;

//----------

procedure ZWindowLinux.sendEvent(AValue:longword);
var
  event : TXClientMessageEvent; // static ?
  //MEventAtom  : TAtom;
begin
  // Hint: Local variable "event" does not seem to be initialized
  //FillByte(event,SizeOf(event),0);
  event._type         := ClientMessage;
  event.message_type  := FCustomAtom;
  event.display       := FDisplay;
  event.window        := FWindow;
  event.format        := 32;
  event.data.l[0]     := AValue;
  XSendEvent(FDisplay,FWindow,False,NoEventMask,PXEvent(@event));
  XFlush(FDisplay);
end;

//----------

procedure ZWindowLinux.setCursor(ACursor:LongInt);
begin
  //if( aCursor<0 ) aCursor = DEF_CURSOR;
  if (FWinCursor>=0) then resetCursor;
  //mWinCursor = XCreatePixmapCursor( gDP,bitmapNoData,bitmapNoData,&mBlack,&mBlack,0,0 );
  if (ACursor>=0) then
  begin
    FWinCursor := XCreateFontCursor(FDisplay, ACursor);
    XDefineCursor(FDisplay, FWindow, FWinCursor);
  end;
end;

//----------

procedure ZWindowLinux.setCursorPos(AXpos,AYpos:LongInt);
begin
end;

//----------

procedure ZWindowLinux.setPos(AXpos,AYpos:LongInt);
var
  attr : TXWindowChanges;
begin
  attr.x := AXpos;
  attr.y := AYpos;
  XConfigureWindow(FDisplay,FWindow,CWX or CWY,@attr);
  XFlush(FDisplay);
end;

//----------

procedure ZWindowLinux.setSize(AWidth,AHeight:LongInt);
begin
  XResizeWindow(FDisplay,FWindow,AWidth,AHeight);
  XFlush(FDisplay);
end;

//----------

procedure ZWindowLinux.setTitle(ATitle:PChar);
begin
end;

//----------

procedure ZWindowLinux.show;
begin
  XMapWindow(FDisplay,FWindow);
  XFlush(FDisplay);
end;

//----------

procedure ZWindowLinux.showCursor;
begin
  //setCursor(DEF_CURSOR);
end;

//----------

procedure ZWindowLinux.startTimer(ms:LongInt);
begin
  FTimerSlp := ms;
  FTimerRun := true;
  FTimerThr := BeginThread(@z_linux_timerproc,self);
end;

//----------

procedure ZWindowLinux.stopTimer;
begin
  FTimerRun := false;
  WaitForThreadTerminate(FTimerThr,Z_THREAD_TIMEOUT);
end;

//----------

procedure ZWindowLinux.sync;
begin
  XSync(FDisplay,false);
end;

//----------

procedure ZWindowLinux.unlock;
begin
  XUnlockDisplay(FDisplay);
end;

//----------------------------------------------------------------------
end.

