
//#define KODE_GUI_CAIRO

#include "base/kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"

const char* buttonrow_txt[] = {
  "1", "2", "3", "4", "FIVE"
};

const char* buttonrow_txt2[] = {
  "A", "B", "ccc", "d", "e"
};

const char* tabs_txt[] = {
  "item1", "item2", "i3", "4", "..and 5.."
};

const char* selector_txt[] = {
  "item1", "item2", "i3", "4", "..and 5.."
};

int main(void) {
  KODE_Window* window = KODE_New KODE_Window(640,480);
  window->setTitle("KODE_SdlWindow");
  
  KODE_MenuWidget* menu = KODE_New KODE_MenuWidget();
  menu->setMenuSize(1,5);
  menu->setMenuMirror(false,true);
  menu->appendMenuItems(selector_txt,5);
  
  window->appendWidget( KODE_New KODE_ColorWidget(      KODE_Rect( 10, 10,190, 20),  KODE_Red ));
  window->appendWidget( KODE_New KODE_BorderWidget(     KODE_Rect( 10, 40,190, 20),  KODE_BrightYellow ));
  window->appendWidget( KODE_New KODE_DragValueWidget(  KODE_Rect( 10, 70,190, 20),  0.0f ));
  window->appendWidget( KODE_New KODE_SliderWidget(     KODE_Rect( 10,100,190, 20)        ));
  window->appendWidget( KODE_New KODE_SwitchWidget(     KODE_Rect( 10,130,190, 20),  0, "on", "off" ));
  window->appendWidget( KODE_New KODE_ButtonWidget(     KODE_Rect( 10,160,190, 20),  0, "on", "off" ));
  window->appendWidget( KODE_New KODE_ButtonRowWidget(  KODE_Rect( 10,190,190, 20),  5, buttonrow_txt, KODE_BUTTON_ROW_SINGLE ));
  window->appendWidget( KODE_New KODE_ButtonRowWidget(  KODE_Rect( 10,220,190, 20),  5, buttonrow_txt2, KODE_BUTTON_ROW_MULTI ));
  window->appendWidget( KODE_New KODE_MenuItemWidget(   KODE_Rect( 10,250,190, 20),  "menuitem" ));
  window->appendWidget( KODE_New KODE_RangeSliderWidget(KODE_Rect( 10,280,190, 20),  0.2f, 0.7f ));
  window->appendWidget( KODE_New KODE_ScrollBarWidget(  KODE_Rect( 10,310,190, 20),  false ));
  window->appendWidget( KODE_New KODE_SelectorWidget(   KODE_Rect( 10,340,190, 20),  menu ));
  window->appendWidget( KODE_New KODE_TextEditWidget(   KODE_Rect( 10,370,190, 20),  "textedit" ));
  
  window->appendWidget( KODE_New KODE_KeyboardWidget(   KODE_Rect(210, 10,190, 50)   ));
  window->appendWidget( KODE_New KODE_KnobWidget(       KODE_Rect(210, 70, 50, 50)   ));
  
  KODE_ValueGraphWidget* vg = (KODE_ValueGraphWidget*)
  window->appendWidget( KODE_New KODE_ValueGraphWidget( KODE_Rect(210,130,190, 60)   ));
    vg->appendNode(0.0f,0);
    vg->appendNode(0.3f,0);
    vg->appendNode(0.9f,0);
    vg->appendNode(0.4f,0);
    vg->appendNode(1.0f,0);
    
  //KODE_TabsWidget* tabs = (KODE_TabsWidget*)
  //window->appendWidget( KODE_New KODE_TabsWidget( KODE_Rect(210,300,190, 60), tabs_txt ));
  //  tabs->appendPage( KODE_New KODE_ColorWidget(KODE_Rect(0,0,100,100)), "page1" );
  //  tabs->appendPage( KODE_New KODE_ColorWidget(KODE_Rect(0,0,100,100)), "page2" );
  //  tabs->appendPage( KODE_New KODE_ColorWidget(KODE_Rect(0,0,100,100)), "page_3" );

  window->appendWidget( menu );
  
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
}