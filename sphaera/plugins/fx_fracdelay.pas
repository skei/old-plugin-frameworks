{$include sphaera.inc}
unit fx_fracdelay;

{
  20 hz = 44100/20 = 2205
}

{$define S_MAX_SAMPLES := 4096}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin;

type

  myPlugin = class(SPlugin)
  private
    buf : array[0..S_MAX_SAMPLES*2*2] of Single;
    pos : longint;
    size : single;
    frac : single;
    z0 : single;
    z00 : single;
    z01 : single;
    //
    dly,fbk,wet,dry : single;

  public
    procedure   on_create; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_midiEvent(AOffset: LongWord; AMsg1, AMsg2, AMsg3: Byte); override;
    procedure   on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Math,
  s_parameter,
  s_util;

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_fracdelay';
  FAuthor := 'th.skei';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0101;
  SSetFlag(FFlags,spf_perSample+spf_receiveMidi);
  FNumInputs  := 2;
  FNumOutputs := 2;
  //
  appendParameter( SParamFloat.create( 'delay (spl)', 100, 0,  S_MAX_SAMPLES ));
  appendParameter( SParamFloat.create( 'feedback',    0.9, 0,  1 ));
  appendParameter( SParamFloat.create( 'wet (db)',   -6,  -60, 0  ));
  appendParameter( SParamFloat.create( 'dry (db)',   -6,  -60, 0  ));
  //
  //buf  = 0;
  pos  := 0;
  size := 0;
  frac := 0;
  z00  := 0;
  z01  := 0;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
begin
  case AIndex of
    0:  begin
          dly  := AValue;
          size := floor(dly);
          frac := dly - size;
        end;
    1:  fbk := AValue;
    2:  wet := power(2,AValue/6);
    3:  dry := power(2,AValue/6);
  end;
end;

//----------

procedure myPlugin.on_midiEvent(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
var
  m,note,vel : longint;
  freq : single;
begin
  m := AMsg1 and 240;
  if m = 9*16 then
  begin
    note := AMsg2 and 127;
    freq := 440 * power(2,(note-69.0)/12.0);
    vel  := AMsg3 and 127;
    //wet  := vel/127;
    dly  := _sampleRate / freq;
    //slider1 := dly;
    //slider3 := wet;
    //sliderchange(slider1+slider3);
    size := floor(dly);
    frac := dly - size;
    pos  := 0;
    z0   := 0;
  end;
  //(m == 13*16) ? (
  //  note = msg23 & 127;
  //  vel  = (msg23/256) | 0;
  //  fbk = (note/127) - 0.001;
  //);
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1       : single;
  in0,in1 : single;
  d00,d01 : single;
  i00,i01 : single;
  pos2 : longint;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;

  pos2 := pos*2;
  in0 := spl0;
  in1 := spl1;
  d00 := buf[pos2];
  d01 := buf[pos2+1];
  if dly<1 then begin
    i00 := (in0*(1-frac)) + (d00*frac);
    i01 := (in1*(1-frac)) + (d01*frac);
  end else begin
    i00 := (d00*(1-frac)) + (z00*frac);
    i01 := (d01*(1-frac)) + (z01*frac);
  end;
  buf[pos2]   := in0 + (fbk*i00);
  buf[pos2+1] := in1 + (fbk*i01);
  z00 := d00;
  z01 := d01;
  pos += 1;
  if pos>=size then pos := 0; //-=size
  spl0 := (in0*dry) + (i00*wet);
  spl1 := (in1*dry) + (i01*wet);

  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.

