unit z_freeverb;
{$include zenith.inc}

{

  This file is packaged with HuMuS. It is licensed under the
  3-clause BSD license:

  Copyright (c) 2009, HuMuS Team
  (Maintainer: Roland Schaefer <mail[at]rolandschaefer.net>)
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


  The work in this file is built upon a Pascal unit which has an unspecified
  free license, which, according to the author, does not prevent
  commercial use.  The 3-clause BSD license, which is as permissive as the
  author intended his unspecified license to be, joins this code into the GPL-
  licensed HuMuS code.
  Usage in HuMuS was approved by author Thaddy de Koning on 2009-09-23.
  The orignal license header follows for reference.

       Unit: Reverb for either KOL or VCL Delphi and Freepascal 1.9.x,
             probably Kylix.
    purpose: Based on the Freeverb (C++) design by Jezar[at]dreampoint.co.uk
     Author: Thaddy de Koning, thaddy[at]thaddy.com
  Copyright: Original in C++ �2000, Jezar
             Delphi and Freepascal version �2003,
             Basm  added �2004, Thaddy de Koning
             Use as you like, copyrighted freeware.

}

//{$MODE OBJFPC}
//{$ASMMODE INTEL}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

//uses
//  Classes;

const

  { Reverb model tuning values, taken from original algoritm by Jezar }
  KdeNorm : Single = 1.0e-23;
  NumCombs      = 8;
  NumAllPasses  = 4;
  Muted         = 0;
  FixedGain     = 0.015;
  ScaleWet      = 3;
  ScaleDry      = 2;
  ScaleDamp     = 0.4;
  ScaleRoom     = 0.28;
  OffsetRoom    = 0.7;
  InitialRoom   = 0.5;
  InitialDamp   = 0.5;
  InitialWet    = 1 / ScaleWet;
  InitialDry    = 1 / ScaleDry;
  InitialWidth  = 1;
  InitialMode   = 0;
  FreezeMode    = 0.5;
  StereoSpread  = 23;

  {
    These values assume 44.1KHz sample rate
    they will probably be OK for 48KHz sample rate
    but would need scaling for 96KHz (or other) sample rates.
    The values were obtained by listening tests.
  }

  CombTuningL1    = 1116;
  CombTuningR1    = 1116 + StereoSpread;
  CombTuningL2    = 1188;
  CombTuningR2    = 1188 + StereoSpread;
  CombTuningL3    = 1277;
  CombTuningR3    = 1277 + StereoSpread;
  CombTuningL4    = 1356;
  CombTuningR4    = 1356 + StereoSpread;
  CombTuningL5    = 1422;
  CombTuningR5    = 1422 + StereoSpread;
  CombTuningL6    = 1491;
  CombTuningR6    = 1491 + StereoSpread;
  CombTuningL7    = 1557;
  CombTuningR7    = 1557 + StereoSpread;
  CombTuningL8    = 1617;
  CombTuningR8    = 1617 + StereoSpread;
  AllPassTuningL1 = 556;
  AllPassTuningR1 = 556 + StereoSpread;
  AllPassTuningL2 = 441;
  AllPassTuningR2 = 441 + StereoSpread;
  AllPassTuningL3 = 341;
  AllPassTuningR3 = 341 + StereoSpread;
  AllPassTuningL4 = 225;
  AllPassTuningR4 = 225 + StereoSpread;

type

  TAllPass = class//(TObject)
  private
    Feedback    : Single;
    Buffer      : Pointer;
    BufSize,
    BufIdx      : integer;
  public
    constructor Create(BufferSize: integer); virtual;
    destructor  Destroy; override;
    function    Process(const Input: Single): Single; register;
    procedure   Mute;
    procedure   SetFeedBack(Value: Single);
    function    GetFeedback: Single;
  end;

  //----------

  TComb = class//(TObject)
  private
    Feedback,
    FilterStore,
    Damp1,
    Damp2       : Single;
    Buffer      :pointer;
    BufSize,
    BufIdx      : integer;
    //FTube       : Boolean;
  public
    constructor Create(BufferSize: integer); virtual;
    destructor  Destroy; override;
    function    Process(const Input: Single): Single;register;
    procedure   Mute;
    procedure   SetDamp(Value: Single);
    function    GetDamp: Single;
    procedure   SetFeedBack(Value: Single);
    function    GetFeedback: Single;
  end;

  //----------

  TReverb = class//(TComponent)
  private
    {FSamplerate: Single;
    Isband: boolean;
    Freq,
    FQual,
    Ftype: Single;}
    Gain,
    RoomSize,
    RoomSize1,
    Damp,
    Damp1,
    Wet,
    Wet1,
    Wet2,
    Dry,
    Width,
    Mode        : Single;
    CombL       : array [0..NumCombs - 1] of TComb;
    CombR       : array [0..NumCombs - 1] of TComb;
    AllPassL    : array [0..NumAllPasses - 1] of TAllPass;
    AllPassR    : array [0..NumAllPasses - 1] of TAllPass;
  protected
    procedure   Update;
  public
    constructor Create{(aOwner:Tcomponent)};//override;
    destructor  Destroy; override;
    procedure   Mute;
    procedure   ProcessMix(const InputL: array of Single;
                           const InputR: array of Single;
                           var OutputL: array of Single;
                           var OutputR: array of Single;
                           NumSamples: integer);
    procedure   ProcessReplace(const InputL: array of Single;
                               const InputR: array of Single;
                               var OutputL: array of Single;
                               var OutputR: array of Single;
                               NumSamples: integer);
    procedure   SetRoomSize(Value: Single);
    function    GetRoomSize: Single;
    procedure   SetDamp(Value: Single);
    function    GetDamp: Single;
    procedure   SetWet(Value: Single);
    function    GetWet: Single;
    procedure   SetDry(Value: Single);
    function    GetDry: Single;
    procedure   SetWidth(Value: Single);
    function    GetWidth: Single;
    procedure   SetMode(Value: Single);
    function    GetMode: Single;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//------------------------------
// AllPass
//------------------------------

constructor TAllPass.Create(Buffersize: integer);
begin
  inherited Create;
  BufSize := Buffersize * 4;
  Buffer := AllocMem(BufSize);
  BufIdx := 0;
end;

//----------

destructor TAllPass.Destroy;
begin
  FreeMem(Buffer);
  inherited;
end;

//----------

function TAllPass.GetFeedback: Single;
begin
  Result := Feedback;
end;

//----------

procedure TAllPass.Mute;
begin
  Fillchar(Buffer^,BufSize, 0);
end;

//----------

function TAllPass.Process(const Input: Single): Single; assembler;
asm
  mov  ecx, [eax].buffer                   // buffer start in ecx
  mov  edx, [eax].BufIdx                   // buffer index in edx
  fld  Input
  {
    This checks for very small values that can cause a processor
    to switch in extra precision mode, which is expensive.
    Since such small values are irrelevant to audio, avoid this.
    The code is equivalent to the C inline macro by Jezar.
    This is the same spot where the original C macro appears.
  }
  test dword ptr [ecx+edx], $7F800000      // test if denormal
  jnz @Normal
  mov dword ptr [ecx+edx], 0               // if so, zero out
@normal:
  fld  [ecx+edx].Single                    // load current sample from buffer
  fsub st(0), st(1)                        // subtract input sample
  { NOT fsub, because delphi 7 translates that into fsubp! }
  fxch                                    // this is a zero cycle operant,
                                          // just renames the stack internally
  fmul [eax].feedback                     // multiply stored sample with feedback
  fadd Input                              // and add the input
  fstp [ecx + edx].Single;                // store at the current sample pos
  add  edx, 4                             // increment sample position
  cmp  edx, [eax].BufSize;                // are we at end of buffer?
  jb   @OK
  xor  edx, edx                           // if so, reset buffer index
@OK:
  mov  [eax].BufIdx, edx                  // and store new index,
                                          // result already in st(0),
                                          // hence the fxch
end;

//----------

procedure TAllPass.SetFeedBack(Value: Single);
begin
  Feedback := Value;
end;

//------------------------------
// Comb
//------------------------------

constructor TComb.Create(Buffersize: integer);
begin
  inherited Create;
  BufSize := Buffersize * 4;
  Buffer:=AllocMem(BufSize);
  FilterStore := 0;
  BufIdx := 0;
end;

//----------

destructor TComb.Destroy;
begin
  FreeMem(Buffer);
  inherited;
end;

//----------

function TComb.GetDamp: Single;
begin
  Result := Damp1;
end;

//----------

function TComb.GetFeedback: Single;
begin
  Result := Feedback;
end;

//----------

procedure TComb.Mute;
begin
  Fillchar(Buffer^, BufSize, 0);
end;

//----------

function TComb.Process(const Input: Single): Single; assembler;
asm
  mov   ecx, [eax].Buffer                        // buffer start in ecx
  mov   edx, [eax].BufIdx                        // buffer index in edx
  {
    This checks for very small values that can cause a processor
    to switch in extra precision mode, which is expensive.
    Since such small values are irrelevant to audio, avoid this.
    The code is equivalent to the C inline macro by Jezar.
    This is the same spot where the original C macro appears.
  }
  test  dword ptr [ecx+edx], $7F800000           // test if denormal
  jnz   @Normal
  mov   dword ptr [ecx+edx], 0                   // if so, zero out
@normal:
  fld   [ecx+edx].Single;                        // load sample from buffer
  fld   st(0)                                    // duplicate on the stack
  fmul  [eax].Damp2                              // multiply with Damp2
  fld   [eax].FilterStore;                       // load stored filtered sample
  fmul  [eax].Damp1                              // multiply with Damp1
  // Warning: "faddp" without operand translated into "faddp %st,%st(1)"
  {%H-}faddp
  // fadd  Kdenorm
  fst   [eax].FilterStore                        // store it back
  {
    This checks for very small values that can cause a processor
    to switch in extra precision mode, which is expensive.
    Since such small values are irrelevant to audio, avoid this.
    The code is equivalent to the C inline macro by Jezar.
    This is the same spot where the original C macro appears.
  }
  test  dword ptr [eax].FilterStore, $7F800000   // test if denormal
  jnz   @Normal2
  mov   dword ptr [eax].FilterStore, 0           // if so, zero out
@normal2:
  fmul  [eax].feedback                           // multiply with feedback
  fadd  Input                                    // and add to input sample
  //fadd  Kdenorm
  fstp  [ecx+edx].Single                         // store at current buffer pos
  add   edx, 4                                   // update buffer index
  cmp   edx, [eax].BufSize;                      // end of buffer reached?
  jb    @OK
  xor   edx, edx                                 // if so, reset buffer index
@OK:
  mov  [eax].BufIdx, edx                         // and store new index.
                                                 // result already in st(0),
                                                 // hence duplicate
end;

//----------

procedure TComb.SetDamp(Value: Single);
begin
  Damp1 := Value;
  Damp2 := 1 - Value;
end;

//----------

procedure TComb.SetFeedBack(Value: Single);
begin
  Feedback := Value;
end;

//----------------------------------------------------------------------
// Reverb
//----------------------------------------------------------------------

constructor TReverb.Create{(AOwner:Tcomponent)};
begin
  inherited {Create(AOwner)};
  CombL[0] := TComb.Create(CombTuningL1);
  CombR[0] := TComb.Create(CombTuningR1);
  CombL[1] := TComb.Create(CombTuningL2);
  CombR[1] := TComb.Create(CombTuningR2);
  CombL[2] := TComb.Create(CombTuningL3);
  CombR[2] := TComb.Create(CombTuningR3);
  CombL[3] := TComb.Create(CombTuningL4);
  CombR[3] := TComb.Create(CombTuningR4);
  CombL[4] := TComb.Create(CombTuningL5);
  CombR[4] := TComb.Create(CombTuningR5);
  CombL[5] := TComb.Create(CombTuningL6);
  CombR[5] := TComb.Create(CombTuningR6);
  CombL[6] := TComb.Create(CombTuningL7);
  CombR[6] := TComb.Create(CombTuningR7);
  CombL[7] := TComb.Create(CombTuningL8);
  CombR[7] := TComb.Create(CombTuningR8);
  AllPassL[0] := TAllPass.Create(AllPassTuningL1);
  AllPassR[0] := TAllPass.Create(AllPassTuningR1);
  AllPassL[1] := TAllPass.Create(AllPassTuningL2);
  AllPassR[1] := TAllPass.Create(AllPassTuningR2);
  AllPassL[2] := TAllPass.Create(AllPassTuningL3);
  AllPassR[2] := TAllPass.Create(AllPassTuningR3);
  AllPassL[3] := TAllPass.Create(AllPassTuningL4);
  AllPassR[3] := TAllPass.Create(AllPassTuningR4);
  { Set default values }
  AllPassL[0].SetFeedBack(0.5);
  AllPassR[0].SetFeedBack(0.5);
  AllPassL[1].SetFeedBack(0.5);
  AllPassR[1].SetFeedBack(0.5);
  AllPassL[2].SetFeedBack(0.5);
  AllPassR[2].SetFeedBack(0.5);
  AllPassL[3].SetFeedBack(0.5);
  AllPassR[3].SetFeedBack(0.5);
  SetWet(InitialWet);
  SetRoomSize(initialroom);
  SetDry(InitialDry);
  SetDamp(InitialDamp);
  SetWidth(InitialWidth);
  SetMode(InitialMode);
  Mute;
end;

//----------

destructor TReverb.Destroy;
var
  i: integer;
begin
  for i := 0 to 3 do
  begin
    AllPassL[i].Free;
    AllPassR[i].Free;
  end;
  for i := 0 to 7 do
  begin
    CombR[i].Free;
    CombL[i].Free;
  end;
  inherited Destroy;
end;

//----------

function TReverb.GetDamp: Single;
begin
  Result := Damp / ScaleDamp;
end;

//----------

function TReverb.GetDry: Single;
begin
  Result := Dry / ScaleDry;
end;

//----------

function TReverb.GetMode: Single;
begin
  if Mode >= FreezeMode then
    Result := 1
  else
    Result := 0;
end;

//----------

function TReverb.GetRoomSize: Single;
begin
  Result := (RoomSize - OffsetRoom) / ScaleRoom;
end;

//----------

function TReverb.GetWet: Single;
begin
  Result := Wet / ScaleWet;
end;

//----------

function TReverb.GetWidth: Single;
begin
  Result := Width;
end;

//----------

procedure TReverb.Mute;
var
  i: integer;
begin
  if GetMode >= FreezeMode then Exit;

  for i := 0 to NumCombs - 1 do
  begin
    CombL[i].Mute;
    CombR[i].Mute;
  end;
  for i := 0 to NumAllPasses - 1 do
  begin
    AllPassL[i].Mute;
    AllPassR[i].Mute;
  end;
end;

//----------

procedure TReverb.ProcessMix(const InputL: array of Single;
  const InputR: array of Single;
  var OutputL: array of Single;
  var OutputR: array of Single;
  NumSamples: integer);
var
  OutL, OutR, Input: Single;
  i, j: integer;
begin
  for i := 0 to NumSamples - 1 do
  begin
    OutL := 0;
    OutR := 0;
    OutL := InputL[i];
    OutR := InputR[i];
    Input := (InputL[i] + InputR[i]) * Gain;
    {  Accumulate comb filters in parallel }
    for j := 0 to NumCombs - 1 do
    begin
      OutL := OutL + CombL[j].Process(Input);
      OutR := OutR + CombR[j].Process(Input);
    end;
    { Feed through AllPasses in series }
    for j := 0 to NumAllPasses - 1 do
    begin
      OutL := AllPassL[j].Process(OutL);
      OutR := AllPassR[j].Process(OutR);
    end;
    { Calculate output MIXING with anything already there }
    OutputL[i] := OutputL[i] + OutL * Wet1 +
      OutR * Wet2 + InputL[i] * Dry;
    OutputR[i] := OutputR[i] + OutR * Wet1 +
      OutL * Wet2 + InputR[i] * Dry;
    if (i mod 100) = 0 then
    begin
      Writeln('Sample ', i);
      Writeln('InputL : ', InputL[i]);
      Writeln('OutputL : ', OutputL[i]);
    end;
  end;
end;

//----------

procedure TReverb.ProcessReplace(const InputL: array of Single;
  const InputR: array of Single;
  var OutputL: array of Single;
  var OutputR: array of Single;
  NumSamples: integer);
var
  OutL, OutR, Input: Single;
  i, j: integer;
begin
  for i := 0 to NumSamples - 1 do
  begin
    OutL := 0;
    OutR := 0;
    Input := (InputL[i] + InputR[i]) * Gain;
    { Accumulate comb filters in parallel }
    for j := 0 to NumCombs - 1 do
    begin
      OutL := OutL + CombL[j].Process(Input);
      OutR := OutR + CombR[j].Process(Input);
    end;
    { Feed through AllPasses in series }
    for j := 0 to NumAllPasses - 1 do
    begin
      OutL := AllPassL[j].Process(OutL);
      OutR := AllPassR[j].Process(OutR);
    end;
    { Calculate output REPLACING anything already there }
    OutputL[i] := OutL * Wet1 + OutR * Wet2 + InputL[i] * Dry;
    OutputR[i] := OutR * Wet1 + OutL * Wet2 + InputR[i] * Dry;
  end;
end;

//----------

procedure TReverb.SetDamp(Value: Single);
begin
  Damp := Value * ScaleDamp;
  Update;
end;

//----------

procedure TReverb.SetDry(Value: Single);
begin
  Dry := Value * ScaleDry;
  Update;
end;

//----------

procedure TReverb.SetMode(Value: Single);
begin
  Mode := Value;
  Update;
end;

//----------

procedure TReverb.SetRoomSize(Value: Single);
begin
  RoomSize := (Value * ScaleRoom) + OffsetRoom;
  Update;
end;

//----------

procedure TReverb.SetWet(Value: Single);
begin
  Wet := Value * ScaleWet;
  Update;
end;

//----------

procedure TReverb.SetWidth(Value: Single);
begin
  Width := Value;
  Update;
end;

//----------

procedure TReverb.Update;
var
  i: integer;
begin
  { Recalculate internal values after parameter change }
  Wet1 := Wet * (Width / 2 + 0.5);
  Wet2 := Wet * ((1 - Width) / 2);
  if Mode >= FreezeMode then
  begin
    RoomSize1 := 1;
    Damp1 := 0;
    Gain := Muted;
  end
  else
  begin
    RoomSize1 := RoomSize;
    Damp1 := Damp;
    Gain := FixedGain;
  end;
  for i := 0 to NumCombs - 1 do
  begin
    CombL[i].SetFeedBack(RoomSize1);
    CombR[i].SetFeedBack(RoomSize1);
  end;
  for i := 0 to NumCombs - 1 do
  begin
    CombL[i].SetDamp(Damp1);
    CombR[i].SetDamp(Damp1);
  end;
end;

//----------------------------------------------------------------------
end.


