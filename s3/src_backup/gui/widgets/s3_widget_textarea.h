#ifndef s3_widget_textarea_included
#define s3_widget_textarea_included
//----------------------------------------------------------------------

/*
  maybe this one needs some locking?
  if i press midi keys in a synth plugin, and draws text output to
  this window from the process thread (a big no no! but..)..
  during drawing, some of the members, like the string list might
  be updated, and we can mismatches, etc..

  we can do:
  * queue?
  * locks
*/

#include "common/s3_lock.h"
#include "common/s3_string.h"
#include "gui/s3_widget.h"
#include "gui/widgets/s3_widget_color.h"
#include "gui/widgets/s3_widget_scrollbar.h"

//----------------------------------------------------------------------

class S3_Widget_TextArea
//: public S3_Widget {
: public S3_Widget_Color {

  private:

    S3_Widget_ScrollBar*  MScrollBar;
    S3_StringArray        MStrings;
    uint32                MMaxStrings;
    uint32                MStringHeight;

    //S3_Lock               MLock;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Widget_TextArea(S3_Rect ARect, uint32 AMaxStrings, uint32 AAlignment=s3_wa_none)
    //: S3_Widget(ARect,AAlignment) {
    : S3_Widget_Color(ARect,S3_Grey,AAlignment) {
      MName         = "S3_Widget_TextArea";
      MHint         = "textarea";
      margins(2);
      //MStrings.clear();
      MMaxStrings   = AMaxStrings;
      MStringHeight = 16;
      MScrollBar    = new S3_Widget_ScrollBar( S3_Rect(10), 1, true, s3_wa_fillRight );
      MScrollBar->value(0);
      MScrollBar->thumbsize(1);
      appendWidget( MScrollBar );
    }

    //----------

    virtual ~S3_Widget_TextArea() {
      #ifndef S3_NO_AUTODELETE
      deleteStrings();
      #endif
    }

  //------------------------------
  //
  //------------------------------

  public:

    void appendText(const char* AText) {
      //MLock.lock();
      if (MStrings.size() >= MMaxStrings ) {
        delete MStrings[0];
        //MStrings.remove(0);
        MStrings.removeFirst();
      }
      MStrings.append( new S3_String(AText) );
      //MLock.unlock();
    }

    //----------

    void appendString(const char* format, ...) {
      char buffer[256];
      //S3_Strcpy(buffer,MPrefix);
      va_list args;
      va_start(args,format);
      vsprintf(buffer,format,args);
      appendText(buffer);
    }

    //----------

    void deleteStrings(void) {
      for (uint32 i=0; i<MStrings.size(); i++) {
        //MLock.lock();
        delete MStrings[i];
        MStrings[i] = S3_NULL;
        //MLock.unlock();
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      //MLock.lock();
      //S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
      S3_Widget_Color::on_widgetPaint(AWidget,APainter,ARect,AMode);
      int32 num_strings = MStrings.size();
      if (num_strings>0) {
        float total_height = MStrings.size() * MStringHeight;
        if (total_height > MRect.h) {
          MScrollBar->thumbsize( MRect.h / total_height );
          float num_visible = (float)MRect.h / (float)MStringHeight;
          float offset = (float)( total_height - MRect.h ) * MScrollBar->value();
          float first_visible = offset / MStringHeight;
          float first_string = S3_Trunc(first_visible);
          float first_offset = (first_visible - first_string) * MStringHeight;
          //S3_DTrace("num_visible %.2f offset %.2f first_visible %.2f first_string %.2f first_offset %.2f\n",num_visible,offset,first_visible,first_string,first_offset);
          int32 first = first_string;
          int32 num   = ceil( num_visible );
          int32 y = -first_offset;
          APainter->setTextColor(S3_Black);
          for (int32 i=0; i<num; i++) {
            APainter->drawText(2,2+y+i*MStringHeight,MStrings[first+i]->ptr());
          }
        }
        else {
          MScrollBar->thumbsize(1);
          APainter->setTextColor(S3_Black);
          for (int32 i=0; i<num_strings; i++) {
            APainter->drawText(2,2+i*MStringHeight,MStrings[i]->ptr());
          }
        }
      }
      //MLock.unlock();
    }

  //------------------------------
  //
  //------------------------------

  public:

  //virtual
  void do_widgetUpdate(S3_Widget* AWidget) {
    //S3_Widget::do_widgetUpdate(AWidget);
    S3_Widget_Color::do_widgetUpdate(AWidget);
    if (AWidget==MScrollBar) {
      do_widgetRedraw(this,MRect,s3_pm_normal);
    }
  }

};

//----------------------------------------------------------------------
#endif
