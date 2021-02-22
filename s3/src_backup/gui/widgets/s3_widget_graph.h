//#ifndef s3_widget_graph_included
//#define s3_widget_graph_included
////----------------------------------------------------------------------
//
//#include "common/s3_list.h"
//#include "gui/s3_widget.h"
//
////----------------------------------------------------------------------
//
//#define PIN_DISTANCE 2
//#define PIN_WIDTH    6
//#define PIN_HEIGHT   4
//
////----------------------------------------------------------------------
//
//enum S3_EPinFlags {
//  s3_pf_input   = 0,
//  s3_pf_output  = 1,
//  s3_pt_data    = 0,
//  s3_pt_signal  = 2,
//  s3_pf_const   = 0,
//  s3_pf_static  = 4,
//  s3_pr_dynamic = 8
//};
//
////----------------------------------------------------------------------
//
//class S3_Graph_Module
//: public S3_ListNode {
//  private:
//    const char* MName;
//    uint32      MFlags;
//    uint32      MNumInputs;
//    uint32      MNumOutputs;
//    uint32*     MOnputFlags;
//    uint32*     MOutputFlags;
//    S3_Rect     MRect;
//};
//
////----------------------------------------------------------------------
//
//class S3_Graph_Wire
//: public S3_ListNode {
//  private:
//    uint32            MFlags;
//    S3_Graph_Module*  MInputModule;
//    uint32            MInputPin;
//    S3_Graph_Module*  MOutputModule;
//    uint32            MOutputPin;
//    S3_Point          MSrcPos;
//    S3_Point          MDstPos;
//};
//
////----------------------------------------------------------------------
////
////----------------------------------------------------------------------
//
//class S3_Widget_Graph
//: public S3_Widget {
//  private:
//    S3_List MModules;
//    S3_List MWires;
//};
//
////----------------------------------------------------------------------
//
//#undef PIN_DISTANCE
//#undef PIN_WIDTH
//#undef PIN_HEIGHT
//
////----------------------------------------------------------------------
//#endif
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//#if 0
//
//{$include kode.inc}
//unit kode_widget_graph;
//
////----------------------------------------------------------------------
//interface
////----------------------------------------------------------------------
//
//uses
//  kode_canvas,
//  kode_color,
//  kode_flags,
//  kode_list,
//  kode_module,
//  kode_rect,
//  kode_widget;
//
//const
//  MODULE_WIDTH  = 64;
//  MODULE_HEIGHT = 32;
//  PIN_WIDTH     = 7;
//  PIN_HEIGHT    = 7;
//  PIN_XDIST     = 8;
//  PIN_YDIST     = 8;
//  BASE_HEIGHT   = MODULE_HEIGHT - (PIN_HEIGHT*2) - 2;
//
//type
//
//  KWidget_Graph = class(KWidget)
//  private
//    FGraph            : KGraph;
//    FPaintCanvas      : KCanvas;
//    FPaintRect        : KRect;
//    FPaintMode        : LongWord;
//  protected
//    FDraggingModules  : Boolean;
//    FDraggingWire     : Boolean;
//    FDraggingSelect   : Boolean;
//    { hovering }
//    FHoverModule      : KModule;
//    FHoverInput       : LongInt;
//    FHoverOutput      : LongInt;
//    FMousePrevX       : longint;
//    FMousePrevY       : longint;
//    { wire }
//    FDragWireX1       : LongInt;
//    FDragWireY1       : LongInt;
//    FDragWireX2       : LongInt;
//    FDragWireY2       : LongInt;
//    FDragWireModule   : KModule;
//    FDragWireInput    : LongInt;
//    FDragWireOutput   : LongInt;
//    { selection }
//    FDragSelectX1     : LongInt;
//    FDragSelectY1     : LongInt;
//    FDragSelectX2     : LongInt;
//    FDragSelectY2     : LongInt;
//  public
//    constructor create(ARect:KRect; AGraph:KGraph; AAlignment:LongWord=kwa_none);
//    destructor  destroy; override;
//  public
//    function    hasSelection : Boolean;
//    procedure   clearSelection;
//    procedure   selectModule(AModule:KModule);
//    procedure   deselectModule(AModule:KModule);
//    procedure   toggleModule(AModule:KModule);
//    function    isSelected(AModule:KModule) : Boolean;
//    procedure   selectModules(x1,y1,x2,y2:LongInt);
//    procedure   moveSelectedModules(AXdelta,AYdelta:longint);
//  public
//    procedure   startDragInput(AModule:KModule; APin:LongInt);
//    procedure   startDragOutput(AModule:KModule; APin:LongInt);
//    procedure   startDragSelect(AXpos,AYpos:LongInt);
//  public
//    procedure   drawModule(AModule:KModule);
//    procedure   drawWire(AWire:KWire);
//    procedure   drawModules;
//    procedure   drawWires;
//  public
//    procedure   on_mouseDown(AXpos,AYpos,AButton,AState : LongInt); override;
//    procedure   on_mouseMove(AXpos,AYpos,AState : LongInt); override;
//    procedure   on_mouseUp(AXpos,AYpos,AButton,AState : LongInt); override;
//    procedure   on_keyDown(AKey,AState:LongInt); override;
//    procedure   on_paint(ACanvas:KCanvas; ARect:KRect; AMode:LongWord=0); override;
//  end;
//
////----------------------------------------------------------------------
//implementation
////----------------------------------------------------------------------
//
//uses
//  {%H-}kode_const,
//  {%H-}kode_debug,
//  kode_modules,
//  kode_utils;
//
////----------
//
//constructor KWidget_Graph.create(ARect:KRect; AGraph:KGraph; AAlignment:LongWord=kwa_none);
//begin
//  inherited create(ARect,AAlignment);
//  FName             := 'KWidget_Graph';
//  FGraph            := AGraph;
//  FPaintCanvas      := nil;
//  FPaintRect        := rect0;
//  FPaintMode        := 0;
//  FHoverModule      := nil;
//  FHoverInput       := -1;
//  FHoverOutput      := -1;
//  FDraggingModules  := false;
//  FDraggingWire     := false;
//  FDraggingSelect   := false;
//  FDragWireX1       := 0;
//  FDragWireY1       := 0;
//  FDragWireX2       := 0;
//  FDragWireY2       := 0;
//  FDragWireModule   := nil;
//  FDragWireInput    := -1;
//  FDragWireOutput   := -1;
//  FDragSelectX1     := 0;
//  FDragSelectY1     := 0;
//  FDragSelectX2     := 0;
//  FDragSelectY2     := 0;
//  FMousePrevX       := 0;
//  FMousePrevY       := 0;
//  clearSelection;
//  //setFlag(kwf_clip);
//end;
//
////----------
//
//destructor KWidget_Graph.destroy;
//begin
//  inherited;
//end;
//
////--------------------------------------------------
//// selection
////--------------------------------------------------
//
//function KWidget_Graph.hasSelection : Boolean;
//var
//  node    : KListNode;
//  module  : KModule;
//begin
//  node := FGraph.modules.head;
//  while Assigned(node) do
//  begin
//    module := node as KModule;
//    if module.selected then exit(true);
//    node := node.next;
//  end;
//  result := false;
//end;
//
////----------
//
//procedure KWidget_Graph.clearSelection;
//var
//  node    : KListNode;
//  module  : KModule;
//begin
//  node := FGraph.modules.head;
//  while Assigned(node) do
//  begin
//    module := node as KModule;
//    module.selected := false;
//    node := node.next;
//  end;
//end;
//
////----------
//
//procedure KWidget_Graph.selectModule(AModule:KModule);
//begin
//  AModule.selected := true;
//end;
//
////----------
//
//procedure KWidget_Graph.deselectModule(AModule:KModule);
//begin
//  AModule.selected := false;
//end;
//
////----------
//
//procedure KWidget_Graph.toggleModule(AModule:KModule);
//begin
//  AModule.selected := not AModule.selected;
//end;
//
////----------
//
//function KWidget_Graph.isSelected(AModule:KModule) : Boolean;
//begin
//  result := AModule.selected;
//end;
//
////----------
//
//procedure KWidget_Graph.selectModules(x1,y1,x2,y2:LongInt);
//var
//  temp : longint;
//  module : KModule;
//  node : KListNode;
//begin
//  x1 -= FRect.x;
//  y1 -= FRect.y;
//  x2 -= FRect.x;
//  y2 -= FRect.y;
//  if x1 > x2 then
//  begin
//    temp := x1;
//    x1 := x2;
//    x2 := temp;
//  end;
//  if y1 > y2 then
//  begin
//    temp := y1;
//    y1 := y2;
//    y2 := temp;
//  end;
//  node := FGraph.modules.head;
//  while Assigned(node) do
//  begin
//    module := node as KModule;
//    if  ( module.xpos                <= x2)
//    and ((module.xpos+MODULE_WIDTH)  >= x1)
//    and ( module.ypos                <= y2)
//    and ((module.ypos+MODULE_HEIGHT) >= y1) then
//    begin
//      module.selected := true;
//    end;
//    node := node.next;
//  end; // while
//end;
//
////----------
//
//procedure KWidget_Graph.moveSelectedModules(AXdelta,AYdelta:longint);
//var
//  node : KListNode;
//  module : KModule;
//begin
//  node := FGraph.modules.head;
//  while Assigned(node) do
//  begin
//    module := node as KModule;
//    if module.selected then
//    begin
//      module.xpos := module.xpos + AXdelta;
//      module.ypos := module.ypos + AYdelta;
//    end;
//    node := node.next;
//  end;
//end;
//
////--------------------------------------------------
//// dragging
////--------------------------------------------------
//
//procedure KWidget_Graph.startDragInput(AModule:KModule; APin:LongInt);
//begin
//  FDraggingWire    := true;
//  FDragWireInput   := APin;
//  FDragWireOutput  := -1;
//  FDragWireModule  := AModule;
//  FDragWireX1   := FRect.x + AModule.xpos + (APin * PIN_XDIST) + (PIN_WIDTH div 2);
//  FDragWireY1   := FRect.y + AModule.ypos + (PIN_HEIGHT div 2);
//  FDragWireX2   := FDragWireX1;
//  FDragWireY2   := FDragWireY1;
//end;
//
////----------
//
//procedure KWidget_Graph.startDragOutput(AModule:KModule; APin:LongInt);
//begin
//  FDraggingWire := true;
//  FDragWireInput   := -1;
//  FDragWireOutput  := APin;
//  FDragWireModule  := AModule;
//  FDragWireX1   := FRect.x + AModule.xpos + (APin * PIN_XDIST) + (PIN_WIDTH div 2);
//  FDragWireY1   := FRect.y + AModule.ypos + (MODULE_HEIGHT-1) - (PIN_HEIGHT div 2);
//  FDragWireX2   := FDragWireX1;
//  FDragWireY2   := FDragWireY1;
//end;
//
////----------
//
//procedure KWidget_Graph.startDragSelect(AXpos,AYpos:LongInt);
//begin
//  FDraggingSelect := true;
//  FDragSelectX1   := AXpos;
//  FDragSelectY1   := AYpos;
//  FDragSelectX2   := AXpos;
//  FDragSelectY2   := AYpos;
//end;
//
////--------------------------------------------------
//// draw
////--------------------------------------------------
//
//procedure KWidget_Graph.drawModule(AModule:KModule);
//var
//  x1,y1,x2,y2 : LongInt;
//  i : LongInt;
//begin
//  x1 := FRect.x + AModule.xpos;
//  y1 := FRect.y + AModule.ypos;
//  x2 := x1 + MODULE_WIDTH - 1;
//  y2 := y1 + MODULE_HEIGHT - 1;
//
//  { background }
//  if AModule.selected then FPaintCanvas.setFillColor(KDarkGrey)
//  else FPaintCanvas.setFillColor(KLightGrey);
//  FPaintCanvas.fillRect( x1, y1+PIN_YDIST, x2, y2-PIN_YDIST );
//
//  { inputs }
//  if AModule.numInputs > 0 then
//  begin
//    for i := 0 to AModule.numInputs-1 do
//    begin
//      { fill }
//      if AModule.input[i]^.typ = kpt_signal then FPaintCanvas.setFillColor(KLightGrey)
//      else FPaintCanvas.setFillColor(KDarkGrey);
//      FPaintCanvas.fillRect( x1+(i*PIN_XDIST), y1, x1+(i*PIN_XDIST)+PIN_WIDTH-1, y1+PIN_HEIGHT-1 );
//      { border }
//      if (AModule = FHoverModule) and (i = FHoverInput) and (not FDraggingModules) then
//      begin
//        if AModule.input[i]^.typ = kpt_signal then FPaintCanvas.setDrawColor(KBlack)
//        else if AModule.input[i]^.typ = kpt_data then FPaintCanvas.setDrawColor(KWhite);
//        FPaintCanvas.drawRect( x1+(i*PIN_XDIST), y1, x1+(i*PIN_XDIST)+PIN_WIDTH-1, y1+PIN_HEIGHT-1 );
//      end;
//    end;
//  end;
//
//  { outputs }
//  if AModule.numOutputs > 0 then
//  begin
//    for i := 0 to AModule.numOutputs-1 do
//    begin
//      { fill }
//      if AModule.output[i]^.typ = kpt_signal then FPaintCanvas.setFillColor(KLightGrey)
//      else FPaintCanvas.setFillColor(KDarkGrey);
//      FPaintCanvas.fillRect(x1+(i*PIN_XDIST),y2-PIN_HEIGHT+1,x1+(i*PIN_XDIST)+PIN_WIDTH-1,y2);
//      { border }
//      if (AModule = FHoverModule) and (i = FHoverOutput) and (not FDraggingModules) then
//      begin
//        if AModule.output[i]^.typ = kpt_signal then FPaintCanvas.setDrawColor(KBlack)
//        else if AModule.output[i]^.typ = kpt_data then FPaintCanvas.setDrawColor(KWhite);
//        FPaintCanvas.drawRect(x1+(i*PIN_XDIST),y2-PIN_HEIGHT+1,x1+(i*PIN_XDIST)+PIN_WIDTH-1,y2);
//      end;
//    end;
//  end;
//
//  { name }
//  if AModule.selected then FPaintCanvas.setTextColor(KLightGrey)
//  else FPaintCanvas.setTextColor(KDarkGrey);
//  FPaintCanvas.drawText(x1,y1,x2,y2,AModule.name,kta_center);
//
//  { border }
//  {if AModule = FHoverModule then
//  begin
//    FPaintCanvas.setDrawColor(KLightRed);
//    FPaintCanvas.drawRect( x1, y1+PIN_YDIST, x2, y2-PIN_YDIST );
//  end;}
//
//end;
//
////----------
//
//procedure KWidget_Graph.drawWire(AWire:KWire);
//var
//  x1,y1,x2,y2 : LongInt;
//begin
//  x1 := FRect.x + AWire.outModule.xpos + 2 + (AWire.outPin * PIN_XDIST);
//  y1 := FRect.y + AWire.outModule.ypos + 31 - 2;
//  x2 := FRect.x + AWire.inModule.xpos + 2 + (AWire.InPin * PIN_XDIST);
//  y2 := FRect.y + AWire.inModule.ypos + 2;
//  if AWire.outModule.output[AWire.outPin]^.typ = kpt_signal then FPaintCanvas.setDrawColor(KLightGrey)
//  else FPaintCanvas.setDrawColor(KDarkGrey);
//  FPaintCanvas.drawLine(x1,y1,x2,y2);
//end;
//
////----------
//
//procedure KWidget_Graph.drawModules;
//var
//  node : KListNode;
//  module : KModule;
//begin
//  node := FGraph.modules.head;
//  while Assigned(node) do
//  begin
//    module := node as KModule;
//    drawModule(module);
//    node := node.next;
//  end;
//end;
//
////----------
//
//procedure KWidget_Graph.drawWires;
//var
//  wire : KWire;
//begin
//  wire := FGraph.wires.head as KWire;
//  while Assigned(wire) do
//  begin
//    drawWire(wire);
//    wire := wire.next as KWire;
//  end;
//end;
//
////----------------------------------------------------------------------
//// mouse down
////----------------------------------------------------------------------
//
//{
//  AXpos, AYpos = 'world' coordinates
//  x,y = local coords (graph)
//}
//
//procedure KWidget_Graph.on_mouseDown(AXpos,AYpos,AButton,AState : LongInt);
//var
//  node    : KListNode;
//  module  : KModule;
//  pin     : LongInt;
//  changed : boolean;
//  x,y : longint;
//begin
//  x := AXpos - FRect.x;
//  y := AYpos - FRect.y;
//  changed := false;
//  if AButton = kmb_left then
//  begin
//    FMousePrevX := AXpos;
//    FMousePrevY := AYpos;
//
//    module := nil;
//    node := FGraph.modules.tail;
//    while Assigned(node) do
//    begin
//      module := node as KModule;
//
//      if module.inside({AXpos,AYpos}x,y) then
//      begin
//
//        { inputs }
//        pin := module.insideInput({AXpos,AYpos}x,y);
//        if (pin >= 0) and not FDraggingModules then
//        begin
//          startDragInput(module,pin);
//          break;
//        end else
//
//        { base }
//        if module.insideBase({AXpos,AYpos}x,y) then
//        begin
//          if KHasFlag(AState,kmb_ctrl) then
//          begin
//            toggleModule(module);
//            changed := true;
//          end
//          else
//          begin
//            if not module.selected then clearSelection;
//            selectModule(module);
//            FDraggingModules := true;
//            changed := true;
//          end;
//          break;
//        end else
//
//        { outputs }
//        pin := module.insideOutput({AXpos,AYpos}x,y);
//        if (pin >= 0) and not FDraggingModules then
//        begin
//          startDragOutput(module,pin);
//          break;
//        end;
//
//      end; // inside module
//      node := node.prev;
//    end; // while
//
//    if not Assigned(node) then
//    begin
//
//      if hasSelection then
//      begin
//        if not KHasFlag(AState,kmb_ctrl) then
//        begin
//          clearSelection;
//          changed := true;
//        end;
//      end;
//
//      if not FDraggingSelect then
//      begin
//        if not KHasFlag(AState,kmb_ctrl) then
//          clearSelection;
//        startDragSelect(AXpos,AYpos);
//      end;
//
//    end; // node
//  end;
//
//  if changed then do_redraw(self,FRect);
//  inherited;
//end;
//
////----------------------------------------------------------------------
//// mouse move
////----------------------------------------------------------------------
//
//procedure KWidget_Graph.on_mouseMove(AXpos,AYpos,AState : LongInt);
//var
//  node    : KListNode;
//  module  : KModule;
//  pin     : LongInt;
//  changed : boolean;
//  x,y : longint;
//begin
//  x := AXpos - FRect.x;
//  y := AYpos - FRect.y;
//  changed := false;
//  node := FGraph.modules.tail as KModule;
//  while Assigned(node) do
//  begin
//    module := node as KModule;
//
//    if module.inside(x,y{AXpos,AYpos}) then
//    begin
//
//      { inputs }
//      pin := module.insideInput(x,y{AXpos,AYpos});
//      if pin >= 0 then
//      begin
//        FHoverModule := module; // used when drawing modules
//        if (FHoverInput <> pin) and not FDraggingSelect then
//        begin
//          FHoverInput := pin;
//          FHoverOutput := -1;
//          changed := true;
//        end;
//        break;
//      end else
//
//      { base }
//      if module.insideBase(x,y{AXpos,AYpos}) then
//      begin
//        FHoverModule := module;
//        if FHoverInput <> -1 then
//        begin
//          FHoverInput := -1;
//          changed := true;
//        end;
//        if FHoveroutput <> -1 then
//        begin
//          FHoverOutput := -1;
//          changed := true;
//        end;
//        break;
//      end else
//
//      { outputs }
//      pin := module.insideOutput(x,y{AXpos,AYpos});
//      if pin >= 0 then
//      begin
//        FHoverModule := module;
//        if (FHoverOutput <> pin) and not FDraggingSelect then
//        begin
//          FHoverInput := -1;
//          FHoverOutput := pin;
//          changed := true;
//        end;
//        break;
//      end;
//
//    end;
//    node := node.prev;
//  end; // while
//
//  if not Assigned(node) then
//  begin
//    FHoverModule := nil;
//    if FHoverInput <> -1 then
//    begin
//      FHoverInput := -1;
//      changed := true;
//    end;
//    if FHoveroutput <> -1 then
//    begin
//      FHoverOutput := -1;
//      changed := true;
//    end;
//  end;
//
//  if FDraggingWire then
//  begin
//    FDragWireX2 := AXpos;
//    FDragWireY2 := AYpos;
//    changed := true;
//  end else
//
//  if FDraggingModules then
//  begin
//    moveSelectedModules(AXpos-FMousePrevX,AYpos-FMousePrevY);
//    changed := true;
//  end else
//
//  if FDraggingSelect then
//  begin
//    FDragSelectX2 := AXpos;
//    FDragSelectY2 := AYpos;
//    selectModules(FDragSelectX1,FDragSelectY1,FDragSelectX2,FDragSelectY2);
//    changed := true;
//  end;
//
//  if changed then do_redraw(self,FRect);
//  FMousePrevX := AXpos;
//  FMousePrevY := AYpos;
//  inherited;
//end;
//
////----------------------------------------------------------------------
//// mouse up
////----------------------------------------------------------------------
//
//procedure KWidget_Graph.on_mouseUp(AXpos,AYpos,AButton,AState : LongInt);
//var
//  wire : KWire;
//  changed : boolean;
//begin
//  //AXpos -= FRect.x;
//  //AYpos -= FRect.y;
//  changed := false;
//  if AButton = kmb_left then
//  begin
//
//    if FDraggingModules then
//    begin;
//      FDraggingModules := false;
//      changed := true;
//    end;
//
//    if FDraggingWire then
//    begin
//      FDraggingWire := false;
//      changed := true;
//
//      if Assigned(FDragWireModule) then
//      begin
//
//        if FDragWireInput >= 0 then
//        begin
//          if FHoverOutput >= 0 then
//          begin
//            wire := FGraph.findWire(FDragWireModule,FDragWireInput,FHoverModule,FHoverOutput);
//            if Assigned(wire) then
//            begin
//              FGraph.removeWire(wire);
//              wire.destroy;
//            end
//            else
//            begin
//              FGraph.addWire( KWire.create( FDragWireModule,FDragWireInput, FHoverModule, FHoverOutput ) ); // in -> out
//            end;
//          end
//          else
//          begin
//            if FDragWireInput <> FHoverInput then
//            FGraph.removeInputWires(FDragWireModule,FDragWireInput);
//          end;
//
//        end; // input
//
//        if FDragWireOutput >= 0 then
//        begin
//          if FHoverInput >= 0 then
//          begin
//            wire := FGraph.findWire(FHoverModule,FHoverInput,FDragWireModule,FDragWireOutput);
//            if Assigned(wire) then
//            begin
//              FGraph.removeWire(wire);
//              wire.destroy;
//            end
//            else
//            begin
//              FGraph.addWire(KWire.create(FHoverModule,FHoverInput,FDragWireModule,FDragWireOutput)); // in -> out
//            end;
//          end // output
//          else
//          begin
//            if FDragWireOutput <> FHoverOutput then
//            FGraph.removeOutputWires(FDragWireModule,FDragWireOutput);
//          end;
//        end;
//
//      end; // dragWireModule
//    end; // dragging wire
//
//    if FDraggingSelect then
//    begin
//      FDraggingSelect := false;
//      changed := true;
//    end;
//
//  end;
//
//  if changed then do_redraw(self,FRect);
//  inherited;
//end;
//
////----------------------------------------------------------------------
//// keyboard
////----------------------------------------------------------------------
//
//procedure KWidget_Graph.on_keyDown(AKey,AState:LongInt);
//begin
//  KTrace(['key:',AKey,' state:',AState,KODE_CR]);
//  inherited;
//end;
//
////----------------------------------------------------------------------
//// paint
////----------------------------------------------------------------------
//
//procedure KWidget_Graph.on_paint(ACanvas:KCanvas; ARect:KRect; AMode:LongWord=0);
//begin
//  FPaintCanvas  := ACanvas;
//  FPaintRect    := ARect;
//  FPaintMode    := AMode;
//
//  //ACanvas.setDrawColor(KLightGrey);
//  //ACanvas.drawRect(FRect.x,FRect.y,FRect.x2,FRect.y2);
//
//  ACanvas.pushClip(FRect);
//  drawModules;
//  drawWires;
//  if FDraggingWire then
//  begin
//    ACanvas.setDrawColor(KWhite);
//    ACanvas.drawLine(FDragWireX1,FDragWireY1,FDragWireX2,FDragWireY2);
//  end;
//  if FDraggingSelect then
//  begin
//    ACanvas.setDrawColor(KLightYellow);
//    ACanvas.drawRect(FDragSelectX1,FDragSelectY1,FDragSelectX2,FDragSelectY2);
//  end;
//  ACanvas.popClip;
//  inherited;
//end;
//
////----------------------------------------------------------------------
//end.
//
//
//#endif // 0
