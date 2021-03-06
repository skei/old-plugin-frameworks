#ifndef kode_modular_widget_included
#define kode_modular_widget_included
//----------------------------------------------------------------------

#include "base/kode_list.h"
#include "gui/widgets/kode_border_widget.h"

//----------------------------------------------------------------------

#define MODULE_WIDTH    96
#define MODULE_HEIGHT   32
#define PIN_WIDTH       7
#define PIN_HEIGHT      5
#define PIN_XDIST       9
#define PIN_YDIST       7
#define MAX_PINS        16
#define BASE_HEIGHT     (MODULE_HEIGHT-(PIN_HEIGHT*2)-2)

//----------------------------------------------------------------------

struct  KODE_GraphPin;
class   KODE_GraphWire;
class   KODE_GraphModule;
struct  KODE_Graph;

//----------

enum KODE_PinType {
  kpt_data    = 0,
  kpt_signal  = 1,
};

//----------

class KODE_GraphWire : public KODE_ListNode {
  public:
    KODE_GraphModule* inModule  = KODE_NULL;
    int32_t           inPin     = 0;
    KODE_GraphModule* outModule = KODE_NULL;
    int32_t           outPin    = 0;
  public:
    KODE_GraphWire(KODE_GraphModule* AInModule, int32_t AInPin, KODE_GraphModule* AOutModule, int32_t AOutPin) {
      inModule  = AInModule;
      inPin     = AInPin;
      outModule = AOutModule;
      outPin    = AOutPin;
    }
};

//----------

class KODE_GraphModule : public KODE_ListNode {
  public:
    const char* name        = "module";
    int32_t     xpos        = 0;
    int32_t     ypos        = 0;
    int32_t     numInputs   = 0;
    int32_t     numOutputs  = 0;
    uint32_t    inputs[MAX_PINS]  = {0};
    uint32_t    outputs[MAX_PINS] = {0};
    bool        selected    = false;
    bool        hovering    = false;

  public:

    bool inside(int32_t x, int32_t y) {
      if ( (x >= xpos) && (x <  xpos + MODULE_WIDTH)
        && (y >= ypos) && (y <  ypos + MODULE_HEIGHT)) return true;
      return false;
    }

    bool insideBase(int32_t x, int32_t y) {
      if (y < (ypos + PIN_HEIGHT + BASE_HEIGHT)) return true;
      return false;
    }

    int32_t insideInput(int32_t x, int32_t y) {
      if (y < (ypos + PIN_HEIGHT)) {
        int32_t pin = (x-xpos) / PIN_WIDTH;
        if (pin < numInputs) return pin;
      }
      return -1;
    }

    int32_t insideOutput(int32_t x, int32_t y) {
      int32_t pin = (x-xpos) / PIN_WIDTH;
      if (pin < numOutputs) return pin;
      return -1;
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_GraphWidget
: public KODE_BorderWidget {

private:
  
  //KODE_Graph*       MGraph            = KODE_NULL;
    KODE_List         MModules;
    KODE_List         MWires;
    KODE_Painter*     MPainter          = KODE_NULL;
    KODE_Rect         MPaintRect        = KODE_Rect(0);
    
protected:

    KODE_Color MModuleColor             = KODE_LightGrey;
    KODE_Color MSelectedModuleColor     = KODE_DarkGrey;
    
    KODE_Color MSignalPinColor          = KODE_LightGrey;
    KODE_Color MDataPinColor            = KODE_DarkGrey;
    KODE_Color MSignalPinHoverColor     = KODE_Black;
    KODE_Color MDataPinHoverColor       = KODE_White;
    
    KODE_Color MModuleNameColor         = KODE_DarkGrey;
    KODE_Color MSelectedModuleNameColor = KODE_LightGrey;
    
    KODE_Color MSignalWireColor          = KODE_LightGrey;
    KODE_Color MDataWireColor            = KODE_DarkGrey;

  
    bool              MDraggingModules  = false;
    bool              MDraggingWire     = false;
    bool              MDraggingSelect   = false;
    // hovering
    KODE_GraphModule* MHoverModule      = KODE_NULL;
    int32_t           MHoverInput       = -1;
    int32_t           MHoverOutput      = -1;
    int32_t           MMousePrevX       = 0;
    int32_t           MMousePrevY       = 0;
    // wire
    int32_t           MDragWireX1       = 0;
    int32_t           MDragWireY1       = 0;
    int32_t           MDragWireX2       = 0;
    int32_t           MDragWireY2       = 0;
    KODE_GraphModule* MDragWireModule   = KODE_NULL;
    int32_t           MDragWireInput    = -1;
    int32_t           MDragWireOutput   = -1;
    // selection
    int32_t           MDragSelectX1     = 0;
    int32_t           MDragSelectY1     = 0;
    int32_t           MDragSelectX2     = 0;
    int32_t           MDragSelectY2     = 0;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    KODE_GraphWidget(KODE_Rect ARect)
    : KODE_BorderWidget(ARect) {
      MName  = "KWidget_Graph";
      clearSelection();
      setFlag(KODE_WIDGET_HOVER);
      //setFlag(KODE_WIDGET_CLIP);
      setBackgroundColor( KODE_Color(0.55f) );
    }

    //----------

    virtual ~KODE_GraphWidget() {
      #ifndef KODE_NO_AUTODELETE
        deleteModules();
        deleteWires();
      #endif
    }

  //----------------------------------------
  // modules
  //----------------------------------------

  public:

    //void appendModule(KODE_GraphModule* AModule) {
    //  MModules.append(AModule);
    //}

    //----------

    void deleteModules(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_ListNode* next = node->next();
        KODE_Delete node;
        node = next;
      }
    }

    //----------

    void addModule(KODE_GraphModule* AModule, int32_t AXpos, int32_t AYpos, const char* AName="") {
      AModule->xpos = AXpos;
      AModule->ypos = AYpos;
      AModule->selected = false;
      AModule->hovering = false;
      AModule->name = AName;
      MModules.insertTail(AModule);
    }

    //----------

    void removeModule(KODE_GraphModule* AModule) {
      MModules.removeNode(AModule);
    }

    //----------

    void removeSelectedModules(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        node = node->next();
        if (module->selected) {
          removeWires(module);
          removeModule(module);
          //delete module; // module.destroy;
        }
      }
    }

  //----------------------------------------
  // wires
  //----------------------------------------

  public:

    void appendWire(KODE_GraphWire* AWire) {
      MWires.insertTail(AWire);
    }

    //----------

    void deleteWires(void) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_ListNode* next = node->next();
        KODE_Delete node;
        node = next;
      }

    }
    
    //----------

    void addWire(KODE_GraphWire* AWire) {
      MWires.insertTail(AWire);
    }

    //----------

    void removeWire(KODE_GraphWire* AWire) {
      MWires.removeNode(AWire);
    }

    //----------

    // remove all input wires connected to a specific pin on a module

    void removeInputWires(KODE_GraphModule* AModule, int32_t APin) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        node = node->next();
        if ((AModule == wire->inModule) && (APin == wire->inPin))
        /*or ( (AModule = wire.outModule) and (APin = wire.outPin) )*/ {
          removeWire(wire);
          KODE_Delete wire; // wire.Destroy;
        }
        //node = node.next;
      }
    }

    //----------

    // remove all input wires connected to a specific pin on a module

    void removeOutputWires(KODE_GraphModule* AModule, int32_t APin) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        node = node->next();
        if ((AModule == wire->outModule) && (APin == wire->outPin)  )
        /*or ( (AModule = wire.outModule) and (APin = wire.outPin) )*/ {
          removeWire(wire);
          KODE_Delete wire;//wire.Destroy;
        }
        //node = node.next;
      }
    }

    //----------

    // remove all wires connected to a module

    void removeWires(KODE_GraphModule* AModule) {
      //node : KListNode;
      //ire : KODE_GraphWire;
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        node = node->next();
        if ((AModule == wire->inModule) || (AModule == wire->outModule)) {
          removeWire(wire);
          KODE_Delete wire;//wire.Destroy;
        }
        //node = node.next;
      }
    }

    //----------

    // find wire, input or output

    KODE_GraphWire* findWire(KODE_GraphModule* AModule, int32_t APin) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        if ( ((AModule == wire->inModule ) && (APin == wire->inPin ))
          || ((AModule == wire->outModule) && (APin == wire->outPin)) ) return wire;//System.exit(wire);
        node = node->next();
      }
      return KODE_NULL;
    }

    //----------

    KODE_GraphWire* findWire(KODE_GraphModule* AInModule, int32_t AInPin, KODE_GraphModule* AOutModule, int32_t AOutPin) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        if ( (AInModule  == wire->inModule)
          && (AInPin     == wire->inPin)
          && (AOutModule == wire->outModule)
          && (AOutPin    == wire->outPin) ) return wire;
        node = node->next();
      }
      return KODE_NULL;
    }

  //----------------------------------------
  // selection
  //----------------------------------------

  public:

    bool hasSelection(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        if (module->selected) return true;
        node = node->next();
      }
      return false;
    }

    //----------

    void clearSelection(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        module->selected = false;
        node = node->next();
      }
    }

    //----------

    void selectModule(KODE_GraphModule* AModule) {
      AModule->selected = true;
    }

    //----------

    void deselectModule(KODE_GraphModule* AModule) {
      AModule->selected = false;
    }

    //----------

    void toggleModule(KODE_GraphModule* AModule) {
      AModule->selected = AModule->selected ? false : true;
    }

    //----------

    bool isSelected(KODE_GraphModule* AModule) {
      return AModule->selected;
    }

    //----------

    void selectModules(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
      x1 -= MRect.x;
      y1 -= MRect.y;
      x2 -= MRect.x;
      y2 -= MRect.y;
      int32_t temp;
      if (x1 > x2) {
        temp = x1;
        x1 = x2;
        x2 = temp;
      }
      if (y1 > y2) {
        temp = y1;
        y1 = y2;
        y2 = temp;
      }
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        if ( ( module->xpos                <= x2)
          && ((module->xpos+MODULE_WIDTH)  >= x1)
          && ( module->ypos                <= y2)
          && ((module->ypos+MODULE_HEIGHT) >= y1)) {
          module->selected = true;
        }
        node = node->next();
      } // while
    }


    //----------

    void moveSelectedModules(int32_t AXdelta, int32_t AYdelta) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        if (module->selected) {
          module->xpos = module->xpos + AXdelta;
          module->ypos = module->ypos + AYdelta;
        }
        node = node->next();
      }
    }

  //----------------------------------------
  // dragging
  //----------------------------------------

  public:

    void startDragInput(KODE_GraphModule* AModule, int32_t APin) {
      MDraggingWire   = true;
      MDragWireInput  = APin;
      MDragWireOutput = -1;
      MDragWireModule = AModule;
      MDragWireX1     = MRect.x + AModule->xpos + (APin * PIN_XDIST) + (PIN_WIDTH / 2);
      MDragWireY1     = MRect.y + AModule->ypos + (PIN_HEIGHT / 2);
      MDragWireX2     = MDragWireX1;
      MDragWireY2     = MDragWireY1;
    }

    //----------

    void startDragOutput(KODE_GraphModule* AModule, int32_t APin) {
      MDraggingWire   = true;
      MDragWireInput  = -1;
      MDragWireOutput = APin;
      MDragWireModule = AModule;
      MDragWireX1     = MRect.x + AModule->xpos + (APin * PIN_XDIST) + (PIN_WIDTH / 2);
      MDragWireY1     = MRect.y + AModule->ypos + (MODULE_HEIGHT-1) - (PIN_HEIGHT / 2);
      MDragWireX2     = MDragWireX1;
      MDragWireY2     = MDragWireY1;
    }

    //----------

    void startDragSelect(int32_t AXpos, int32_t AYpos) {
      MDraggingSelect = true;
      MDragSelectX1   = AXpos;
      MDragSelectY1   = AYpos;
      MDragSelectX2   = AXpos;
      MDragSelectY2   = AYpos;
    }

  //----------------------------------------
  // draw
  //----------------------------------------

  public:

    void drawModule(KODE_GraphModule* AModule) {
      int32_t x1 = MRect.x + AModule->xpos;
      int32_t y1 = MRect.y + AModule->ypos;
      int32_t x2 = x1 + MODULE_WIDTH - 1;
      int32_t y2 = y1 + MODULE_HEIGHT - 1;

      // background

      if (AModule->selected) MPainter->setFillColor(MSelectedModuleColor);
      else MPainter->setFillColor(MModuleColor);
      MPainter->fillRectangle( x1, y1+PIN_YDIST, x2, y2-PIN_YDIST );

      // inputs

      for (int32_t i=0; i<AModule->numInputs; i++) {
        // fill
        if (AModule->inputs[i] == kpt_signal) MPainter->setFillColor(MSignalPinColor);
        else MPainter->setFillColor(MDataPinColor);
        MPainter->fillRectangle( x1+(i*PIN_XDIST), y1, x1+(i*PIN_XDIST)+PIN_WIDTH-1, y1+PIN_HEIGHT-1 );
        // border
        if ((AModule == MHoverModule) && (i == MHoverInput) && (!MDraggingModules)) {
          if (AModule->inputs[i] == kpt_signal) MPainter->setDrawColor(MSignalPinHoverColor);
          else if (AModule->inputs[i] == kpt_data) MPainter->setDrawColor(MDataPinHoverColor);
          MPainter->drawRectangle( x1+(i*PIN_XDIST), y1, x1+(i*PIN_XDIST)+PIN_WIDTH-1, y1+PIN_HEIGHT-1 );
        }
      }

      // outputs

      for (int32_t i=0; i<AModule->numOutputs; i++) {
        // fill
        if (AModule->outputs[i] == kpt_signal) MPainter->setFillColor(MSignalPinColor);
        else MPainter->setFillColor(MDataPinColor);
        MPainter->fillRectangle(x1+(i*PIN_XDIST),y2-PIN_HEIGHT+1,x1+(i*PIN_XDIST)+PIN_WIDTH-1,y2);
        // border
        if ((AModule == MHoverModule) && (i == MHoverOutput) && (!MDraggingModules)) {
          if (AModule->outputs[i] == kpt_signal) MPainter->setDrawColor(MSignalPinHoverColor);
          else if (AModule->outputs[i] == kpt_data) MPainter->setDrawColor(MDataPinHoverColor);
          MPainter->drawRectangle(x1+(i*PIN_XDIST),y2-PIN_HEIGHT+1,x1+(i*PIN_XDIST)+PIN_WIDTH-1,y2);
        }
      }

      // name

      if (AModule->selected) MPainter->setTextColor(MSelectedModuleNameColor);
      else MPainter->setTextColor(MModuleNameColor);
      MPainter->drawText(x1,y1,x2,y2,AModule->name,KODE_TEXT_ALIGN_CENTER);

      // border

      //if (AModule == MHoverModule) {
      //  MPainter->setDrawColor(KLightRed);
      //  MPainter->drawRectangle( x1, y1+PIN_YDIST, x2, y2-PIN_YDIST );
      //}

    }

    //----------

    void drawWire(KODE_GraphWire* AWire) {
      int32_t x1 = MRect.x + AWire->outModule->xpos + 2 + (AWire->outPin * PIN_XDIST);
      int32_t y1 = MRect.y + AWire->outModule->ypos + 31 - 2;
      int32_t x2 = MRect.x + AWire->inModule->xpos + 2 + (AWire->inPin * PIN_XDIST);
      int32_t y2 = MRect.y + AWire->inModule->ypos + 2;
      if (AWire->outModule->outputs[AWire->outPin] == kpt_signal) MPainter->setDrawColor(MSignalWireColor);
      else MPainter->setDrawColor(MDataWireColor);
      MPainter->drawLine(x1,y1,x2,y2);
    }

    //----------

    void drawModules(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        drawModule(module);
        node = node->next();
      }
    }

    //----------

    void drawWires(void) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        drawWire(wire);
        node = node->next();
      }
    }

  //----------------------------------------
  // mouse down
  //----------------------------------------

  public:

    // AXpos, AYpos = 'world' coordinates
    // x,y = local coords (graph)

    void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {

      KODE_ListNode* node;
      KODE_GraphModule* module;
      int32_t pin;
      bool changed;
      int32_t x,y;

      x = AXpos - MRect.x;
      y = AYpos - MRect.y;
      changed = false;
      if (AButton == KODE_BUTTON_LEFT) {
        MMousePrevX = AXpos;
        MMousePrevY = AYpos;
        module = KODE_NULL;
        node = MModules.tail();
        while (node) {
          module = (KODE_GraphModule*)node;
          if (module->inside(x,y)) {

            // inputs

            pin = module->insideInput(x,y);
            if ((pin >= 0) && (!MDraggingModules)) {
              startDragInput(module,pin);
              break;
            } else

            // base

            if (module->insideBase(x,y)) {
              if (AState & KODE_KEY_CTRL) {
                toggleModule(module);
                changed = true;
              }
              else {
                if (!module->selected) clearSelection();
                selectModule(module);
                MDraggingModules = true;
                changed = true;
              }
              break;
            } else

            // outputs

            pin = module->insideOutput(x,y);
            if ((pin >= 0) && (!MDraggingModules)) {
              startDragOutput(module,pin);
              break;
            }

          } // inside module
          node = node->prev();
        } // while

        if (!node) {

          if (hasSelection()) {
            if (!(AState & KODE_KEY_CTRL)) {
              clearSelection();
              changed = true;
            }
          }

          if (!MDraggingSelect) {
            if (!(AState & KODE_KEY_CTRL))
              clearSelection();
            startDragSelect(AXpos,AYpos);
          }

        } // node
      }

      if (changed) do_redraw(this,MRect);
      KODE_BorderWidget::on_mouseClick(AXpos,AYpos,AButton,AState);
    }

  //----------------------------------------
  // mouse move
  //----------------------------------------

  public:

    void on_mouseMove(float AXpos, float AYpos, uint32_t AState) override {

      int32_t x = AXpos - MRect.x;
      int32_t y = AYpos - MRect.y;
      bool changed = false;
      KODE_ListNode* node = (KODE_GraphModule*)MModules.tail();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        if (module->inside(x,y)) {

          // inputs

          int32_t pin = module->insideInput(x,y);
          if (pin >= 0) {
            MHoverModule = module; // used when drawing modules
            if ((MHoverInput != pin) && (!MDraggingSelect)) {
              MHoverInput = pin;
              MHoverOutput = -1;
              changed = true;
            }
            break;
          } else

          // base

          if (module->insideBase(x,y)) {
            MHoverModule = module;
            if (MHoverInput != -1) {
              MHoverInput = -1;
              changed = true;
            }
            if (MHoverOutput != -1) {
              MHoverOutput = -1;
              changed = true;
            }
            break;
          } else

          // outputs

          pin = module->insideOutput(x,y);
          if (pin >= 0) {
            MHoverModule = module;
            if ((MHoverOutput != pin) && !MDraggingSelect) {
              MHoverInput = -1;
              MHoverOutput = pin;
              changed = true;
            }
            break;
          }

        }
        node = node->prev();
      } // while

      if (!node) {
        MHoverModule = KODE_NULL;
        if (MHoverInput != -1) {
          MHoverInput = -1;
          changed = true;
        }
        if (MHoverOutput != -1) {
          MHoverOutput = -1;
          changed = true;
        }
      }

      if (MDraggingWire) {
        MDragWireX2 = AXpos;
        MDragWireY2 = AYpos;
        changed = true;
      } else

      if (MDraggingModules) {
        moveSelectedModules(AXpos-MMousePrevX,AYpos-MMousePrevY);
        changed = true;
      } else

      if (MDraggingSelect) {
        MDragSelectX2 = AXpos;
        MDragSelectY2 = AYpos;
        selectModules(MDragSelectX1,MDragSelectY1,MDragSelectX2,MDragSelectY2);
        changed = true;
      }

      if (changed) do_redraw(this,MRect);
      MMousePrevX = AXpos;
      MMousePrevY = AYpos;

      KODE_BorderWidget::on_mouseMove(AXpos,AYpos,AState);

    }

  //----------------------------------------
  // mouse up
  //----------------------------------------

  public:

    void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {

      KODE_GraphWire* wire;
      bool changed;

      //AXpos -= FRect.x;
      //AYpos -= FRect.y;
      changed = false;
      if (AButton == KODE_BUTTON_LEFT) {

        if (MDraggingModules) {
          MDraggingModules = false;
          changed = true;
        }

        if (MDraggingWire) {
          MDraggingWire = false;
          changed = true;

          if (MDragWireModule) {

            if (MDragWireInput >= 0) {
              if (MHoverOutput >= 0) {
                wire = findWire(MDragWireModule,MDragWireInput,MHoverModule,MHoverOutput);
                if (wire) {
                  removeWire(wire);
                  KODE_Delete wire;
                }
                else {
                  addWire( KODE_New KODE_GraphWire(MDragWireModule,MDragWireInput, MHoverModule, MHoverOutput) ); // in -> out
                }
              }
              else {
                if (MDragWireInput != MHoverInput)
                removeInputWires(MDragWireModule,MDragWireInput);
              }

            } // input

            if (MDragWireOutput >= 0) {
              if (MHoverInput >= 0) {
                wire = findWire(MHoverModule,MHoverInput,MDragWireModule,MDragWireOutput);
                if (wire) {
                  removeWire(wire);
                  KODE_Delete wire;
                }
                else {
                  addWire( KODE_New KODE_GraphWire(MHoverModule,MHoverInput,MDragWireModule,MDragWireOutput) ); // in -> out
                }
              } // output
              else {
                if (MDragWireOutput != MHoverOutput)
                  removeOutputWires(MDragWireModule,MDragWireOutput);
              }
            }

          } // dragWireModule
        } // dragging wire

        if (MDraggingSelect) {
          MDraggingSelect = false;
          changed = true;
        }

      }

      if (changed) do_redraw(this,MRect);
      //inherited;
      KODE_BorderWidget::on_mouseRelease(AXpos,AYpos,AButton,AState);

    }

  //----------------------------------------
  // keyboard
  //----------------------------------------

  public:

    //void on_widget_keyPress(KWidget* AWidget,char AChar, int32_t AKey, int32_t AState) override {
    //  KWidget::on_widget_keyPress(AWidget,AChar,AKey,AState);
    //}

  //----------------------------------------
  // paint
  //----------------------------------------

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {

      KODE_BorderWidget::on_paint(APainter,ARect);

      MPainter    = APainter;
      MPaintRect  = ARect;
      //ACanvas.setDrawColor(KLightGrey);
      //ACanvas.drawRect(FRect.x,FRect.y,FRect.x2,FRect.y2);
      APainter->pushClip(MRect);
      drawModules();
      drawWires();
      if (MDraggingWire) {
        APainter->setDrawColor(KODE_White);
        APainter->drawLine(MDragWireX1,MDragWireY1,MDragWireX2,MDragWireY2);
      }
      if (MDraggingSelect) {
        APainter->setDrawColor(KODE_LightYellow);
        APainter->drawRectangle(MDragSelectX1,MDragSelectY1,MDragSelectX2,MDragSelectY2);
      }
      APainter->popClip();
    }

    //----------

};

//----------------------------------------------------------------------

#undef PIN_WIDTH
#undef PIN_HEIGHT
#undef MODULE_WIDTH
#undef MODULE_HEIGHT
#undef PIN_XDIST
#undef PIN_YDIST
#undef MAX_PINS
#undef BASE_HEIGHT

//----------------------------------------------------------------------
#endif
