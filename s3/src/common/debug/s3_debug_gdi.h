#ifndef s3_debug_gdi_included
#define s3_debug_gdi_included
//----------------------------------------------------------------------

#ifdef S3_DEBUG_GDI

#ifndef GR_GDIOBJECTS_PEAK
  #define GR_GDIOBJECTS_PEAK 2
#endif

#ifndef GR_USEROBJECTS_PEAK
  #define GR_USEROBJECTS_PEAK 4
#endif


//----------------------------------------------------------------------
// GdiTable
//----------------------------------------------------------------------
// https://github.com/panoramixor/GDIObjDump/blob/master/src/GDIObjDump/common.h

/*

typedef LONG (WINAPI NTQIP)(HANDLE,WORD,PVOID,ULONG,PULONG);

//----------

typedef struct {
  DWORD   pKernelAddress;
  WORD    wProcessId;
  WORD    wCount;
  WORD    wUpper;
  WORD    wType;
  DWORD   pUserAddress;
} GDITableEntry32;

typedef struct
{
  PVOID64 pKernelAddress;
  USHORT  wProcessId;
  USHORT  wCount;
  USHORT  wUpper;
  USHORT  wType;
  PVOID64 pUserAddress;
} GDITableEntry64;

#ifdef S3_32BIT
  typedef GDITableEntry32 GDITableEntry;
#endif

#ifdef S3_64BIT
  typedef GDITableEntry64 GDITableEntry;
#endif

typedef struct {
  ULONG       Length;
  BOOLEAN     Initialized;
  PVOID       SsHandle;
  LIST_ENTRY  InLoadOrderModuleList;
  LIST_ENTRY  InMemoryOrderModuleList;
  LIST_ENTRY  InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct PEB { // shortened
  char            Glob1[12];
  PPEB_LDR_DATA   LoaderData;           // 16 bytes to here
  char            Glob2[132];           // = 152 - sizeof(PVOID) - 16
  GDITableEntry*  GdiSharedHandleTable; // 152 bytes to here
} PEB;

typedef struct {
  PVOID Reserved1;
  PEB*  PebBaseAddress;
  PVOID Reserved2[2];
  PVOID UniqueProcessId;
  PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;

*/

//----------------------------------------------------------------------

class S3_Debug_Gdi {

  /*
  private:
    NTQIP* lpfnNTQuery;
  */

  private:
    HANDLE MProcess;
    uint32 MProcessId;
    uint32 MStartGdiObjects;
    uint32 MStartUserObjects;
    uint32 MStartGdiObjects_Peak;
    uint32 MStartUserObjects_Peak;
    uint32 MStopGdiObjects;
    uint32 MStopUserObjects;
    uint32 MStopGdiObjects_Peak;
    uint32 MStopUserObjects_Peak;

  private:


    //XP SP2 randomizes PEB address so have to use this
    //to support it. Default addr at bottom wont work that case.

    /*
    PEB* GetPEBAddress(HANDLE hProc) {
      PROCESS_BASIC_INFORMATION pbi;
      DWORD dwSize;
      if (lpfnNTQuery!=NULL) {
        (*lpfnNTQuery)(hProc, 0, &pbi, sizeof(pbi), &dwSize);
        return pbi.PebBaseAddress ;
      }
      return (PEB*)0x7ffdf000;
    }
    */

  public:

    S3_Debug_Gdi() {
      //lpfnNTQuery             = (NTQIP *)GetProcAddress(GetModuleHandle("ntdll.dll"),"NtQueryInformationProcess");
      MProcess                = GetCurrentProcess();
      MProcessId              = GetCurrentProcessId();
      MStartGdiObjects        = GetGuiResources(MProcess,GR_GDIOBJECTS);        // 0
      MStartUserObjects       = GetGuiResources(MProcess,GR_USEROBJECTS);       // 1
      MStartGdiObjects_Peak   = GetGuiResources(MProcess,GR_GDIOBJECTS_PEAK);   // 2
      MStartUserObjects_Peak  = GetGuiResources(MProcess,GR_USEROBJECTS_PEAK);  // 4
      dumpCaps();
    }

    ~S3_Debug_Gdi() {
      MStopGdiObjects       = GetGuiResources(MProcess,GR_GDIOBJECTS);
      MStopUserObjects      = GetGuiResources(MProcess,GR_USEROBJECTS);
      MStopGdiObjects_Peak  = GetGuiResources(MProcess,GR_GDIOBJECTS_PEAK);
      MStopUserObjects_Peak = GetGuiResources(MProcess,GR_USEROBJECTS_PEAK);
      dump();
      //dumpGdiTable();
    }

    uint32 gdi_objects(void)  { return GetGuiResources(MProcess,GR_GDIOBJECTS); }
    uint32 user_objects(void) { return GetGuiResources(MProcess,GR_USEROBJECTS); }

    void dump(void) {
      S3_DTrace("\n----------------------------------------\n");
      S3_DTrace("GDI objects\n");
      S3_DTrace("----------------------------------------\n");
      S3_DTrace("init:\n");
      S3_DTrace("  gdi objects: %i\n",MStartGdiObjects);
      S3_DTrace("  user objects: %i\n",MStartUserObjects);
      S3_DTrace("  peak gdi objects: %i\n",MStartGdiObjects_Peak);
      S3_DTrace("  peak user objects: %i\n",MStartUserObjects_Peak);
      S3_DTrace("exit:\n");
      S3_DTrace("  gdi objects: %i\n",MStopGdiObjects);
      S3_DTrace("  user objects: %i\n",MStopUserObjects);
      S3_DTrace("  peak gdi objects: %i\n",MStopGdiObjects_Peak);
      S3_DTrace("  peak user objects: %i\n",MStopUserObjects_Peak);
      S3_DTrace("----------------------------------------\n\n");
    }

    /*
    void dumpGdiTable(void) {
      PEB* p = GetPEBAddress(MProcess);
      GDITableEntry* gditable = p->GdiSharedHandleTable;
      S3_DTrace("\n----------------------------------------\n");
      S3_DTrace("GDI Table (ProcessId:%i)\n",MProcessId);
      S3_DTrace("----------------------------------------\n");
      uint32 num = GetGuiResources(MProcess,GR_GDIOBJECTS);
      for (uint32 i=0; i<num; i++) {
        S3_DTrace("%i) KernelAddress:%i ProcessID:%i Count:%i Upper:%i Type:%i UserAddress:%i\n",
          i,
          gditable[i].pKernelAddress,
          gditable[i].wProcessId,
          gditable[i].wCount,
          gditable[i].wUpper,
          gditable[i].wType,
          gditable[i].pUserAddress
        );
      }
      S3_DTrace("----------------------------------------\n\n");
    }
    */

    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd144877%28v=vs.85%29.aspx

    void dumpCaps(void) {
      HDC tempdc = GetDC(S3_NULL); // dc for the entire screen
      S3_DTrace("\n----------------------------------------\n");
      S3_DTrace("GDI GetDeviceCaps (screen)\n");
      S3_DTrace("----------------------------------------\n");
      S3_DTrace("  HORZRES        %i\n",GetDeviceCaps(tempdc,HORZRES));
      S3_DTrace("  VERTRES        %i\n",GetDeviceCaps(tempdc,VERTRES));
      S3_DTrace("  BITSPIXEL      %i\n",GetDeviceCaps(tempdc,BITSPIXEL));
      S3_DTrace("  PLANES         %i\n",GetDeviceCaps(tempdc,PLANES));
      S3_DTrace("  VREFRESH       %i\n",GetDeviceCaps(tempdc,VREFRESH));
      S3_DTrace("  BLTALIGNMENT   %i\n",GetDeviceCaps(tempdc,BLTALIGNMENT));
      S3_DTrace("  SHADEBLENDCAPS %x\n",GetDeviceCaps(tempdc,SHADEBLENDCAPS));
      S3_DTrace("  RASTERCAPS     %08x\n",GetDeviceCaps(tempdc,RASTERCAPS));
      S3_DTrace("  CURVECAPS      %08x\n",GetDeviceCaps(tempdc,CURVECAPS));
      S3_DTrace("  LINECAPS       %08x\n",GetDeviceCaps(tempdc,LINECAPS));
      S3_DTrace("  POLYGONALCAPS  %08x\n",GetDeviceCaps(tempdc,POLYGONALCAPS));
      S3_DTrace("  TEXTCAPS       %08x\n",GetDeviceCaps(tempdc,TEXTCAPS));
      S3_DTrace("----------------------------------------\n\n");
      ReleaseDC(0,tempdc);
    }

};

//----------

static S3_Debug_Gdi s3_debug_gdi;

//----------

#endif // S3_DEBUG_GDI

//----------------------------------------------------------------------
#endif

























// http://www.delphigroups.info/2/11/636394.html


/*

unit LeakingBitMaps;
(*
  This unit save the ''leaking'' bitmaps of your application in a specified
  directory. Include this unit in your project so that its ''finalization''
  code is executed just before your application terminates.
  The finalization routine writes a log file containing information about
  the leaking bitmaps. It also writes the leaking bitmap as files.
  In Windows Explorer, use ''view thumbnails'' to see them all at once!
  The ''SaveToFile'' procedure may raise exceptions ('Out of system
  resource...',
  'Not enough storage...'), just ignore them.
  You'll have to specify your log file name and a directory for the bitmap
  files.
  Works on W2K and probably on XP.
  Christian Aymon
  www.cyamon.com
  *)

interface

uses
  Windows;

const
  // CHANGE THESE TWO CONSTANTS
  LogFileName = 'd:\aaa-scratch\aaa.txt';
  BitMapsDirectory = 'd:\aaa-bitmaps';

const
  EntriesQty = 16384;

type

  TGdiEntry = record
    pKernelInfo : DWORD;
    ProcessId : WORD;
    Count : WORD;
    Upper : WORD;
    HandleType : WORD;
    pUserInfo : DWORD;
  end;

  TGdiTable = array [0..EntriesQty-1] of TGdiEntry;
  PGdiTable = ^TGdiTable;

function GdiQueryTable : PGdiTable; stdcall;

implementation

uses
  SysUtils,
  Graphics;

function GdiQueryTable; external gdi32 name 'GdiQueryTable'; procedure P;

var
  Table : PGdiTable;
  LogFile : TextFile;
  BitMapQty : integer;
  i : integer;
  H : THandle;
  B : Windows.TBitMap;
  BitMap : TBitMap;

begin
  Table := GdiQueryTable;
  AssignFile(LogFile, LogFileName);
  Rewrite(LogFile);
  Writeln(LogFile, Format('ProcessId = %d', [GetCurrentProcessId]));
  BitMapQty := 0;
  try
    for i := 0 to EntriesQty-1 do
    begin
      with Table^[i] do
      begin
        if (ProcessId = GetCurrentProcessId) and (HandleType = 5) and (pKernelInfo>= $80000000) then
        begin
          inc(BitMapQty);
          H := Upper;
          H := (H shl 16) + i;
          if GetObject(H, SizeOf(B), @B) <>0 then
          begin
            Writeln(LogFile, Format('N=%d, Handle=%s, W=%d,H=%d',[i, IntToHex(H, 8), B.bmWidth, B.bmHeight]));
            Bitmap := TBitMap.Create;
            try
              BitMap.Handle := H;
              try
                Bitmap.SaveToFile(Format('%s\%s.bmp',[BitMapsDirectory, IntToHex(H, 8)]));
              except
                end {try};
              finally
                Bitmap.Free;
            end {try};
          end {if};
        end {if};
      end {with};
    end {for};
    Writeln(LogFile, Format('BitMapQty=%d', [BitMapQty]));
  finally
    CloseFile(LogFile);
  end {try};
end;

initialization
finalization
  P;
end.

*/
