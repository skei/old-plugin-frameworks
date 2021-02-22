{$include sphaera.inc}
unit s_widget_wave;

{
  drag mouse up/down to change value (0..1)
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_canvas,
  s_color,
  s_rect,
  s_widget;

type

  SWidgetWave = class(SWidget)
  protected
    FBackColor  : SColor;
    FWaveColor  : SColor;
    FGridColor  : SColor;
    FBuffer     : PSingle;
    FSize       : LongInt;
    FNumBeats   : LongInt;
    FSubDiv     : LongInt;
    FRecPos     : LongInt;
  public
    constructor create(ARect:SRect; AAlignment:Longword);
    destructor  destroy; override;
    procedure   on_paint(ACanvas:SCanvas; ARect:SRect); override;
    property    buffer   : PSingle read FBuffer write FBuffer;
    property    size     : LongInt read FSize write FSize;
    property    numbeats : LongInt read FNumBeats write FNumBeats;
    property    subdiv   : LongInt read FSubDiv write FSubDiv;
    property    recpos   : LongInt read FRecPos write FRecPos;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses
//  s_window;

//----------

constructor SWidgetWave.create(ARect:SRect; AAlignment:Longword);
begin
  inherited;
  FText := 'SWidgetWave';
  FBackColor := s_dark_grey;
  FWaveColor := s_light_grey;
  FGridColor := s_grey;
end;

//----------

destructor SWidgetWave.destroy;
begin
  inherited;
end;

//----------

procedure SWidgetWave.on_paint(ACanvas:SCanvas; ARect:SRect);
var
  wr : SRect;
  i,num : longint;
  w,x,y,h2 : single;
  step : single;
  n :single;
  idx:single;
begin
  wr := FRect;

  { background }
  ACanvas.setColor(FBackColor);
  ACanvas.rectangle(wr.x,wr.y,wr.w,wr.h);
  ACanvas.fill;

  ACanvas.setLineWidth(1);
  { grid }
  ACanvas.setColor(FGridColor);
  num := FNumBeats * FSubDiv;
  if num > 1 then
  begin
    w := wr.w / num;
    x := 0.5 + wr.x + w;
    for i := 0 to num-2 do
    begin
    //while num > 0 do
    //begin
      ACanvas.moveTo(x,wr.y);
      ACanvas.lineTo(x,wr.y2);
      ACanvas.stroke;
      x += w;
      //num -= 1;
    end;
  end;

  { wave }
  if (wr.w>0) and (wr.h>0) and Assigned(FBuffer) and (FSize > 0) then
  begin
    ACanvas.setColor(FWaveColor);
    h2 := wr.h / 2;
    x := wr.x + 0.5;
    idx := 0;
    step := FSize / wr.w;
    ACanvas.moveTo(wr.x+0.5,wr.y+h2);
    for i := 0 to wr.w-1 do
    begin
      n := FBuffer[ trunc(idx) * 2 ]; // left
      y := wr.y + h2 + (n*h2);
      ACanvas.lineTo(x,y);
      x += 1;
      idx += step;
    end;
    ACanvas.stroke;
  end;

  { cursor }
  x := 0.5 + wr.w * FRecPos / FSize;
  ACanvas.setColor(s_light_red); // green
  ACanvas.moveTo(x,wr.y);
  ACanvas.lineTo(x,wr.y2);
  ACanvas.stroke;
  //inherited; // paint sub-widgets
end;

//----------------------------------------------------------------------
end.

