unit z_gui_base;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  //z_object,
  z_widget,
  z_color,
  z_rect;
//  z_rect;

type

  //ZWidgetBase = class;

  ZWindowBase = class;
  ZSurfaceBase = class;
  ZCanvasBase = class;
  ZImageBase = class;

  //----------

  //ZWidgetBase = class
  //end;

  //----------

  ZWindowBase = class(ZWidget) //(ZObject) //(ZWidgetBase)
  public
    procedure   beginPaint; virtual; abstract;
    procedure   endPaint; virtual; abstract;
    procedure   eventLoop; virtual; abstract;
    procedure   flush; virtual; abstract;
    procedure   grabCursor; virtual; abstract;
    procedure   hide; virtual; abstract;
    procedure   hideCursor; virtual; abstract;
    procedure   invalidate(AXpos,AYpos,AWidth,AHeight:LongInt); virtual; abstract;
    procedure   lock; virtual; abstract;
    //procedure   redrawWidget(AWidget:ZWidget); virtual; abstract;
    procedure   releaseCursor; virtual; abstract;
    procedure   reparent(AParent:Pointer); virtual; abstract;
    procedure   resetCursor; virtual; abstract;
    procedure   sendEvent(AValue:longword); virtual; abstract;
    procedure   setCursor(ACursor:LongInt) virtual; abstract;
    procedure   setCursorPos(AXpos,AYpos:LongInt); virtual; abstract;
    procedure   setPos(x,y:LongInt); virtual; abstract;
    procedure   setSize(x,y:LongInt); virtual; abstract;
    procedure   setTitle(ATitle:PChar); virtual; abstract;
    procedure   show; virtual; abstract;
    procedure   showCursor; virtual; abstract;
    procedure   startTimer(ms:LongInt); virtual; abstract;
    procedure   stopTimer; virtual; abstract;
    procedure   sync; virtual; abstract;
    procedure   unlock; virtual; abstract;
  end;

  ZSurfaceBase = class
  end;

  ZImageBase = class
  end;

  ZCanvasBase = class
  public
    procedure setColor(AColor:ZColor); virtual; abstract;
    procedure setSize(ASize:Single); virtual; abstract;

    procedure drawRect(ARect:ZRect); virtual; abstract;
    procedure fillRect(ARect:ZRect); virtual; abstract;

    procedure drawRect(ARect:ZRect; AColor:ZColor); virtual; abstract;
    procedure fillRect(ARect:ZRect; AColor:ZColor); virtual; abstract;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------


//----------------------------------------------------------------------
end.

