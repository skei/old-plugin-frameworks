#ifndef s3_skin_included
#define s3_skin_included
//----------------------------------------------------------------------

/*
  paint modes:
    s3_pm_normal    = 0,
    s3_pm_hover     = 1,
    s3_pm_interact  = 2,
    s3_pm_enter     = 3,
    s3_pm_leave     = 4

    ---

    disabled
    off, off hover, off click
    on, on hover, on click


*/

//----------

/*
  panel (up/down)
  clickable header/button, up/down
  text

*/



//----------------------------------------------------------------------

class S3_Skin {

  public:

    uint32      MFlags;
    S3_Painter* MPainter;
    struct {
      S3_Color  background;
      S3_Color  border;
      S3_Color  textLabel;
      S3_Color  textValue;
    } MColors;
    struct {
      bool fillBackground;
      bool drawBorder;
      bool drawLabel;
      bool drawValue;
    } MOptions;

  public:

    void setPainter(S3_Painter* p) { MPainter = p; }

  //------------------------------
  //
  //------------------------------

  public:

    S3_Skin() {
      MFlags                  = 0;
      MPainter                = S3_NULL;
      MColors.background      = S3_DarkGrey;
      MColors.border          = S3_Black;
      MColors.textLabel       = S3_LightGrey;
      MColors.textValue       = S3_White;
      MOptions.fillBackground = false;
      MOptions.drawBorder     = false;
      MOptions.drawLabel      = false;
      MOptions.drawValue      = false;
    }

    //----------

  //------------------------------
  //
  //------------------------------

  public:

};

//----------------------------------------------------------------------
#endif
