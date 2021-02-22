#ifndef kode_sa_header_widget_included
#define kode_sa_header_widget_included
//----------------------------------------------------------------------

#include "gui/kode_image.h"
//#include "gui/kode_surface.h"
//#include "../data/knob2_32x32_65.h"
//#include "../data/fader1_20x64_65.h"
#include "../data/sa_logo_40_trans_black.h"
//#include "../data/knob_100.h"

#include "gui/widgets/kode_color_widget.h"

//----------------------------------------------------------------------

class KODE_SAHeaderWidget
: public KODE_ColorWidget {
  
private:

  KODE_Image* image = KODE_NULL;
  char text2_buffer[256];
  
public:

  KODE_SAHeaderWidget(KODE_Drawable* ADrawable, const char* AName, const char* AVersion, const char*AFormat="")
  : KODE_ColorWidget( KODE_Rect(60), KODE_DarkGrey ) {
    
    setFillBackground();
    setBackgroundColor( KODE_Color(0.50f) );
    setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);

    image = KODE_New KODE_Image(ADrawable,sa_logo_40_trans_black,sa_logo_40_trans_black_size);
    image->getBitmap()->premultAlpha();
    KODE_ImageWidget* image1 = KODE_New KODE_ImageWidget(KODE_Rect( 10,10, 40,40 ), image);
    appendWidget(image1);
    
    KODE_TextWidget* text1 = KODE_New KODE_TextWidget( KODE_Rect( 60,10, 100,20 ), AName );
    text1->setFillBackground(false);
    text1->setDrawBorder(false);
    appendWidget(text1);
    
    KODE_Strcpy(text2_buffer,AVersion);
    KODE_Strcat(text2_buffer," : ");
    KODE_Strcat(text2_buffer,AFormat);
    
    KODE_TextWidget* text2 = KODE_New KODE_TextWidget( KODE_Rect( 60,30, 100,20 ), /*AVersion*/text2_buffer );
    text2->setFillBackground(false);
    text2->setDrawBorder(false);
    appendWidget(text2);
    
  }
  
  virtual ~KODE_SAHeaderWidget() {
    if (image) KODE_Delete image;
  }
  
};

//----------------------------------------------------------------------
#endif
