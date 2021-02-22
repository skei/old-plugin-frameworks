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
  FILE *f = popen(s3_zenity_buffer,"r");
  if ( fgets(s3_zenity_filename,1024,f) ) {
    return s3_zenity_filename;
  }
  else {
    return "";//S3_NULL;
  }
}

//----------

const char* S3_Dialog_ColorSelect(const char* ATitle, bool APalette=true) {
  S3_Strcpy(s3_zenity_buffer,"zenity --color-selection");
  S3_Strcat(s3_zenity_buffer," --title='");    S3_Strcat(s3_zenity_buffer,ATitle);    S3_Strcat(s3_zenity_buffer,"'");
  if (APalette) S3_Strcat(s3_zenity_buffer," --show-palette");
  return s3_do_zenity();
}

const char* SDialog_Error(const char* ATitle, const char* AText) {
  S3_Strcpy(s3_zenity_buffer,"zenity --error");
  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);    S3_Strcat(s3_zenity_buffer,"'");
  return s3_do_zenity();
}

const char* SDialog_FileOpen(const char* ATitle, bool AMultiple=false) {
  S3_Strcpy(s3_zenity_buffer,"zenity --file-selection");
  S3_Strcat(s3_zenity_buffer," --title='");    S3_Strcat(s3_zenity_buffer,ATitle);    S3_Strcat(s3_zenity_buffer,"'");
  if (AMultiple) {
    S3_Strcat(s3_zenity_buffer," --multiple");
    S3_Strcat(s3_zenity_buffer," --separator=':'");
  }
  return s3_do_zenity();
}

const char* SDialog_FileSave(const char* ATitle, const char* AFilename) {
  S3_Strcpy(s3_zenity_buffer,"zenity --file-selection");
  S3_Strcat(s3_zenity_buffer," --save");
  S3_Strcat(s3_zenity_buffer," --title='");    S3_Strcat(s3_zenity_buffer,ATitle);    S3_Strcat(s3_zenity_buffer,"'");
  S3_Strcat(s3_zenity_buffer," --filename='"); S3_Strcat(s3_zenity_buffer,AFilename); S3_Strcat(s3_zenity_buffer,"'");
  return s3_do_zenity();
}

const char* SDialog_Info(const char* ATitle, const char* AText) {
  S3_Strcpy(s3_zenity_buffer,"zenity --info");
  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);    S3_Strcat(s3_zenity_buffer,"'");
  return s3_do_zenity();
}

const char* SDialog_Question(const char* ATitle, const char* AText) {
  S3_Strcpy(s3_zenity_buffer,"zenity --question");
  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);    S3_Strcat(s3_zenity_buffer,"'");
  return s3_do_zenity();
}

const char* SDialog_Text(const char* ATitle, const char* AText, const char* AEntryText) {
  S3_Strcpy(s3_zenity_buffer,"zenity --entry");
  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);            S3_Strcat(s3_zenity_buffer,"'");
  S3_Strcat(s3_zenity_buffer," --entry-text='");    S3_Strcat(s3_zenity_buffer,AEntryText); S3_Strcat(s3_zenity_buffer,"'");
  return s3_do_zenity();
}

// segmentation fault

const char* SDialog_TextInfo(const char* ATitle, const char* AFilename, bool AEditable=false, bool AHtml=false, const char* AUrl=S3_NULL) {
  S3_Strcpy(s3_zenity_buffer,"zenity --text-info");
  S3_Strcat(s3_zenity_buffer," --filename='");       S3_Strcat(s3_zenity_buffer,AFilename);   S3_Strcat(s3_zenity_buffer,"'");
  if (AEditable) S3_Strcat(s3_zenity_buffer," --editable");
  if (AHtml) S3_Strcat(s3_zenity_buffer," --html");
  if (AUrl) S3_Strcat(s3_zenity_buffer," --url='");  S3_Strcat(s3_zenity_buffer,AUrl);        S3_Strcat(s3_zenity_buffer,"'");
  return s3_do_zenity();
}

const char* SDialog_Value(const char* ATitle, const char* AText, int AVal=0, int AMin=0, int AMax=100, int AStep=1) {
  char temp[256];
  S3_Strcpy(s3_zenity_buffer,"zenity --scale");
  S3_Strcat(s3_zenity_buffer," --text='");       S3_Strcat(s3_zenity_buffer,AText); S3_Strcat(s3_zenity_buffer,"'");
  S3_Strcat(s3_zenity_buffer," --value='");      S3_Strcat(s3_zenity_buffer, S3_IntToString(temp,AVal));  S3_Strcat(s3_zenity_buffer,"'");
  S3_Strcat(s3_zenity_buffer," --min-value='");  S3_Strcat(s3_zenity_buffer, S3_IntToString(temp,AMin));  S3_Strcat(s3_zenity_buffer,"'");
  S3_Strcat(s3_zenity_buffer," --max-value='");  S3_Strcat(s3_zenity_buffer, S3_IntToString(temp,AMax));  S3_Strcat(s3_zenity_buffer,"'");
  S3_Strcat(s3_zenity_buffer," --step='");       S3_Strcat(s3_zenity_buffer, S3_IntToString(temp,AStep)); S3_Strcat(s3_zenity_buffer,"'");
  //S3_Strcat(s3_zenity_buffer," --print-partial");
  //S3_Strcat(s3_zenity_buffer," --hide-value");
  return s3_do_zenity();
}

const char* SDialog_Warning(const char* ATitle, const char* AText) {
  S3_Strcpy(s3_zenity_buffer,"zenity --warning");
  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);    S3_Strcat(s3_zenity_buffer,"'");
  return s3_do_zenity();
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
//  S3_Strcpy(s3_zenity_buffer,"zenity --error");
//  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);    S3_Strcat(s3_zenity_buffer,"'");
//  return s3_do_zenity();
  return S3_NULL;
}

const char* SDialog_FileOpen(const char* ATitle, bool AMultiple=false) {
//  S3_Strcpy(s3_zenity_buffer,"zenity --file-selection");
//  S3_Strcat(s3_zenity_buffer," --title='");    S3_Strcat(s3_zenity_buffer,ATitle);    S3_Strcat(s3_zenity_buffer,"'");
//  if (AMultiple) {
//    S3_Strcat(s3_zenity_buffer," --multiple");
//    S3_Strcat(s3_zenity_buffer," --separator=':'");
//  }
//  return s3_do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_FileSave(const char* ATitle, const char* AFilename) {
//  S3_Strcpy(s3_zenity_buffer,"zenity --file-selection");
//  S3_Strcat(s3_zenity_buffer," --save");
//  S3_Strcat(s3_zenity_buffer," --title='");    S3_Strcat(s3_zenity_buffer,ATitle);    S3_Strcat(s3_zenity_buffer,"'");
//  S3_Strcat(s3_zenity_buffer," --filename='"); S3_Strcat(s3_zenity_buffer,AFilename); S3_Strcat(s3_zenity_buffer,"'");
//  return s3_do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Info(const char* ATitle, const char* AText) {
//  S3_Strcpy(s3_zenity_buffer,"zenity --info");
//  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);    S3_Strcat(s3_zenity_buffer,"'");
//  return s3_do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Question(const char* ATitle, const char* AText) {
//  S3_Strcpy(s3_zenity_buffer,"zenity --question");
//  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);    S3_Strcat(s3_zenity_buffer,"'");
//  return s3_do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Text(const char* ATitle, const char* AText, const char* AEntryText) {
//  S3_Strcpy(s3_zenity_buffer,"zenity --entry");
//  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);            S3_Strcat(s3_zenity_buffer,"'");
//  S3_Strcat(s3_zenity_buffer," --entry-text='");    S3_Strcat(s3_zenity_buffer,AEntryText); S3_Strcat(s3_zenity_buffer,"'");
//  return s3_do_zenity();
  return S3_NULL;
}

// segmentation fault

const char* S3_Dialog_TextInfo(const char* ATitle, const char* AFilename, bool AEditable=false, bool AHtml=false, const char* AUrl=S3_NULL) {
//  S3_Strcpy(s3_zenity_buffer,"zenity --text-info");
//  S3_Strcat(s3_zenity_buffer," --filename='");       S3_Strcat(s3_zenity_buffer,AFilename);   S3_Strcat(s3_zenity_buffer,"'");
//  if (AEditable) S3_Strcat(s3_zenity_buffer," --editable");
//  if (AHtml) S3_Strcat(s3_zenity_buffer," --html");
//  if (AUrl) S3_Strcat(s3_zenity_buffer," --url='");  S3_Strcat(s3_zenity_buffer,AUrl);        S3_Strcat(s3_zenity_buffer,"'");
//  return s3_do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Value(const char* ATitle, const char* AText, int AVal=0, int AMin=0, int AMax=100, int AStep=1) {
//  char temp[256];
//  S3_Strcpy(s3_zenity_buffer,"zenity --scale");
//  S3_Strcat(s3_zenity_buffer," --text='");       S3_Strcat(s3_zenity_buffer,AText); S3_Strcat(s3_zenity_buffer,"'");
//  S3_Strcat(s3_zenity_buffer," --value='");      S3_Strcat(s3_zenity_buffer, SIntToString(temp,AVal));  S3_Strcat(s3_zenity_buffer,"'");
//  S3_Strcat(s3_zenity_buffer," --min-value='");  S3_Strcat(s3_zenity_buffer, SIntToString(temp,AMin));  S3_Strcat(s3_zenity_buffer,"'");
//  S3_Strcat(s3_zenity_buffer," --max-value='");  S3_Strcat(s3_zenity_buffer, SIntToString(temp,AMax));  S3_Strcat(s3_zenity_buffer,"'");
//  S3_Strcat(s3_zenity_buffer," --step='");       S3_Strcat(s3_zenity_buffer, SIntToString(temp,AStep)); S3_Strcat(s3_zenity_buffer,"'");
//  //S3_Strcat(s3_zenity_buffer," --print-partial");
//  //S3_Strcat(s3_zenity_buffer," --hide-value");
//  return s3_do_zenity();
  return S3_NULL;
}

const char* S3_Dialog_Warning(const char* ATitle, const char* AText) {
//  S3_Strcpy(s3_zenity_buffer,"zenity --warning");
//  S3_Strcat(s3_zenity_buffer," --text='");    S3_Strcat(s3_zenity_buffer,AText);    S3_Strcat(s3_zenity_buffer,"'");
//  return s3_do_zenity();
  return S3_NULL;
}

#endif // S3_WIN32

//#endif // 0

//----------------------------------------------------------------------
#endif
