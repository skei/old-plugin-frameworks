#ifndef s3_debug_window_included
#define s3_debug_window_included
//----------------------------------------------------------------------

#ifdef S3_DEBUG_WINDOW

#include "gui/s3_window.h"
#include "gui/widgets/s3_widget_color.h"
#include "gui/widgets/s3_widget_textarea.h"

class S3_Debug_Window {

  private:
    S3_Window*          MWindow;
    S3_Widget_TextArea* MTextArea;


  public:

    S3_Debug_Window() {
      MWindow = S3_NULL;
    }

    //----------

    ~S3_Debug_Window() {
      if (MWindow) {
        MWindow->close();
        delete MWindow;
      }
    }

    //----------

    void initialize(void) {
      MWindow = new S3_Window(S3_NULL,S3_Rect(640,480));
      MWindow->setTitle("S3_Debug_Window");
      MTextArea = new S3_Widget_TextArea( S3_Rect(), 100, s3_wa_fillClient );
      MWindow->appendWidget(MTextArea);
      MWindow->on_widgetAlign(MWindow);
      MWindow->open();
    }

    //----------

    void print(const char* ABuffer) {
      MTextArea->appendText(ABuffer);
      MWindow->do_widgetRedraw(MTextArea,MTextArea->rect(),s3_pm_normal);
    }

};

//----------

static
S3_Debug_Window s3_debug_window;

#define S3_Debug_Window_Print  s3_debug_window.print

#endif // S3_DEBUG_WINDOW

//----------------------------------------------------------------------
#endif
