{$include sphaera.inc}
unit s_canvas;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------


uses
  {$ifdef SPH_CAIRO} s_canvas_cairo, {$endif}
  s_color,
  s_const,
  s_rect;

type

  SCanvas = class(SCanvasImpl)
  public
    procedure   setColor(AColor:SColor);
    procedure   drawTextBox(x,y,w,h:double; AText:PChar; AAlignment:LongInt=sta_center);
    procedure   drawRoundedPath(x,y,w,h,r:double);
    procedure   drawHLine(x,y,x2:double);
    procedure   drawVLine(x,y,y2:double);
    procedure   drawFrame(x1,y1,x2,y2:double);
    procedure   cliprect(ARect:SRect; AReset:Boolean=false);
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

procedure SCanvas.setColor(AColor:SColor);
begin
  setSourceColor(AColor.r,AColor.g,AColor.b,AColor.a);
end;

//----------

procedure SCanvas.drawTextBox(x,y,w,h:double; AText:PChar; AAlignment:LongInt);
var
  e:PTextExtents;
  xx,yy{,ww,hh} : single;
begin
  e := textExtents(AText);
  //ACairo.moveTo(R.x + R.w/2 - ext^.width/2 , R.y + R.h/2 + ext^.height/2 );
  case AAlignment of
    sta_left:
      begin
        xx := x;
        yy := (y+h/2) - (e^.height/2 + e^.y_bearing);
      end;
    sta_right:
      begin
        xx := (x+w-1) - (e^.width  + e^.x_bearing);
        yy := (y+h/2) - (e^.height/2 + e^.y_bearing);
      end;
    sta_top:
      begin
        xx := (x + w/2) - (e^.width/2  + e^.x_bearing);
        yy := y + e^.height;
      end;
    sta_bottom:
      begin
        xx := (x + w/2) - (e^.width/2  + e^.x_bearing);
        yy := (y+h-1) - (e^.height + e^.y_bearing);
      end;
    sta_center:
      begin
        xx := (x + w/2) - (e^.width/2  + e^.x_bearing);
        yy := (y+h/2) - (e^.height/2 + e^.y_bearing);
      end;
  end;
  //xx := (x + w/2) - (e^.width/2  + e^.x_bearing);
  //yy := (y + h/2) - (e^.height/2 + e^.y_bearing);
  moveTo(xx,yy);
  showText(AText);
end;

// ----------

procedure SCanvas.drawRoundedPath(x,y,w,h,r:double);
var
  degrees : double;
begin
  degrees := S_PI / 180.0;
  newSubPath();
  arc(x + w-r, y+r,   r, -90  * degrees, 0   * degrees);
  arc(x + w-r, y+h-r, r,  0   * degrees, 90  * degrees);
  arc(x + r,   y+h-r, r,  90  * degrees, 180 * degrees);
  arc(x + r,   y+r,   r,  180 * degrees, 270 * degrees);
  closePath();
end;

//----------

procedure SCanvas.drawHLine(x,y,x2:double);
begin
  moveTo(x+0.5,y+0.5);
  lineTo(x2+0.5,y+0.5);
  stroke;
end;

//----------

procedure SCanvas.drawVLine(x,y,y2:double);
begin
  moveTo(x+0.5,y+0.5);
  lineTo(x+0.5,y2+0.5);
  stroke;
end;

//----------

procedure SCanvas.drawFrame(x1,y1,x2,y2:double);
begin
  moveTo(x1+0.5,y1+0.5);
  lineTo(x2+0.5,y1+0.5);
  lineTo(x2+0.5,y2+0.5);
  lineTo(x1+0.5,y2+0.5);
  lineTo(x1+0.5,y1+0.5);
  stroke;
end;

//----------

procedure SCanvas.cliprect(ARect:SRect; AReset:Boolean);
begin
  if AReset then resetClip;
  rectangle(ARect.x,ARect.y,ARect.w,ARect.h);
  clip;
end;


end.

