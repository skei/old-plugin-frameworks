unit s_widget;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_array,
  s_canvas,
  s_const,
  s_parameter,
  s_rect;

type

  SWidgetLayout = record
    FAlignment  : LongWord;
    FMarginX    : LongInt;
    FMarginY    : LongInt;
    FPaddingX   : LongInt;
    FPaddingY   : LongInt;
  end;

  SWidget = class;
  SWidgetArray = specialize SArray<SWidget>;

  //----------

  SWidget = class
  protected
    FName           : PChar;
    FFlags          : LongWord;
    FRect           : SRect;
    FLayout         : SWidgetLayout;
    FParameter      : SParameter;
    FText           : PChar;
    FValue          : Single;
    FCursor         : LongInt;
    FHint           : PChar;
    //FIndex          : LongInt;
    //
    FCapturedWidget : SWidget;
    FContentRect    : SRect;
    FHoverWidget    : SWidget;
    FInitialRect    : SRect;
    FParentWidget   : SWidget;
    FSubWidgets     : SWidgetArray;
    FConnection     : LongInt;
  public
    constructor create(ARect:SRect; AAlignment:LongWord=swa_none);
    destructor  destroy; override;
    procedure   appendWidget(AWidget:SWidget);
    procedure   deleteWidgets;
    function    findSubWidget(AXpos,AYpos:LongInt) : SWidget;
    procedure   alignment(AAlignment:LongInt);
    procedure   margins(AMarginX,AMarginY:LongInt);
    procedure   padding(APaddingX,APaddingY:LongInt);
  public
    property    _rect  : SRect read FRect write FRect;
    property    _text  : PChar read FText write FText;
    property    _value : Single read FValue write FValue;
    property    _param : SParameter read FParameter write FParameter;
    property    _connection : LongInt read FConnection write FConnection;
    property    _cursor : LongInt read FCursor write FCursor;
    //property    _index : LongInt read FIndex {write FIndex};
    property    _subWidgets : SWidgetArray read FSubWidgets;
  public
    //procedure   on_create; virtual;
    //procedure   on_destroy; virtual;
    procedure   on_setPos(AXpos,AYpos:LongInt); virtual;
    procedure   on_setSize(AWidth,AHeight:LongInt); virtual;
    procedure   on_align; virtual;
    procedure   on_paint(ACanvas:SCanvas; ARect:SRect); virtual;
    procedure   on_keyDown(AKey,AShift:LongInt); virtual;
    procedure   on_keyUp(AKey,AShift:LongInt); virtual;
    procedure   on_mouseDown(AXpos,AYpos,AButton,AState:LongInt); virtual;
    procedure   on_mouseMove(AXpos,AYpos,AState:LongInt); virtual;
    procedure   on_mouseUp(AXpos,AYpos,AButton,AState:LongInt); virtual;
    procedure   on_enter({%H-}AWidget:SWidget); virtual;
    procedure   on_leave({%H-}AWidget:SWidget); virtual;
    procedure   on_timer; virtual;
  public
    procedure   do_move(AWidget:SWidget; AXpos,AYpos:LongInt); virtual;
    procedure   do_resize(AWidget:SWidget; AWidth,AHeight:LongInt); virtual;
    procedure   do_cursor(AWidget:SWidget; ACursor:LongInt); virtual;
    procedure   do_hint(AWidget:SWidget; AHint:PChar); virtual;
    procedure   do_modal(AWidget:SWidget; AMode:LongInt); virtual;
    procedure   do_redraw(AWidget:SWidget; AState:LongInt=0); virtual;
    procedure   do_update(AWidget:SWidget); virtual;
  end;


//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_util;

//----------

constructor SWidget.create(ARect:SRect; AAlignment:LongWord);
begin
  inherited create;

  FContentRect        := rect(0,0);
  FFlags              :=  swf_active +
                          swf_align +
                          swf_capture +
                          swf_clip +
                          //swf_transp
                          swf_visible;
  FHoverWidget        := nil;
  FInitialRect        := ARect;
  FLayout.FAlignment  := AAlignment;
  FLayout.FMarginX    := 0;
  FLayout.FMarginY    := 0;
  FLayout.FPaddingX   := 0;
  FLayout.FPaddingY   := 0;
  FText               := 'SWidget';
  FParameter          := nil;
  FParentWidget       := nil;
  FCapturedWidget     := nil;
  FRect               := ARect;
  FSubWidgets         := SWidgetArray.create;
  FValue              := 0;
  FConnection         := -1;

  FCursor := -1;
  FHint := nil;
  //FIndex := 0;


end;

//----------

destructor SWidget.destroy;
begin
  {$ifndef SPH_NOAUTODELETE}
  deleteWidgets;
  {$endif}
  FSubWidgets.destroy;
  inherited;
end;

//----------------------------------------------------------------------

procedure SWidget.appendWidget(AWidget:SWidget);
begin
  AWidget.FParentWidget := self;
  //AWidget.FIndex := FSubWidgets.size;
  FSubWidgets.append(AWidget);
end;

//----------

procedure SWidget.deleteWidgets;
var
  i : LongInt;
  w : SWidget;
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
  //writeln('SWidget.deleteWidgets ok');
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

function SWidget.findSubWidget(AXpos,AYpos:LongInt) : SWidget;
var
  wdg,w : SWidget;
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
      if SHasFlag(w.FFlags,swf_active)  then
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

{function SWidget.getRect : SRect;
begin
  result := FRect;
end;

function SWidget.getText : PChar;
begin
  result := FText;
end;

function SWidget.getValue : Single;
begin
  result := FValue;
end;

function SWidget.getParameter : SParameter;
begin
  result := FParameter;
end;

procedure SWidget.setParameter(AParameter:SParameter);
begin
  FParameter := AParameter;
end;

procedure SWidget.setValue(AValue:Single);
begin
  FValue := AValue;
end;}

//----------

procedure SWidget.alignment(AAlignment:LongInt);
begin
  FLayout.FAlignment := AAlignment;
end;

procedure SWidget.margins(AMarginX,AMarginY:LongInt);
begin
  FLayout.FMarginX := AMarginX;
  FLayout.FMarginY := AMarginY;
end;

procedure SWidget.padding(APaddingX,APaddingY:LongInt);
begin
  FLayout.FPaddingX := APaddingX;
  FLayout.FPaddingY := APaddingY;
end;

(*

procedure SWidget.setRect(ARect:SRect);
begin
  FRect :=;
end;

procedure SWidget.setText(AText:PChar);
begin
  FText := AText;
end;

procedure SWidget.setValue/AValue:Single);
begin
  FValue := AValue;
end;

procedure SWidget.setParameter(AParameter:SParameter);
begin
  FParameter := AParameter;
end;

*)

//----------------------------------------------------------------------
// on_ (events from parent)
//----------------------------------------------------------------------

//procedure SWidget.on_create;
//begin
//end;

//----------

//procedure SWidget.on_destroy;
//begin
//end;

//----------

{
  move widget,
  also move sub-widgets the same amount
}

procedure SWidget.on_setPos(AXpos,AYpos:LongInt);
var
  i,dx,dy,wx,wy : longint;
  widget :SWidget;
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

procedure SWidget.on_setSize(AWidth,AHeight:LongInt);
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

{
  align subwidgets (not itself)..
  keep track of rect encapsulating each widgets content (sub-widgets),
  so we can resize widgets depending on their content,
  or resize scrollbar size, etc..
}

procedure SWidget.on_align;
var
  i            : LongInt;
  parent       : SRect;
  client       : SRect;
  widget       : SWidget;
  widgetwidth  : LongInt;
  widgetheight : longInt;
begin
  //if zwf_align in FFlags then
  if SHasFlag(FFlags,swf_align) then
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
        case widget.FLayout.FAlignment of

//----------

          //  _____
          // |  _  |
          // | |_| |
          // |_____|
          //

          swa_none:
            begin
              widget.on_setPos(widget.FInitialRect.x+parent.x, widget.FInitialRect.y+parent.y);
            end;

          //   _____
          //  |     |
          //  |     |
          //  |_____|
          //

          swa_fillClient:
            begin
              widget.on_setPos(client.x,client.y);
              widget.on_setSize(client.w,client.h);
            end;

//----------

          //  ______
          // | |
          // | |
          // |_|____
          //

          swa_fillLeft:
            begin
              widget.on_setPos(client.x,client.y);
              widget.on_setSize(widgetwidth,client.h);
              client.x := client.x + (widgetwidth+FLayout.FPaddingX);
              client.w := client.w - (widgetwidth+FLayout.FPaddingX);
              //stackx := client.x;
              //stacky := client.y;
              //largesth := 0;
            end;

          //  _____
          //     | |
          //     | |
          //  ___|_|
          //

          swa_fillRight:
            begin
              widget.on_setPos(client.x2-widgetwidth+1,client.y);
              widget.on_setSize(widgetwidth,client.h);
              client.w := client.w - (widgetwidth+FLayout.FPaddingX);
            end;

          //  _____
          // |_____|
          // |     |
          // |     |
          //

          swa_fillTop:
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
          // |     |
          // |_____|
          // |_____|
          //

          swa_fillBottom:
            begin
              widget.on_setPos(client.x,client.y2-widgetheight+1);
              widget.on_setSize(client.w,widgetheight);
              client.h := client.h - (widgetheight+FLayout.FPaddingY);
            end;

//----------

          //TODO: the rest of the alignment modes frmo axonlib/holos..
          //
          //swa_left
          //  _____
          // |_    |
          // |_|   |
          // |_____|
          //
          //swa_lefttop
          //  _____
          // |_|   |
          // |     |
          // |_____|
          //
          //swa_leftbottom
          //  _____
          // |     |
          // |_    |
          // |_|___|
          //
          //swa_right
          //  _____
          // |    _|
          // |   |_|
          // |_____|
          //
          //swa_righttop
          //  _____
          // |   |_|
          // |     |
          // |_____|
          //
          //swa_rightbottom
          //  _____
          // |     |
          // |    _|
          // |___|_|
          //
          //swa_top
          //  _____
          // | |_| |
          // |     |
          // |_____|
          //
          //swa_topleft
          //  _____
          // |_|   |
          // |     |
          // |_____|
          //
          //swa_topright
          //  _____
          // |   |_|
          // |     |
          // |_____|
          //
          //swa_bottom
          //  _____
          // |     |
          // |  _  |
          // |_|_|_|
          //
          //swa_bottomleft
          //  _____
          // |     |
          // |_    |
          // |_|___|
          //
          //swa_bottomright
          //  _____
          // |     |
          // |    _|
          // |___|_|
          //

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

  called frm wm_paint & wm_size, on_redraw (sub-widgets)
}

procedure SWidget.on_paint(ACanvas:SCanvas; ARect:SRect);
var
  i : LongInt;
  wdg : SWidget;
begin
  //writeln('ZWidget.on_paint ',FText);
  if SHasFlag(FFlags,swf_visible) then
  begin
    if FRect.intersect(ARect) then
    begin
      //if Assigned(FPainter) then
      //begin
      //  FPainter.on_paint(self,ACairo,ARect);
      //end;
      if FSubWidgets.size > 0 then
      begin
        ACanvas.save;
        if SHasFlag(FFlags,swf_clip) then ACanvas.cliprect(FRect);
        for i := 0 to FSubWidgets.size-1 do
        begin
          wdg := FSubWidgets[i];
          if SHasFlag(wdg.FFlags,swf_visible) then
          begin
            if wdg.FRect.intersect(ARect) then
            begin
              wdg.on_Paint(ACanvas,ARect);
            end;
          end; // wdg.visible
        end; // for
        //if SHasFlag(FFlags,swf_clip) then ACairo.resetClip;
        ACanvas.restore;
      end;
    end; // intersect
  end; // visible
end;

//----------

procedure SWidget.on_keyDown(AKey,AShift:LongInt);
begin
  //if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
  //else
  if Assigned(FCapturedWidget) then FCapturedWidget.on_KeyDown(aKey,AShift);
end;

//----------

procedure SWidget.on_keyUp(AKey,AShift:LongInt);
begin
  //if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
  //else
  if Assigned(FCapturedWidget) then FCapturedWidget.on_KeyUp(aKey,AShift);
end;

//----------

procedure SWidget.on_mouseDown(AXpos,AYpos,AButton,AState:LongInt);
var
  hover : SWidget;
begin
  //writeln('ZWidget.on_MouseDown');
  //if (mModalWidget) mModalWidget->doMouseDown(aXpos,aYpos,aButton);
  //else
  if Assigned(FCapturedWidget) then FCapturedWidget.on_MouseDown(AXpos,AYpos,AButton,AState)
  else
  begin
    //if zwf_active in FFlags then
    if SHasFlag(FFlags,swf_active) then
    begin
      hover := findSubWidget(AXpos,AYpos);
      if Assigned(hover) then
      begin
        if hover <> self then
        begin
          //if zwf_capture in FFlags then FCapturedWidget := hover;
          if SHasFlag(FFlags,swf_capture) then FCapturedWidget := hover;
          hover.on_MouseDown(AXpos,AYpos,AButton,AState);
        end; // !hover
      end;
    end; // active
  end; // !capture
end;

//----------

procedure SWidget.on_mouseMove(AXpos,AYpos,AState:LongInt);
var
  hover : SWidget;
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

procedure SWidget.on_mouseUp(AXpos,AYpos,AButton,AState:LongInt);
var
  hover : SWidget;
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
    if SHasFlag(FFlags,swf_active) then
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

{
  mouse cursor enters widget
  AWidget = widget it came from (or nil)
}

procedure SWidget.on_Enter(AWidget:SWidget);
begin
  if FCursor >= 0 then do_cursor(self,FCursor);
  if Assigned(FHint) then do_hint(self,FHint);
end;

//----------

{
  mouse cursor enters widget
  AWidget = widget it came from (or nil)
}

procedure SWidget.on_Leave(AWidget:SWidget);
begin
  {if FCursor >= 0 then} do_cursor(self,smc_default);
  do_hint(self,'');
end;

//----------

procedure SWidget.on_Timer;
begin
end;

//----------------------------------------------------------------------
// do_ (events from sub-widgets)
//----------------------------------------------------------------------

procedure SWidget.do_move(AWidget:SWidget; AXpos,AYpos:LongInt);
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

procedure SWidget.do_resize(AWidget:SWidget; AWidth,AHeight:LongInt);
begin
  if Assigned(FParentWidget) then FParentWidget.do_resize(AWidget,AWidth,AHeight);
end;

//----------

procedure SWidget.do_redraw(AWidget:SWidget; AState:LongInt);
begin
  if Assigned(FParentWidget) then FParentWidget.do_redraw(AWidget,AState);
  // if opaque parent->do_redraw(awidget)
  // if transp & not_already_blocked parent->do_redraw(this)
end;

//----------

procedure SWidget.do_update(AWidget:SWidget);
begin
  if Assigned(FParentWidget) then FParentWidget.do_update(AWidget);
end;

//----------

procedure SWidget.do_cursor(AWidget:SWidget; ACursor:LongInt);
begin
  if Assigned(FParentWidget) then FParentWidget.do_cursor(AWidget,ACursor);
end;

//----------

procedure SWidget.do_hint(AWidget:SWidget; AHint:PChar);
begin
  if Assigned(FParentWidget) then FParentWidget.do_hint(AWidget,AHint);
end;

//----------

procedure SWidget.do_modal(AWidget:SWidget; AMode:LongInt);
begin
  if Assigned(FParentWidget) then FParentWidget.do_modal(AWidget,AMode);
end;

//----------------------------------------------------------------------
end.
