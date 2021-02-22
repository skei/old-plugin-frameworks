

//----------------------------------------------------------------------

#include "gui/kode_image.h"
#include "gui/kode_surface.h"

#include "../data/knob2_32x32_65.h"
#include "../data/fader1_20x64_65.h"
#include "../data/sa_logo_40_white.h"
//#include "../data/knob_100.h"

//----------------------------------------------------------------------

/*
 * class KODE_SAHeaderWidget
: public KODE_ColorWidget {
  
private:

  KODE_Image* image = KODE_NULL;
  
public:

  KODE_SAHeaderWidget(KODE_Drawable* ADrawable, const char* AName, const char* AVersion)
  : KODE_ColorWidget( KODE_Rect(60), KODE_DarkGrey ) {
    
    setFillBackground();
    setBackgroundColor( KODE_Color(0.25f, 0.25f, 0.25f ) );
    setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);

    image = KODE_New KODE_Image(ADrawable,sa_logo_40_white,sa_logo_40_white_size);
    image->getBitmap()->premultAlpha();
    KODE_ImageWidget* image1 = KODE_New KODE_ImageWidget(KODE_Rect( 10,10, 40,40 ), image);
    appendWidget(image1);
    
    KODE_TextWidget* text1 = KODE_New KODE_TextWidget( KODE_Rect( 60,10, 100,20 ), AName );
    text1->setFillBackground(false);
    text1->setDrawBorder(false);
    appendWidget(text1);
    
    KODE_TextWidget* text2 = KODE_New KODE_TextWidget( KODE_Rect( 60,30, 100,20 ), AVersion );
    text2->setFillBackground(false);
    text2->setDrawBorder(false);
    appendWidget(text2);
    
  }
  
  virtual ~KODE_SAHeaderWidget() {
    if (image) KODE_Delete image;
  }
  
};
 * */

//----------------------------------------------------------------------

const char* buttonrow_txt[] = {
  "1", "2", "3", "4", "5"
};

const char* selector_txt[] = {
  "menuitem 1", "menuitem 2", "menuitem 3", "menuitem 4", "menuitem 5"
};

//----------------------------------------------------------------------

class myEditor
: public KODE_Editor {
  
//------------------------------
private:
//------------------------------

  KODE_Image*   m_knob_image   = KODE_NULL;
  KODE_Surface* m_knob_surface = KODE_NULL;
  KODE_TileMap* m_knob_tilemap = KODE_NULL;
  
  KODE_Image*   m_fader_image   = KODE_NULL;
  KODE_Surface* m_fader_surface = KODE_NULL;
  KODE_TileMap* m_fader_tilemap = KODE_NULL;
  
  #ifdef KODE_GUI_XCB
    KODE_XcbPainter* m_knob_painter = KODE_NULL;
    KODE_XcbPainter* m_fader_painter = KODE_NULL;
  #else
    KODE_CairoPainter* m_knob_painter = KODE_NULL;
    KODE_CairoPainter* m_fader_painter = KODE_NULL;
  #endif

//------------------------------
public:
//------------------------------

  myEditor(KODE_EditorListener* AListener, KODE_Descriptor* ADescriptor, void* AParent)
  : KODE_Editor(AListener,ADescriptor,AParent) {
  }
  
  //----------
  
  virtual ~myEditor() {
    if (m_knob_image)   KODE_Delete m_knob_image;
    if (m_knob_surface) KODE_Delete m_knob_surface;
    if (m_knob_painter) KODE_Delete m_knob_painter;
    if (m_knob_tilemap) KODE_Delete m_knob_tilemap;
  }
  
//------------------------------
public:
//------------------------------

  void initWidgets() {
    
//----------
    
    // menu
    
    KODE_MenuWidget* menu = KODE_New KODE_MenuWidget();
      menu->setMenuSize(1,5);
      menu->appendMenuItems(selector_txt,5);
      
    // images
      
    m_knob_image = KODE_New KODE_Image(this,knob2_32x32_65,knob2_32x32_65_size);     // symbols (in .h file)
    m_knob_image->getBitmap()->fillBackground( KODE_BGRA(KODE_Grey) );
    m_knob_surface = KODE_New KODE_Surface(this,32,992);                             // bitmap dimensions
    #ifdef KODE_GUI_XCB
      KODE_XcbPainter* m_knob_painter = KODE_New KODE_XcbPainter(m_knob_surface);
    #else
      KODE_CairoPainter* m_knob_painter = KODE_New KODE_CairoPainter(m_knob_surface);
    #endif
    m_knob_painter->drawBitmap(0,0,m_knob_image);
    //m_painter->flush();
    m_knob_tilemap = KODE_New KODE_TileMap(m_knob_surface,KODE_Rect(32,992),1,31);    // rect(bitmap dimensions),xcount,ycount
    
    m_fader_image = KODE_New KODE_Image(this,fader1_20x64_65,fader1_20x64_65_size);     // symbols (in .h file)
    m_fader_image->getBitmap()->fillBackground( KODE_BGRA(KODE_Grey) );
    m_fader_surface = KODE_New KODE_Surface(this,20,4160);                             // bitmap dimensions
    #ifdef KODE_GUI_XCB
      KODE_XcbPainter* m_fader_painter = KODE_New KODE_XcbPainter(m_fader_surface);
    #else
      KODE_CairoPainter* m_fader_painter = KODE_New KODE_CairoPainter(m_fader_surface);
    #endif
    m_fader_painter->drawBitmap(0,0,m_fader_image);
    //m_painter->flush();
    m_fader_tilemap = KODE_New KODE_TileMap(m_fader_surface,KODE_Rect(20,4160),1,65);    // rect(bitmap dimensions),xcount,ycount
    
//----------
    
    // sa-header
    
    KODE_SAHeaderWidget* saheader = KODE_New KODE_SAHeaderWidget(this,"plugin-name","v0.0.0");
    appendWidget(saheader);
    
    //----------
    // left
    //----------
    
    KODE_Widget* left_panel = KODE_New KODE_Widget( KODE_Rect(200));
    left_panel->setAlignment(KODE_WIDGET_ALIGN_LEFT_FILL);
    left_panel->setPadding(5);
    left_panel->setSpacing(5);
    left_panel->setName("left_panel");
    appendWidget(left_panel);
    
    KODE_GroupBoxWidget* groupbox1 = KODE_New KODE_GroupBoxWidget( KODE_Rect(300) );
    groupbox1->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
    //groupbox1->setPadding(5);
    left_panel->appendWidget(groupbox1);
    
      KODE_BorderWidget* container1 = groupbox1->getContainer();
      container1->setPadding(5);
      container1->setSpacing(5);
      container1->setBorderColor(KODE_DarkGrey);
      container1->setBackgroundColor(KODE_Grey);
    
      KODE_ScrollBoxWidget* groupbox1_scrollbox = KODE_New KODE_ScrollBoxWidget( KODE_Rect() );
      groupbox1_scrollbox->setAlignment(KODE_WIDGET_ALIGN_CLIENT_FILL);
      groupbox1->appendWidget(groupbox1_scrollbox);
    
      KODE_ColorWidget* color = KODE_New KODE_ColorWidget( KODE_Rect(20), KODE_DarkGrey );
      color->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(color);
      
      KODE_BorderWidget* border = KODE_New KODE_BorderWidget( KODE_Rect(20), KODE_DarkGrey );
      border->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      border->setFillBackground(false);
      groupbox1_scrollbox->appendWidget(border);
      
      KODE_TextWidget* text = KODE_New KODE_TextWidget( KODE_Rect(20), "Text" );
      text->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      text->setDrawBorder(false);
      text->setFillBackground(false);
      text->setTextColor(KODE_DarkGrey);
      groupbox1_scrollbox->appendWidget(text);
      
      KODE_ValueWidget* value = KODE_New KODE_ValueWidget( KODE_Rect(20), 0.5f );
      value->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      value->setText("Value");
      value->setDrawBorder(false);
      value->setFillBackground(false);
      value->setTextColor(KODE_DarkGrey);
      value->setValueColor(KODE_DarkGrey);
      groupbox1_scrollbox->appendWidget(value);
      
      KODE_DragValueWidget* dragvalue = KODE_New KODE_DragValueWidget( KODE_Rect(20), 0.5f );
      dragvalue->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      dragvalue->setText("DragValue");
      dragvalue->setDrawBorder(false);
      dragvalue->setFillBackground(false);
      dragvalue->setTextColor(KODE_DarkGrey);
      dragvalue->setValueColor(KODE_DarkGrey);
      groupbox1_scrollbox->appendWidget(dragvalue);
      
      KODE_SliderWidget* slider = KODE_New KODE_SliderWidget( KODE_Rect(20) );
      slider->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      slider->setText("Slider");
      slider->setValue(0.5f);
      groupbox1_scrollbox->appendWidget(slider);
      
      KODE_ButtonWidget* button = KODE_New KODE_ButtonWidget( KODE_Rect(20), 0, "Button On", "Button Off" );
      button->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(button);
      
      KODE_SwitchWidget* switch_ = KODE_New KODE_SwitchWidget( KODE_Rect(20), 0, "Switch On", "Switch Off" );
      switch_->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(switch_);
      
      KODE_ButtonRowWidget* buttonrow = KODE_New KODE_ButtonRowWidget( KODE_Rect(20), 5, buttonrow_txt );
      buttonrow->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(buttonrow);
      
      KODE_SelectorWidget* selector = KODE_New KODE_SelectorWidget( KODE_Rect(20), menu );
      selector->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(selector);
      
      KODE_ScrollBarWidget* scrollbar = KODE_New KODE_ScrollBarWidget( KODE_Rect(20) );
      scrollbar->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(scrollbar);
      
      
      KODE_RangeSliderWidget* rangeslider = KODE_New KODE_RangeSliderWidget( KODE_Rect(20), 0.1f, 0.7f );
      rangeslider->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(rangeslider);
      
      KODE_SliderBankWidget* sliderbank = KODE_New KODE_SliderBankWidget( KODE_Rect(30), 16 );
      sliderbank->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(sliderbank);
      
      //KODE_TextEditWidget* textedit = KODE_New KODE_TextEditWidget( KODE_Rect(20), "TextEdit" );
      //textedit->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      //groupbox1_scrollbox->appendWidget(textedit);
      
      KODE_ValueGraphWidget* valuegraph = KODE_New KODE_ValueGraphWidget( KODE_Rect(30), 16 );
      valuegraph->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(valuegraph);
      
      KODE_KeyboardWidget* keyboard = KODE_New KODE_KeyboardWidget( KODE_Rect(30) );
      keyboard->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      groupbox1_scrollbox->appendWidget(keyboard);
      
    //----------

    KODE_GroupBoxWidget* groupbox2 = KODE_New KODE_GroupBoxWidget( KODE_Rect(200) );
    groupbox2->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
    //groupbox1->setPadding(5);
    left_panel->appendWidget(groupbox2);
    
    //----------
    //
    //----------

    // sizer 1

    KODE_SizerWidget* sizer = KODE_New KODE_SizerWidget( KODE_Rect(5), KODE_SIZER_LEFT, left_panel/*groupbox1*/ );
    sizer->setAlignment(KODE_WIDGET_ALIGN_LEFT_FILL);
    appendWidget(sizer);
    
    //----------
    // middle
    //----------

    // tabs
    
    KODE_TabsWidget* tabs = KODE_New KODE_TabsWidget( KODE_Rect(400) );
    tabs->setAlignment(KODE_WIDGET_ALIGN_LEFT_FILL);
    tabs->setMinWidth(100);
    //tabs->setMaxWidth(400);
    appendWidget(tabs);
    
      //----------
      // tab 1
      //----------
      
      KODE_ColorWidget* tab1 = KODE_New KODE_ColorWidget( KODE_Rect(0), KODE_Grey );
      tab1->setAlignment(KODE_WIDGET_ALIGN_CLIENT);
      tab1->setPadding(5);
      tab1->setSpacing(5);
      tabs->appendPage(tab1,"1");
      
        //
        
        KODE_CurveWidget* curve = KODE_New KODE_CurveWidget( KODE_Rect(32) );
        curve->setAlignment(KODE_WIDGET_ALIGN_STACK_HORIZ);
        tab1->appendWidget(curve);
        
        //
        
        KODE_ImageStripWidget* imagestrip = KODE_New KODE_ImageStripWidget( KODE_Rect(32), m_knob_tilemap, 0.0f );
        imagestrip->setAlignment(KODE_WIDGET_ALIGN_STACK_HORIZ);
        tab1->appendWidget(imagestrip);
        
        KODE_ImageStripWidget* imagestrip2 = KODE_New KODE_ImageStripWidget( KODE_Rect(20,64), m_fader_tilemap, 0.0f );
        imagestrip2->setAlignment(KODE_WIDGET_ALIGN_STACK_HORIZ);
        tab1->appendWidget(imagestrip2);

        
        //
        
        KODE_TimelineWidget* timeline = KODE_New KODE_TimelineWidget( KODE_Rect(150) );
        timeline->setAlignment(KODE_WIDGET_ALIGN_BOTTOM_FILL);
        tab1->appendWidget(timeline);
        
          KODE_TimelineTrack* track;
          KODE_TimelineSegment* segment;
          
          track = KODE_New KODE_TimelineTrack("track1");
          timeline->addTrack(track);
            segment = KODE_New KODE_TimelineSegment("segment", 1.0f, 10.0f);
            track->addSegment(segment);
            segment = KODE_New KODE_TimelineSegment("item2", 11.0f, 15.0f);
            track->addSegment(segment);
        
          track = KODE_New KODE_TimelineTrack("track two");
          timeline->addTrack(track);
            segment = KODE_New KODE_TimelineSegment("s1", 4.0f, 7.0f);
            track->addSegment(segment);
            segment = KODE_New KODE_TimelineSegment("s2", 7.0f, 18.0f);
            track->addSegment(segment);
        
        //
        
        KODE_GraphWidget* graph = KODE_New KODE_GraphWidget( KODE_Rect(0) );
        graph->setAlignment(KODE_WIDGET_ALIGN_CLIENT_FILL);
        tab1->appendWidget(graph);
        
        KODE_GraphModule* module = KODE_New KODE_GraphModule();
          module->numInputs = 2;
          module->inputs[0] = kpt_data;
          module->inputs[1] = kpt_data;
          module->numOutputs = 3;
          module->outputs[0] = kpt_signal;
          module->outputs[1] = kpt_data;
          module->outputs[2] = kpt_data;
        graph->addModule(module,10,10,"module1");
      
        module = KODE_New KODE_GraphModule();
          module->numInputs = 1;
          module->inputs[0] = kpt_data;
          module->numOutputs = 2;
          module->outputs[0] = kpt_signal;
          module->outputs[1] = kpt_data;
        graph->addModule(module,90,50,"module2");
      
        module = KODE_New KODE_GraphModule();
          module->numInputs = 2;
          module->inputs[0] = kpt_signal;
          module->inputs[1] = kpt_data;
          module->numOutputs = 2;
          module->outputs[0] = kpt_data;
          module->outputs[1] = kpt_signal;
        graph->addModule(module,50,100,"m3");
      
      //----------
      // tab 2
      //----------
      
      KODE_ColorWidget* tab2 = KODE_New KODE_ColorWidget( KODE_Rect(0), KODE_Grey );
      tab2->setAlignment(KODE_WIDGET_ALIGN_CLIENT);
      tab2->setPadding(5);
      tab2->setSpacing(5);
      tabs->appendPage(tab2,"2");
      
        KODE_GridWidget* grid = KODE_New KODE_GridWidget( KODE_Rect(64), 5, 5 );
        grid->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
        tab2->appendWidget(grid);

        //
        
      //KODE_ImageStripWidget* imagestrip = KODE_New KODE_ImageStripWidget( KODE_Rect(100), m_tilemap, 0.0f );
      //imagestrip->setAlignment(KODE_WIDGET_ALIGN_STACK_VERT);
      //tab2->appendWidget(imagestrip);
      
      //----------
      //----------
      
      tabs->selectPage(0);
    
    //----------
    //
    //----------
    
    // sizer 1

    KODE_SizerWidget* sizer2 = KODE_New KODE_SizerWidget( KODE_Rect(5), KODE_SIZER_LEFT, tabs );
    sizer2->setAlignment(KODE_WIDGET_ALIGN_LEFT_FILL);
    appendWidget(sizer2);

    //----------
    // right
    //----------

    // scroll box
    
    KODE_ScrollBoxWidget* scrollbox = KODE_New KODE_ScrollBoxWidget( KODE_Rect() );
    scrollbox->setAlignment(KODE_WIDGET_ALIGN_CLIENT_FILL);
    appendWidget(scrollbox);

      //KODE_ColorWidget* cw2 = KODE_New KODE_ColorWidget( KODE_Rect(0), KODE_DarkRed );
      //cw2->setAlignment(KODE_WIDGET_ALIGN_CLIENT_FILL);
      for (uint32_t i=0; i<50; i++) {
        KODE_KnobWidget* knob = KODE_New KODE_KnobWidget(KODE_Rect(32),0);
        knob->setAlignment(KODE_WIDGET_ALIGN_STACK_VERT);
        knob->setDrawBorder();
        knob->setBorderColor(KODE_DarkGrey);
        knob->setFillBackground();
        knob->setBackgroundColor(KODE_Grey);
        scrollbox->appendWidget(knob);
      }
      
      // teste.. non-stacking after stacking..
      
      KODE_KnobWidget* knob = KODE_New KODE_KnobWidget(KODE_Rect(0,0,32,32),0);
      knob->setAlignment(KODE_WIDGET_ALIGN_TOP_LEFT);
      knob->setDrawBorder();
      knob->setBorderColor(KODE_White);
      knob->setFillBackground();
      knob->setBackgroundColor(KODE_Grey);
      scrollbox->appendWidget(knob);
    
    //----------

//----------
    
    appendWidget(menu);
    
  }

//------------------------------
public:
//------------------------------

};
