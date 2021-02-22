unit z_cpu;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

var

  hasMMX      : Boolean; // Indicates if a CPU can execute MMX Instructions.
  hasMMX2     : Boolean; // Indicates if a CPU can execute extended MMX Instructions.
  hasSSE      : Boolean; // Indicates if a CPU can execute SSE Instructions.
  hasSSE2     : Boolean; // Indicates if a CPU can execute SSE2 Instructions.
  has3DNow    : Boolean; // Indicates if a CPU can execute 3DNow Instructions.
  has3DNowExt : Boolean; // Indicates if a CPU can execute enhanced 3DNow Instructions.

// var in64 : time;
// time := readTSC;
// do all yer dsp
// time := readTSC-time;
// this now = the cpu cycle, count + 20 to 30 cycle overhead

function ZReadTSC : int64; assembler;
asm
  rdtsc;
end;

//----------

procedure CheckCPUID; assembler;
asm
  pushfd
  pop       eax
  mov       ecx, eax
  xor       eax, $200000
  push      eax
  popfd
  pushfd
  pop       eax
  cmp       eax,ecx
end;

procedure CheckMMX; assembler;
asm
  pushad
  mov       eax, $1
  db        $0F,$A2            // CPUID
  test      edx, $800000
  jz        @@End
  mov       hasMMX, 1
@@End:
  popad
  xor       eax,eax
end;

procedure CheckMMX2; assembler;
asm
  pushad
  mov       eax, $1
  db        $0F,$A2            // CPUID
  test      edx, $400000
  jz        @@CheckAMD
  mov       hasMMX2, 1
  jmp       @@End
@@CheckAMD:
  mov       eax, $80000000
  db        $0F,$A2            // CPUID
  cmp       eax, $80000000
  jbe       @@End
  mov       eax, $80000001
  db        $0F,$A2            // CPUID
  test      edx, $400000
  jz	  @@End
  mov       hasMMX2, 1
@@End:
  popad
  xor       eax,eax
end;

procedure CheckSSE; assembler;
asm
  pushad
  mov       eax, $1
  db        $0F,$A2            // CPUID
  test      edx, $2000000
  jz        @@End
  mov       hasSSE, 1
@@End:
  popad
  xor       eax,eax
end;

procedure CheckSSE2; assembler;
asm
  pushad
  mov       eax, $1
  db        $0F,$A2            // CPUID
  test      edx, $4000000
  jz        @@End
  mov       hasSSE2, 1
@@End:
  popad
  xor       eax,eax
end;

procedure Check3DNow; assembler;
asm
  pushad
  mov       eax, $80000000
  db        $0F,$A2            // CPUID
  cmp       eax, $80000000
  jbe       @@End
  mov       eax, $80000001
  db        $0F,$A2            // CPUID
  test      edx, $80000000
  jz	      @@End
  mov       has3DNow, 1
@@End:
  popad
  xor       eax,eax
end;

procedure Check3DNowExt; assembler;
asm
  pushad
  mov       eax, $80000000
  db        $0F,$A2            // CPUID
  cmp       eax, $80000000
  jbe       @@End
  mov       eax, $80000001
  db        $0F,$A2            // CPUID
  test      edx, $40000000
  jz	      @@End
  mov       has3DNowExt, 1
@@End:
  popad
  xor       eax,eax
end;

procedure CheckCPU; assembler;
asm
  mov       ecx, eax
  mov       hasMMX, 0
  mov       hasMMX2, 0
  mov       hasSSE, 0
  mov       hasSSE2, 0
  mov       has3DNow, 0
  mov       has3DNowExt, 0
  call      CheckCPUID
  jz        @@END
  Call      CheckMMX
  Call      CheckMMX2
  {$IFDEF USE_SSE}
  Call      CheckSSE
  Call      CheckSSE2
  {$ENDIF}
  Call      Check3DNow
  Call      Check3DNowExt
@@End:
end;


//----------------------------------------------------------------------
end.
