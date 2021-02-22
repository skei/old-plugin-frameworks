#ifndef s3_dialogs_included
#define s3_dialogs_included
//----------------------------------------------------------------------

//#if 0

//----------------------------------------------------------------------
// linux
//----------------------------------------------------------------------

// uses zenity
// gnome?

#ifdef S3_LINUX

static char s3_zenity_buffer[1024];
static char s3_zenity_filename[1024];

//----------

const char* s3_do_zenity(void) {
  FILE *f = popen(zenity_buffer,"r");
  if ( fgets(zenity_filename,1024,f) ) {
    return zenity_filename;
  }
  else {
    return "";//S3_NULL;
  }
}

//----------

const char* S3_Dialog_ColorSelect(const char* ATitle, bool APalette=true) {
  S3_Strcpy(zenity_buffer,"zenity --color-selection");
  S3_Strcat(zenity_buffer," --title='");    S3_Strcat(zenity_buffer,ATitle);    S3_Strcat(zenity_buffer,"'");
  if (APalette) S3_Strcat(zenity_buffer," --show-palette");
  return s3_do_zenity();
}

const char* SDialog_Error(const char* ATitle, const char* AText) {
  SStrcpy(zenity_buffer,"zenity --error");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_FileOpen(const char* ATitle, bool AMultiple=false) {
  SStrcpy(zenity_buffer,"zenity --file-selection");
  SStrcat(zenity_buffer," --title='");    SStrcat(zenity_buffer,ATitle);    SStrcat(zenity_buffer,"'");
  if (AMultiple) {
    SStrcat(zenity_buffer," --multiple");
    SStrcat(zenity_buffer," --separator=':'");
  }
  return do_zenity();
}

const char* SDialog_FileSave(const char* ATitle, const char* AFilename) {
  SStrcpy(zenity_buffer,"zenity --file-selection");
  SStrcat(zenity_buffer," --save");
  SStrcat(zenity_buffer," --title='");    SStrcat(zenity_buffer,ATitle);    SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --filename='"); SStrcat(zenity_buffer,AFilename); SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_Info(const char* ATitle, const char* AText) {
  SStrcpy(zenity_buffer,"zenity --info");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_Question(const char* ATitle, const char* AText) {
  SStrcpy(zenity_buffer,"zenity --question");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_Text(const char* ATitle, const char* AText, const char* AEntryText) {
  SStrcpy(zenity_buffer,"zenity --entry");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);            SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --entry-text='");    SStrcat(zenity_buffer,AEntryText); SStrcat(zenity_buffer,"'");
  return do_zenity();
}

// segmentation fault

const char* SDialog_TextInfo(const char* ATitle, const char* AFilename, bool AEditable=false, bool AHtml=false, const char* AUrl=S3_NULL) {
  SStrcpy(zenity_buffer,"zenity --text-info");
  SStrcat(zenity_buffer," --filename='");       SStrcat(zenity_buffer,AFilename);   SStrcat(zenity_buffer,"'");
  if (AEditable) SStrcat(zenity_buffer," --editable");
  if (AHtml) SStrcat(zenity_buffer," --html");
  if (AUrl) SStrcat(zenity_buffer," --url='");  SStrcat(zenity_buffer,AUrl);        SStrcat(zenity_buffer,"'");
  return do_zenity();
}

const char* SDialog_Value(const char* ATitle, const char* AText, int AVal=0, int AMin=0, int AMax=100, int AStep=1) {
  char temp[256];
  SStrcpy(zenity_buffer,"zenity --scale");
  SStrcat(zenity_buffer," --text='");       SStrcat(zenity_buffer,AText); SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --value='");      SStrcat(zenity_buffer, SIntToString(temp,AVal));  SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --min-value='");  SStrcat(zenity_buffer, SIntToString(temp,AMin));  SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --max-value='");  SStrcat(zenity_buffer, SIntToString(temp,AMax));  SStrcat(zenity_buffer,"'");
  SStrcat(zenity_buffer," --step='");       SStrcat(zenity_buffer, SIntToString(temp,AStep)); SStrcat(zenity_buffer,"'");
  //SStrcat(zenity_buffer," --print-partial");
  //SStrcat(zenity_buffer," --hide-value");
  return do_zenity();
}

const char* SDialog_Warning(const char* ATitle, const char* AText) {
  SStrcpy(zenity_buffer,"zenity --warning");
  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
  return do_zenity();
}

#endif // S3_LINUX

//----------------------------------------------------------------------
// win32
//----------------------------------------------------------------------

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645524%28v=vs.85%29.aspx

#ifdef S3_WIN32

#include <windows.h>
#include <commdlg.h>

// link with comdlg32

//----------

const char* S3_Dialog_ColorSelect(const char* ATitle, bool APalette=true) {
  CHOOSECOLOR choose_color;
  COLORREF custom_colors[16];
  ZeroMemory(&choose_color, sizeof(choose_color));
  choose_color.lStructSize = sizeof (CHOOSECOLOR);
  choose_color.rgbResult = RGB(0,0,0);//old_attr;
  choose_color.lpCustColors = custom_colors;
  choose_color.Flags = CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN;
  if (ChooseColor(&choose_color) == TRUE) {
    S3_Trace("%08x\n",choose_color.rgbResult); // ABGR
    //return (int) cc.rgbResult;
  } else {
    S3_Trace("---\n");
    //return -1;
  }
  return S3_NULL;
}

const char* S3_Dialog_Error(const char* ATitle, const char* AText) {
//  SStrcpy(zenity_buffer,"zenity --error");
//  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
//  return do_zenity();
  return S3_NULL;
}

const char* SDialog_FileOpen(const char* ATitle, bool AMultiple=false) {
//  SStrcpy(zenity_buffer,"zenity --file-selection");
//  SStrcat(zenity_buffer," --title='");    SStrcat(zenity_buffer,ATitle);    SStrcat(zenity_buffer,"'");
//  if (AMultiple) {
//    SStrcat(zenity_buffer," --multiple");
//    SStrcat(zenity_buffer," --separator=':'");
//  }
//  return do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_FileSave(const char* ATitle, const char* AFilename) {
//  SStrcpy(zenity_buffer,"zenity --file-selection");
//  SStrcat(zenity_buffer," --save");
//  SStrcat(zenity_buffer," --title='");    SStrcat(zenity_buffer,ATitle);    SStrcat(zenity_buffer,"'");
//  SStrcat(zenity_buffer," --filename='"); SStrcat(zenity_buffer,AFilename); SStrcat(zenity_buffer,"'");
//  return do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Info(const char* ATitle, const char* AText) {
//  SStrcpy(zenity_buffer,"zenity --info");
//  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
//  return do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Question(const char* ATitle, const char* AText) {
//  SStrcpy(zenity_buffer,"zenity --question");
//  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
//  return do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Text(const char* ATitle, const char* AText, const char* AEntryText) {
//  SStrcpy(zenity_buffer,"zenity --entry");
//  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);            SStrcat(zenity_buffer,"'");
//  SStrcat(zenity_buffer," --entry-text='");    SStrcat(zenity_buffer,AEntryText); SStrcat(zenity_buffer,"'");
//  return do_zenity();
  return S3_NULL;
}

// segmentation fault

const char* S3_Dialog_TextInfo(const char* ATitle, const char* AFilename, bool AEditable=false, bool AHtml=false, const char* AUrl=S3_NULL) {
//  SStrcpy(zenity_buffer,"zenity --text-info");
//  SStrcat(zenity_buffer," --filename='");       SStrcat(zenity_buffer,AFilename);   SStrcat(zenity_buffer,"'");
//  if (AEditable) SStrcat(zenity_buffer," --editable");
//  if (AHtml) SStrcat(zenity_buffer," --html");
//  if (AUrl) SStrcat(zenity_buffer," --url='");  SStrcat(zenity_buffer,AUrl);        SStrcat(zenity_buffer,"'");
//  return do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Value(const char* ATitle, const char* AText, int AVal=0, int AMin=0, int AMax=100, int AStep=1) {
//  char temp[256];
//  SStrcpy(zenity_buffer,"zenity --scale");
//  SStrcat(zenity_buffer," --text='");       SStrcat(zenity_buffer,AText); SStrcat(zenity_buffer,"'");
//  SStrcat(zenity_buffer," --value='");      SStrcat(zenity_buffer, SIntToString(temp,AVal));  SStrcat(zenity_buffer,"'");
//  SStrcat(zenity_buffer," --min-value='");  SStrcat(zenity_buffer, SIntToString(temp,AMin));  SStrcat(zenity_buffer,"'");
//  SStrcat(zenity_buffer," --max-value='");  SStrcat(zenity_buffer, SIntToString(temp,AMax));  SStrcat(zenity_buffer,"'");
//  SStrcat(zenity_buffer," --step='");       SStrcat(zenity_buffer, SIntToString(temp,AStep)); SStrcat(zenity_buffer,"'");
//  //SStrcat(zenity_buffer," --print-partial");
//  //SStrcat(zenity_buffer," --hide-value");
//  return do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Warning(const char* ATitle, const char* AText) {
//  SStrcpy(zenity_buffer,"zenity --warning");
//  SStrcat(zenity_buffer," --text='");    SStrcat(zenity_buffer,AText);    SStrcat(zenity_buffer,"'");
//  return do_zenity();
  return S3_NULL;
}

#endif // S3_WIN32

//#endif // 0

//----------------------------------------------------------------------
#endif
