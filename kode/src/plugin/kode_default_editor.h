#ifndef kode_default_editor_included
#define kode_default_editor_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_editor.h"

//----------------------------------------------------------------------

//#define BORDER  10
//#define SPACE   5
//#define WIDTH   200
//#define HEIGHT  20
//  
//uint32_t calcDefaultEditorWidth() {
//  return WIDTH + (BORDER * 2);
//}
//  
//uint32_t calcDefaultEditorHeight(uint32_t num) {
//  //uint32_t num = MDescriptor->getNumParameters();
//  return (HEIGHT * num) + (SPACE * (num-1)) + (BORDER * 2);
//}

//----------------------------------------------------------------------

class KODE_DefaultEditor
: public KODE_Editor {

public:

  KODE_DefaultEditor(KODE_EditorListener* AListener, KODE_Descriptor* ADescriptor, void* AParent)
  : KODE_Editor(AListener,ADescriptor,AParent) {
  }
  
  //----------
  
  KODE_DefaultEditor(uint32_t AWidth, uint32_t AHeight, KODE_EditorListener* AListener, KODE_Descriptor* ADescriptor, void* AParent)
  : KODE_Editor(AWidth,AHeight,AListener,ADescriptor,AParent) {
  }
  
  virtual ~KODE_DefaultEditor() {
  }
  
};

//----------------------------------------------------------------------

//#undef BORDER
//#undef SPACE
//#undef WIDTH
//#undef HEIGHT

//----------------------------------------------------------------------
#endif
