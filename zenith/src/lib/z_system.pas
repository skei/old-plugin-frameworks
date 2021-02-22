unit z_system;
{$include zenith.inc}

interface
implementation
end.

(*

uses
  system;

constants

InitProc : Pointer = nil
  InitProc is a routine that can be called after all units were initialized. It can be set by units to
  execute code that can be initialized after all units were initialized.
  When setting the value of InitProc, the previous value should always be saved, and called when
  the installed initialization routine has ﬁnished executing.
IsMultiThread : longbool = false
  Indicates whether more than one thread is running in the application.
ModuleIsLib : Boolean = false
  ModuleIsLib is set by the compiler when linking a library, program or package, and determines
  whether the current module is a library (or package) (True) or program (False).

variables

uses
  SysUtils;



*)

(*

compiler defines during compilation:

Possible deﬁnes when compiling using FPC

  FPC_LINK_DYNAMIC    Deﬁned when the output will be linked dynamically. This is deﬁned when using the -XD compiler switch.
  FPC_LINK_STATIC     Deﬁned when the output will be linked statically. This is the default mode.
  FPC_LINK_SMART      Deﬁned when the output will be smartlinked. This is deﬁned when using the -XX compiler switch.
  FPC_PROFILE         Deﬁned when proﬁling code is added to program. This is deﬁned when using the -pg compiler switch.
  FPC_CROSSCOMPILING  Deﬁned when the target OS/CPU is different from the source OS/CPU.
  FPC                 Always deﬁned for Free Pascal.
  VER2                Always deﬁned for Free Pascal version 2.x.x.
  VER2_0              Always deﬁned for Free Pascal version 2.0.x.
  VER2_2              Always deﬁned for Free Pascal version 2.2.x.
  ENDIAN_LITTLE       Deﬁned when the Free Pascal target is a little-endian processor (80x86, Alpha, ARM).
  ENDIAN_BIG          Deﬁned when the Free Pascal target is a big-endian processor (680x0, PowerPC, SPARC, MIPS).
  FPC_DELPHI          Free Pascal is in Delphi mode, either using compiler switch -MDelphi or using the $MODE DELPHI directive.
  FPC_OBJFPC          Free Pascal is in OBJFPC mode, either using compiler switch -Mobjfpc or using the $MODE OBJFPC directive.
  FPC_TP              Free Pascal is in Turbo Pascal mode, either using compiler switch -Mtp or using the $MODE TP directive.
  FPC_GPC             Free Pascal is in GNU Pascal mode, either using compiler switch -SP or using the $MODE GPC directive.

Possible CPU (target) deﬁnes when compiling using FPC

  CPU86         Intel 80x86 or compatible.
  CPU87         Intel 80x86 or compatible.
  CPU386        Intel 80386 or later.
  CPUI386       Intel 80386 or later.
  CPU68K        Motorola 680x0 or compatible.
  CPUM68K       Motorola 680x0 or compatible.
  CPUM68020     Motorola 68020 or later.
  CPU68         Motorola 680x0 or compatible.
  CPUSPARC32    SPARC v7 or compatible.
  CPUSPARC      SPARC v7 or compatible.
  CPUALPHA      Alpha AXP or compatible.
  CPUPOWERPC    32-bit or 64-bit PowerPC or compatible.
  CPUPOWERPC32  32-bit PowerPC or compatible.
  CPUPOWERPC64  64-bit PowerPC or compatible.
  CPUX86_64     AMD64 or Intel 64-bit processor.
  CPUAMD64      AMD64 or Intel 64-bit processor.
  CPUIA64       Intel itanium 64-bit processor.
  CPUARM        ARM 32-bit processor.
  CPUAVR        AVR 16-bit processor.
  CPU16         16-bit CPU.
  CPU32         32-bit CPU.
  CPU64         64-bit CPU.

Possible FPU deﬁnes when compiling using FPC

  FPUSOFT     Software emulation of FPU (all types).
  FPUSSE64    SSE64 FPU on Intel I386 and higher, AMD64.
  FPUSSE      SSE instructions on Intel I386 and higher.
  FPUSSE2     SSE 2 instructions on Intel I386 and higher.
  FPUSSE3     SSE 3 instructions on Intel I386 and higher, AMD64.
  FPULIBGCC   GCC library FPU emulation on ARM and M68K.
  FPU68881    68881 on M68K.
  FPUFPA      FPA on ARM.
  FPUFPA10    FPA 10 on ARM.
  FPUFPA11    FPA 11 on ARM.
  FPUVFP      VFP on ARM.
  FPUX87      X87 FPU on Intel I386 and higher.
  FPUITANIUM  On Intel Itanium.
  FPUSTANDARD On PowerPC (32/64 bit).
  FPUHARD     On Sparc.

Possible deﬁnes when compiling using target O

  linux             LINUX, UNIX
  freebsd           FREEBSD, BSD, UNIX
  netbsd            NETBSD, BSD, UNIX
  sunos             SUNOS, SOLARIS, UNIX
  go32v2            GO32V2, DPMI
  os2               OS2
  Windows (all)     WINDOWS
  Windows 32-bit    WIN32, MSWINDOWS
  Windows 64-bit    WIN64, MSWINDOWS
  Windows (winCE)   WINCE, UNDER_CE, UNICO
  Classic Amiga     AMIGA
  Atari TOS         ATARI
  Classic Macintosh MAC
  PalmOS            PALMOS
  BeOS              BEOS, UNIX
  QNX RTP           QNX, UNIX
  Mac OS X          BSD, DARWIN, UNIX

*)
