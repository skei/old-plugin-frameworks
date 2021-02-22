unit gui1;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_descriptor,
  //z_instance,
  z_editor,
  z_widget,
  z_widget_value,
  z_painter,
  z_cairo,
  z_rect;

type

  myDescriptor = class(ZDescriptor)
  public
    procedure on_create; override;
  end;

  //myInstance = class(ZInstance)
  //end;

  myPainter = class(ZWidgetPainter)
  public
    procedure   on_paint(AWidget:ZWidget; ACairo:ZCairo; ARect:ZRect); override;
  end;

  myEditor = class(ZEditor)
  private
    paint1  : ZWidgetPainter;
    paint2  : ZWidgetPainter;
    paint3  : ZWidgetPainter;
  public
    procedure on_create; override;
    procedure on_destroy; override;
    procedure on_open; override;
  end;


procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  zenith,
  z_const,
  z_color,
  //z_rect,
  z_window;
  //z_widget;

procedure zenith_prepare;
begin
  zenith_register(myDescriptor);
  //zenith_register(myInstance);
  zenith_register(myEditor);
end;

//----------------------------------------------------------------------

procedure myDescriptor.on_create;
begin
  FFlags += zpf_hasEditor;
end;

//----------------------------------------------------------------------

procedure myPainter.on_paint(AWidget:ZWidget; ACairo:ZCairo; ARect:ZRect);
var
  r:ZRect;
  w,h:longint;
begin
  writeln('paint!');
  r := AWidget.getRect;
  //r.crop(ARect);
  //f not r.empty then
  begin
    ACairo.setSourceColor(0.5,0,0);
    ACairo.rectangle(r.x,r.y,r.w,r.h);
    ACairo.fill;
    w := r.w div 3;
    h := r.h div 2;
    ACairo.setSourceColor(1,1,1);
    ACairo.moveTo( w*0,h*1 );
    ACairo.curveTo( w*1,h*0, w*2,h*2, w*3,h*1);
    ACairo.setLineWidth( (w+h) div 20);
    ACairo.stroke;
  end;
end;



//----------------------------------------------------------------------

procedure myEditor.on_create;
begin
  //writeln('myEditor.on_create');
  FRect := rect(256,256);
  paint1 := ZPaintColor.create(z_dark_green);
  paint2 := myPainter.create;
  paint3 := ZPaintValue.create( z_dark_blue, z_light_blue, z_light_yellow );
end;

//----------

procedure myEditor.on_destroy;
begin
  //writeln('myEditor.on_destroy');
  paint1.destroy;
  paint2.destroy;
  paint3.destroy;
end;

//----------

procedure myEditor.on_open;
var
  win : ZWindow;
begin
  //writeln('myEditor.on_open');
  win := FWindow;
  win.appendWidget( ZWidget.create( rect(100,100), zwa_right, paint1 ) );
  win.appendWidget( ZWidgetValue.create( rect(20), zwa_top, paint3 ) );
  win.appendWidget( ZWidgetValue.create( rect(20), zwa_bottom, paint3 ) );
  win.appendWidget( ZWidget.create( rect(100,100), zwa_client, paint2 ) );
  win.on_align;
end;

//----------------------------------------------------------------------
end.

