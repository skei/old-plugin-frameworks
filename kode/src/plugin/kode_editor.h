#ifndef kode_editor_included
#define kode_editor_included
//----------------------------------------------------------------------

#include "base/kode.h"

#include "plugin/kode_descriptor.h"
#include "plugin/kode_editor_listener.h"

#include "gui/kode_widget.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"
//#include "gui/widgets/kode_slider_widget.h"

class KODE_Editor
: public KODE_Window {
  
//------------------------------
protected:
//------------------------------

  KODE_EditorListener*  MListener   = KODE_NULL;
  KODE_Descriptor*      MDescriptor = KODE_NULL;
  
  //uint32_t              MStateSize    = 0;
  //void*                 MStatePtr     = KODE_NULL;
  
//------------------------------
public:
//------------------------------

  KODE_Editor(KODE_EditorListener* AListener, KODE_Descriptor* ADescriptor, void* AParent)
  : KODE_Window(
      ADescriptor->getEditorDefaultWidth(),
      ADescriptor->getEditorDefaultHeight(),
      AParent) {
    MListener = AListener;
    MDescriptor = ADescriptor;
  }
  
  //----------
  
  KODE_Editor(uint32_t AWidth, uint32_t AHeight, KODE_EditorListener* AListener, KODE_Descriptor* ADescriptor, void* AParent)
  : KODE_Window(
      AWidth,
      AHeight,
      AParent) {
    MListener = AListener;
    MDescriptor = ADescriptor;
  }
  
  //----------
  
  virtual ~KODE_Editor() {
  }
  
//------------------------------
public:
//------------------------------

  //void      setStateSize(uint32_t ASize)  { MStateSize = ASize; }
  //void      setStatePtr(void* APointer)   { MStatePtr = APointer; }
  //uint32_t  getStateSize()                { return MStateSize; }
  //void*     getStatePtr()                 { return MStatePtr; }
  
//------------------------------
public:
//------------------------------

  void setupDefaultWidgets(uint32_t APadding, uint32_t ASpacing, uint32_t AWidgetWidth, uint32_t AWidgetHeight) {
    //setMargins(0);
    setPadding(APadding);
    setSpacing(ASpacing);
    
    const char* plugin_name = MDescriptor->getName();
    const char* plugin_version = MDescriptor->getVersionText();
    KODE_SAHeaderWidget* saheader = KODE_New KODE_SAHeaderWidget(this,plugin_name,plugin_version);
    saheader->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
    appendWidget(saheader);
    
    for (uint32_t i=0; i<MDescriptor->getNumParameters(); i++) {
      KODE_SliderWidget* slider = KODE_New KODE_SliderWidget( KODE_Rect(AWidgetWidth,AWidgetHeight) );
        slider->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
        slider->setText( MDescriptor->getParameter(i)->getName()/*"text"*/);
      appendWidget(slider);
      connect(slider,i);
    }
  }

  //----------
  
  virtual void setParameterValue(uint32_t AIndex, float AValue) {
    KODE_Parameter* param = MDescriptor->getParameter(AIndex);
    KODE_Widget* widget = (KODE_Widget*)param->getWidget();
    if (widget) {
      widget->setValue(AValue);
    }
  }
  
  //----------
  
  virtual void setParameterValueAndRedraw(uint32_t AIndex, float AValue) {
    //KODE_Trace("index %i value %.3f\n",AIndex,AValue);
    KODE_Parameter* param = MDescriptor->getParameter(AIndex);
    KODE_Widget* widget = (KODE_Widget*)param->getWidget();
    if (widget) {
      widget->setValue(AValue);
      widget->redraw();
    }
  }
  
  //----------
  
  virtual void setAllParameterValues(uint32_t ANum, float* AValues) {
    for (uint32_t i=0; i<ANum; i++) {
      float v = AValues[i];
      setParameterValue(i,v);
    }
  }
  
  //----------
  
  virtual void setAllParameterValuesAndRedraw(uint32_t ANum, float* AValues) {
    for (uint32_t i=0; i<ANum; i++) {
      float v = AValues[i];
      setParameterValueAndRedraw(i,v);
    }
  }

//------------------------------
public:
//------------------------------

  //virtual void connect(KODE_Widget* AWidget, KODE_Parameter* AParameter) {
  virtual void connect(KODE_Widget* AWidget, uint32_t AParamIndex) {
    //KODE_Trace("AParamIndex %i\n",AParamIndex);
    KODE_Parameter* param = MDescriptor->getParameter(AParamIndex);
    param->setWidget(AWidget);
    AWidget->setParameter(param);
    AWidget->on_connect(param);
  }

//------------------------------
public: // window
//------------------------------

  void do_update(KODE_Widget* AWidget) override {
    //KODE_Trace("index %i\n",AWidget->getIndex());
    //KODE_Window::do_update(AWidget);
    KODE_Parameter* param = (KODE_Parameter*)AWidget->getParameter();
    if (param) {
      uint32_t index = param->getIndex();
      //KODE_Trace("index %i\n",index);
      if (index >= 0) {
        float value = AWidget->getValue();
        MListener->updateParameterFromEditor(index,value);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
