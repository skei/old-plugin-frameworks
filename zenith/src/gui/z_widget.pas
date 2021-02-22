unit z_widget;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_const,
  //z_object,
  z_rect,
  z_array,
  //z_queue,
  z_parameter,
  z_cairo;

type

  //ZWidget = class
  //public
  //  procedure on_paint(ACairo:ZCairo; ARect:ZRect); virtual;
  //end;

  ZWidgetLayout = record
    FAlignment  : LongWord;
    FMarginX   : LongInt;
    FMarginY   : LongInt;
    FPaddingX   : LongInt;
    FPaddingY   : LongInt;
  end;

  ZWidget = class;
  ZWidgetArray = specialize ZArray<ZWidget>;
  //ZWidgetQueue = specialize ZQueue<ZWidget>;

  {ZWidgetEvent = record
    wdg : ZWidget;
    val : Single;
  end;
  ZWidgetEventQueue = specialize ZQueue<ZWidgetEvent>;}

  //----------

  ZWidgetUpdate = record
    FWidget : ZWidget;
    FValue  : Single;
  end;

  //ZWidgetUpdates = specialize ZQueueMPMC<ZWidgetUpdate>;

  //----------

  ZWidgetPainter = class
  public
  //constructor create;
  //destructor  destroy; override;
    procedure   on_paint({%H-}AWidget:ZWidget; {%H-}ACairo:ZCairo; {%H-}ARect:ZRect); virtual;
  end;

  //----------

  ZWidget = class // (ZObject)
  //private
  protected
    FCapturedWidget : ZWidget;
  //FClientRect     : ZRect;
    FContentRect    : ZRect;
    FHoverWidget    : ZWidget;
    FInitialRect    : ZRect;
    FParentWidget   : ZWidget;
    FSubWidgets     : ZWidgetArray;
  protected
    FAlignment      : LongWord;
  //FCtrlIndex      : LongInt;
    FFlags          : LongWord;//ZWidgetFlags;
    FIndex          : LongInt;
    FLayout         : ZWidgetLayout;
    FPainter        : ZWidgetPainter;
    FParameter      : ZParameter;
    FRect           : ZRect;
    FText           : PChar;
    FValue          : Single;
  //FWdgIndex       : LongInt;
  public
    property    flags : LongWord{ZWidgetFlags} read FFlags write FFlags;
  public
    constructor create(ARect:ZRect; AAlignment:LongWord=zwa_none; APainter:ZWidgetPainter=nil);
    destructor  destroy; override;
  public
    procedure   appendWidget(AWidget:ZWidget); virtual;
  //procedure   clearFlag(AFlag:LongWord); virtual;
    procedure   deleteWidgets; virtual;
    function    findSubWidget(AXpos,AYpos:LongInt) : ZWidget; virtual;
    function    getFlags : LongWord{ZWidgetFlags}; virtual;
    function    getIndex : LongInt; virtual;
    function    getParent : ZWidget; virtual;
    function    getRect : ZRect; virtual;
    function    getParameter : ZParameter; virtual;
    function    getPainter : ZWidgetPainter; virtual;
    function    getText : PChar; virtual;
    function    getValue : Single; virtual;
    procedure   setAlignment(AAlignment:LongWord); virtual;
  //procedure   setFlag(AFlag:LongWord); virtual;
    procedure   setFlags(AFlags:longword{ZWidgetFlags}); virtual;
    procedure   setIndex(AIndex:LongInt); virtual;
    procedure   setMargin(x,y:longint); virtual;
    procedure   setPadding(x,y:longint); virtual;
    procedure   setPainter(APainter:ZWidgetPainter); virtual;
    procedure   setParameter(AParameter:ZParameter); virtual;
    procedure   setText(AText:PChar); virtual;
    procedure   setValue(AValue:Single); virtual;
  public
    procedure   on_align; virtual;
    procedure   on_enter({%H-}AWidget:ZWidget); virtual;
    procedure   on_keyDown(AKey:LongInt; AState:LongWord); virtual;
    procedure   on_keyUp(AKey:LongInt; AState:LongWord); virtual;
    procedure   on_leave({%H-}AWidget:ZWidget); virtual;
    procedure   on_mouseDown(AXpos,AYpos:LongInt; AButton,AState:LongWord); virtual;
    procedure   on_mouseMove(AXpos,AYpos:LongInt; AState:LongWord); virtual;
    procedure   on_mouseUp(AXpos,AYpos:LongInt; AButton,AState:LongWord); virtual;
    procedure   on_paint(ACairo:ZCairo; ARect:ZRect); virtual;
    procedure   on_setPos(AXpos,AYpos:LongInt); virtual;
    procedure   on_setSize(AWidth,AHeight:LongInt); virtual;
    procedure   on_timer; virtual;
  public
    procedure   do_cursor(AWidget:ZWidget; ACursor:LongInt); virtual;
    procedure   do_hint(AWidget:ZWidget; AHint:PChar); virtual;
    procedure   do_modal(AWidget:ZWidget; AMode:LongInt); virtual;
    procedure   do_move(AWidget:ZWidget; AXpos,AYpos:LongInt); virtual;
    procedure   do_redraw(AWidget:ZWidget; AState:LongInt=0); virtual;
    procedure   do_resize(AWidget:ZWidget; AWidth,AHeight:LongInt); virtual;
    procedure   do_update(AWidget:ZWidget); virtual;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_util;

//procedure ZWidget.on_paint(ACairo:ZCairo; ARect:ZRect);
//begin
//end;

//----------------------------------------------------------------------
// ZWidgetPainter
//----------------------------------------------------------------------

procedure ZWidgetPainter.on_paint(AWidget:ZWidget; ACairo:ZCairo; ARect:ZRect);
begin
end;

//----------------------------------------------------------------------
// ZWidget
//----------------------------------------------------------------------

constructor ZWidget.create(ARect:ZRect; AAlignment:LongWord; APainter:ZWidgetPainter);
begin
  inherited create;
  FAlignment          := AAlignment; //zwa_none;
  FCapturedWidget     := nil;
//FClientRect         := rect(0,0);
  FContentRect        := rect(0,0);
//FCtrlIndex          := -1;
  FFlags              :=  zwf_active +
                          zwf_align +
                          zwf_capture +
                          //zwf_clip +
                          //zwf_transp
                          zwf_visible;
  FHoverWidget        := nil;
  FIndex              := -1;
  FInitialRect        := ARect;
  FLayout.FAlignment  := AAlignment;
  FLayout.FMarginX    := 0;
  FLayout.FMarginY    := 0;
  FLayout.FPaddingX   := 0;
  FLayout.FPaddingY   := 0;
  FText               := 'ZWidget';
  FPainter            := APainter;//nil;
  FParameter          := nil;
  FParentWidget       := nil;
  FRect               := ARect;
  FSubWidgets         := ZWidgetArray.create;
  FValue              := 0;
//FWdgIndex           := -1;
end;

destructor ZWidget.destroy;
begin
  {$ifndef Z_NOAUTODELETE}
  deleteWidgets;
  {$endif}
  FSubWidgets.destroy;
  inherited;
end;

//----------

procedure ZWidget.appendWidget(AWidget:ZWidget);
begin
  AWidget.FParentWidget := self;
  //AWidget.FIndex := FSubWidgets.size;
  // skin/painter???
  FSubWidgets.append(AWidget);
end;

//----------

//procedure ZWidget.clearFlag(AFlag:LongWord);
//begin
//  //FFlags := FFlags and (not AFlag);
//  FFlags := FFlags - AFlag;   // !!!!!
//end;

//----------

procedure ZWidget.deleteWidgets;
var
  i : LongInt;
  w : ZWidget;
begin
  //writeln('ZWidget.deleteWidgets');
  if FSubWidgets.size > 0 then
  begin
    for i := 0 to FSubWidgets.size-1 do
    begin
      w := FSubWidgets[i];
      //writeln(i,',',HexStr(w));
      w.destroy;
      //writeln('q');
    end;
  end;
  //writeln('ZWidget.deleteWidgets ok');
end;

//----------

{
  find widget, or sub-widget, or sub-sub-..widget at X,Y
  (depth first traversal)
  scans backward in sub-widgets list
  (opposite of the painters algo we use for drawing)
  so we find the top-most widget, hopefully the one we clicked on
  if none found, returns self
}

function ZWidget.findSubWidget(AXpos,AYpos:LongInt) : ZWidget;
var
  wdg,w : ZWidget;
  i,num : LongInt;
begin
  //result := self;
  wdg := self;
  num := FSubWidgets.size;
  if num > 0 then
  begin
    for i := num-1 downto 0 do
    begin
      w := FSubWidgets[i];
      //if zwf_active in w.FFlags then
      if ZHasFlag(w.FFlags,zwf_active)  then
      begin
        if w.FRect.contain(AXpos,AYpos) then
        begin
          wdg := w.findSubWidget(AXpos,AYpos);
          if wdg <> w then exit(wdg)
          else exit(w);
        end //contains
      end; //active
    end; //for num
  end; //num>0
  result := wdg;
end;

//----------

function ZWidget.getFlags : LongWord{ZWidgetFlags};
begin
  result := FFlags;
end;

//----------

function ZWidget.getIndex : LongInt;
begin
  result := FIndex;
end;

//----------

function ZWidget.getPainter : ZWidgetPainter;
begin
  result := FPainter;
end;

//----------

function ZWidget.getParent : ZWidget;
begin
  result := FParentWidget;
end;

//----------

function ZWidget.getRect : ZRect;
begin
  result := FRect;
end;

//----------

function ZWidget.getParameter : ZParameter;
begin
  result := FParameter;
end;

//----------

function ZWidget.getText : PChar;
begin
  result := FText;
end;

//----------

function ZWidget.getValue : Single;
begin
  result := FValue;
end;

//----------

procedure ZWidget.setAlignment(AAlignment:LongWord);
begin
  FAlignment := AAlignment;
  FLayout.FAlignment:= AAlignment;
end;

//----------

//procedure ZWidget.setFlag(AFlag:LongWord);
//begin
//  FFlags := FFLags or AFlag;
//end;

//----------

procedure ZWidget.setFlags(AFlags:LongWord{ZWidgetFlags});
begin
  FFlags := AFlags;
end;

//----------

procedure ZWidget.setIndex(AIndex:LongInt);
begin
  FIndex := AIndex;
end;

//----------

procedure ZWidget.setMargin(x,y:longint);
begin
  FLayout.FMarginX := x;
  FLayout.FMarginY := y;
end;

//----------

procedure ZWidget.setPadding(x,y:longint);
begin
  FLayout.FPaddingX := x;
  FLayout.FPaddingY := y;
end;

//----------

procedure ZWidget.setPainter(APainter:ZWidgetPainter);
begin
  FPainter := APainter;
end;

//----------

procedure ZWidget.setParameter(AParameter:ZParameter);
begin
  FParameter := AParameter;
end;

//----------

procedure ZWidget.setText(AText:PChar);
begin
  FText := AText;
end;

//----------

procedure ZWidget.setValue(AValue:Single);
begin
  FValue := AValue;
end;

//----------------------------------------------------------------------
// on_
//----------------------------------------------------------------------

procedure ZWidget.on_Timer;
begin
end;

//----------

{
  move widget,
  also move sub-widgets the same amount
}

procedure ZWidget.on_setPos(AXpos,AYpos:LongInt);
var
  i,dx,dy,wx,wy : longint;
  widget :ZWidget;
begin
  {if (FRect.x <> AXpos) or (FRect.y <> AYpos) then
  begin
    FRect.x := AXpos;
    FRect.y := AYpos;
  end;}
  dx := AXpos - FRect.x;
  dy := AYpos - FRect.y;
  if FSubWidgets.size>0 then
  begin
    for i := 0 to FSubWidgets.size - 1 do
    begin
      widget := FSubWidgets[i];
      wx := widget.FRect.x;
      wy := widget.FRect.y;
      widget.on_setPos( wx+dx, wy+dy );
    end;
  end;
  FRect.x := AXpos;
  FRect.y := AYpos;
end;

//----------

{
  set widget size
  we don't resize the sub-widgets,
  they will be handled in following re-alignment..
}

procedure ZWidget.on_setSize(AWidth,AHeight:LongInt);
begin
  //writeln('Widget.on_SetSize');
  {if (FRect.w <> AWidth) or (FRect.h <> AHeight) then
  begin
    FRect.setup(AWidth,AHeight);
  end;}
  {if AWidth  < FMinWidth  then AWidth  := FMinWidth;
  if AWidth  > FMaxWidth  then AWidth  := FMaxWidth;
  if AHeight < FMinHeight then AHeight := FMinHeight;
  if AHeight > FMaxHeight then AHeight := FMaxHeight;}
  FRect.w := AWidth;
  FRect.h := AHeight;
  //setup(AWidth,AHeight);
  //on_Align();
end;

//----------

//procedure ZWidget.on_scroll(ADeltaX,ADeltaY:LongInt);
//begin
//end;

//----------

{
  align subwidgets (not itself)..
  keep track of rect encapsulating each widgets content (sub-widgets),
  so we can resize widgets depending on their content,
  or resize scrollbar size, etc..
}

procedure ZWidget.on_Align;
var
  i            : LongInt;
  parent       : ZRect;
  client       : ZRect;
  widget       : ZWidget;
  widgetwidth  : LongInt;
  widgetheight : longInt;
begin

  //if zwf_align in FFlags then
  if ZHasFlag(FFlags,zwf_align) then
  begin
    parent := rect(FRect);
    parent.add( FLayout.FMarginX,FLayout.FMarginY,-(FLayout.FMarginX*2),-(FLayout.FMarginY*2));
    client := rect(parent);
    FContentRect.setup(FRect.x,FRect.y,0,0);

    //stackx := cli.x;
    //staxky := cli.y;
    //largestw := 0;
    //largesth := 0;

    if FSubWidgets.size > 0 then
    begin
      for i := 0 to FSubWidgets.size-1 do
      begin
        widget := FSubWidgets[i];
        widgetwidth  := widget.FRect.w;  // current widget width
        widgetheight := widget.FRect.h;  // height
        case widget.FAlignment of

//----------

          //  _____
          // |  _  |
          // | |_| |
          // |_____|
          //

          //case wa_None:
          //  wdg->doSetPos(wdg->mOrig.x+parent.x, wdg->mOrig.y+parent.y);
          //  break;

          zwa_none:
            begin
              widget.on_setPos(widget.FInitialRect.x+parent.x, widget.FInitialRect.y+parent.y);
            end;

          //   _____
          //  |     |
          //  |     |
          //  |_____|
          //

          //case wa_Client:
          //  wdg->doSetPos(  client.x, client.y );
          //  wdg->doSetSize( client.w, client.h );
          //  break;

          zwa_client:
            begin
              widget.on_setPos(client.x,client.y);
              widget.on_setSize(client.w,client.h);
            end;

//----------

          //  _____
          // |  |
          // |  |
          // |__|__
          //

          //case wa_Left:
          //  wdg->doSetPos( client.x, client.y );
          //  wdg->doSetSize( ww, client.h );
          //  client.x += (ww+mPaddingX);
          //  client.w -= (ww+mPaddingX);
          //  stackx    = client.x;
          //  stacky    = client.y;
          //  largesth  = 0;
          //  break;

          zwa_left:
            begin
              widget.on_setPos(client.x,client.y);
              widget.on_setSize(widgetwidth,client.h);
              client.x := client.x + (widgetwidth+FLayout.FPaddingX);
              client.w := client.w - (widgetwidth+FLayout.FPaddingX);
              //stackx := client.x;
              //stacky := client.y;
              //largesth := 0;
            end;

          //  ______
          //     |  |
          //     |  |
          //  __ |__|
          //

          //case wa_Right:
          //  wdg->doSetPos( client.x2()-ww+1, client.y );
          //  wdg->doSetSize( ww, client.h );
          //  client.w -= (ww + mPaddingX);
          //  break;

          zwa_right:
            begin
              widget.on_setPos(client.x2-widgetwidth+1,client.y);
              widget.on_setSize(widgetwidth,client.h);
              client.w := client.w - (widgetwidth+FLayout.FPaddingX);
            end;

          //  ______
          // |______|
          // |      |
          //

          //case wa_Top:
          //
          //  wdg->doSetPos( client.x, client.y );
          //  wdg->doSetSize( client.w, wh );
          //  client.y += (wh+mPaddingY);
          //  client.h -= (wh+mPaddingY);
          //  stackx    = client.x;
          //  stacky    = client.y;
          //  largestw  = 0;
          //  break;

          zwa_top:
            begin
              widget.on_setPos(client.x,client.y);
              widget.on_setSize(client.w,widgetheight);
              client.y := client.y + (widgetheight+FLayout.FPaddingY);
              client.h := client.h - (widgetheight+FLayout.FPaddingY);
              //stackx := client.x;
              //stacky := client.y;
              //largestw := 0;
            end;

          //
          // |      |
          // |______|
          // |______|
          //

          //case wa_Bottom:
          //
          //  wdg->doSetPos( client.x, client.y2()-wh+1 );
          //  wdg->doSetSize( client.w, wh );
          //  client.h -= (wh+mPaddingY);
          //  break;

          zwa_bottom:
            begin
              widget.on_setPos(client.x,client.y2-widgetheight+1);
              widget.on_setSize(client.w,widgetheight);
              client.h := client.h - (widgetheight+FLayout.FPaddingY);
            end;

          //TODO: the rest of the alignment modes..

//----------

        end; // case

        FContentRect.combine( widget.FRect ); // keep track of outer boundary
        widget.on_Align;

      end; // for
    end; //sub.size >0
  end; // align
end;

//----------

{
  draw itself (with the painter), and then its eventual sub-widgets
  (painters algo)
}

procedure ZWidget.on_Paint(ACairo:ZCairo; ARect:ZRect);
var
  i : LongInt;
  wdg : ZWidget;
begin
  //if zwf_visible in FFlags then
  if ZHasFlag(FFlags,zwf_visible) then
  begin
    if FRect.intersect(ARect) then
    begin

      (* if zwf_clip in FFlags then
      begin
        ACairo.rectangle( FRect.x{  + FMarginX},
                          FRect.y{  + FMarginY},
                          FRect.w{ - FMarginX},
                          FRect.h{ - FMarginY} );
        //aCanvas->setClipRect(mRect.x+mMarginX,mRect.y+mMarginY,mRect.x2()-mMarginX,mRect.y2()-mMarginY);
        ACairo.clip;
      end; *)

      if Assigned(FPainter) then
      begin
        FPainter.on_paint(self,ACairo,ARect);
      end;

      if FSubWidgets.size > 0 then
      begin

        //if zwf_clip in FFlags then
        if ZHasFlag(FFlags,zwf_clip) then
        begin
          ACairo.rectangle( FRect.x, FRect.y, FRect.w, FRect.h);
          ACairo.clip;
        end;

        for i := 0 to FSubWidgets.size-1 do
        begin
          wdg := FSubWidgets[i];
          //if zwf_visible in wdg.FFlags then
          if ZHasFlag(wdg.FFlags,zwf_visible) then
          begin
            if wdg.FRect.intersect(ARect) then
            begin
              //wdg.on_Paint(ACairo,ARect);
              wdg.on_Paint(ACairo,FRect);

            end;
          end; // wdg.visible
        end; // for

        //if zwf_clip in FFlags then ACairo.resetClip;
        if ZHasFlag(FFlags,zwf_clip) then ACairo.resetClip;

      end;

      (* if zwf_clip in FFlags then
      begin
        ACairo.resetClip;
      end; *)

    end; // intersect
  end; // visible
end;

//----------

{
  mouse cursor enters widget
  AWidget = widget it came from (or nil)
}

procedure ZWidget.on_Enter(AWidget:ZWidget);
begin
end;

//----------

{
  mouse cursor enters widget
  AWidget = widget it came from (or nil)
}

procedure ZWidget.on_Leave(AWidget:ZWidget);
begin
end;

//----------

procedure ZWidget.on_MouseDown(AXpos,AYpos:LongInt; AButton,AState:LongWord);
var
  hover : ZWidget;
begin
  //if (mModalWidget) mModalWidget->doMouseDown(aXpos,aYpos,aButton);
  //else
  if Assigned(FCapturedWidget) then FCapturedWidget.on_MouseDown(AXpos,AYpos,AButton,AState)
  else
  begin
    //if zwf_active in FFlags then
    if ZHasFlag(FFlags,zwf_active) then
    begin
      hover := findSubWidget(AXpos,AYpos);
      if Assigned(hover) then
      begin
        if hover <> self then
        begin
          //if zwf_capture in FFlags then FCapturedWidget := hover;
          if ZHasFlag(FFlags,zwf_capture) then FCapturedWidget := hover;
          hover.on_MouseDown(AXpos,AYpos,AButton,AState);
        end; // !hover
      end;
    end; // active
  end; // !capture
end;

//----------

procedure ZWidget.on_MouseUp(AXpos,AYpos:LongInt; AButton,AState:LongWord);
var
  hover : ZWidget;
begin
  //if (mModalWidget) mModalWidget->doMouseUp(aXpos,aYpos,aButton);
  //else
  if assigned(FCapturedWidget) then
  begin
    FCapturedWidget.on_MouseUp(aXpos,aYpos,aButton,AState);
    FCapturedWidget := nil;
  end //capture
  else
  begin
    //if zwf_active in FFlags then
    if ZHasFlag(FFlags,zwf_active) then
    begin
      hover := findSubWidget(AXpos,AYpos);
      if Assigned(hover) then
      begin
        if hover <> self then hover.on_MouseUp(aXpos,aYpos,aButton,AState);
      end;
    end; //active
  end;
end;

//----------

procedure ZWidget.on_MouseMove(AXpos,AYpos:LongInt; AState:LongWord);
var
  hover : ZWidget;
begin
  //if (mModalWidget) mModalWidget->doMouseMove(aXpos,aYpos,aButton);
  //else
  if Assigned(FCapturedWidget) then
  begin
    FCapturedWidget.on_MouseMove(aXpos,aYpos,AState)
  end
  else
  begin
    hover := findSubWidget(aXpos,aYpos);
    if Assigned(hover) then // nil?
    begin

      if hover <> FHoverWidget then
      begin
        if Assigned(FHoverWidget) then FHoverWidget.on_Leave({FCaptWdg}hover);
        hover.on_Enter(FHoverWidget);
        FHoverWidget := hover;
        //FHoverWidget.on_Enter(FCaptWdg);
      end;

      // make sure we don't call ourselves recursively!
      if hover <> self then
      begin
        hover.on_MouseMove(aXpos,aYpos,aState);
      end;

    end; // hover
  end;
end;

//----------

procedure ZWidget.on_KeyDown(AKey:LongInt; AState:LongWord);
begin
  //if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
  //else
  if Assigned(FCapturedWidget) then FCapturedWidget.on_KeyDown(aKey,aState);
end;

//----------

procedure ZWidget.on_KeyUp(AKey:LongInt; AState:LongWord);
begin
  //if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
  //else
  if Assigned(FCapturedWidget) then FCapturedWidget.on_KeyUp(aKey,aState);
end;

//----------------------------------------------------------------------
// do_
//----------------------------------------------------------------------

procedure ZWidget.do_move(AWidget:ZWidget; AXpos,AYpos:LongInt);
begin
  if Assigned(FParentWidget) then FParentWidget.do_move(AWidget,AXpos,AYpos);
end;

//----------

  (*
  // called from wdgSizer
  virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY, int aMode)
    {
      int w = mRect.w + aDeltaX;
      int h = mRect.h + aDeltaY;
      axWidget::doSetSize(w,h);
      mListener->onSize(aWidget,aDeltaX,aDeltaY,aMode);
    }
  *)

procedure ZWidget.do_resize(AWidget:ZWidget; AWidth,AHeight:LongInt);
begin
  //writeln('ZWidget.on_resize');
  if Assigned(FParentWidget) then FParentWidget.do_resize(AWidget,AWidth,AHeight);
end;

//----------

procedure ZWidget.do_redraw(AWidget:ZWidget; AState:LongInt);
begin
  if Assigned(FParentWidget) then FParentWidget.do_redraw(AWidget,AState);
end;

//----------

procedure ZWidget.do_update(AWidget:ZWidget);
begin
  if Assigned(FParentWidget) then FParentWidget.do_update(AWidget);
end;

//----------

procedure ZWidget.do_cursor(AWidget:ZWidget; ACursor:LongInt);
begin
  if Assigned(FParentWidget) then FParentWidget.do_cursor(AWidget,ACursor);
end;

//----------

procedure ZWidget.do_hint(AWidget:ZWidget; AHint:PChar);
begin
  if Assigned(FParentWidget) then FParentWidget.do_hint(AWidget,AHint);
end;

//----------

procedure ZWidget.do_modal(AWidget:ZWidget; AMode:LongInt);
begin
  if Assigned(FParentWidget) then FParentWidget.do_modal(AWidget,AMode);
end;

//----------------------------------------------------------------------
end.


