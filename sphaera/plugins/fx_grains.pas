{$include sphaera.inc}
unit fx_grains;

{$define S_MAX_GRAINS := 1024}
{$define S_BUFFERSIZE := 1024*1024}


//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin;

type

  SGrain = record
    _on  : boolean;
    _pos : single;
    _sta : single;
    _end : single;
    _dur : single;
    _spd : single;
    _ph  : single;
    _pa  : single;
    _dh  : single;
    _da  : single;
  end;

  //----------

  myPlugin = class(SPlugin)
  private
    FGrains     : array[0..S_MAX_GRAINS-1] of SGrain;
    BUFFER      : array[0..S_BUFFERSIZE-1] of single;
    master      : single;
    numgrains   : longint;
    buffersize  : longint;
    freeze      : boolean;
    graindist   : single;
    grainsize   : single;
    graindur    : single;
    grainpitch  : single;
    grainenv    : single;
    startjit    : single;
    pitchjit    : single;
    sizejit     : single;
    durjit      : single;
    index       : longint;
    countdown   : single;
  public
    procedure   on_create; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Math,
  s_parameter,
  s_random,
  s_rect,
  s_util;

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_grains';
  FAuthor := 'th.skei';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0101;
  SSetFlag(FFlags,spf_perSample+spf_receiveMidi+spf_hasEditor);
  FNumInputs  := 2;
  FNumOutputs := 2;
  FEditorRect   := rect(400,400);

  //
  appendParameter( SParamFloat.create( 'master (db)',     -6,  -60, 6    ));
  appendParameter( SParamInt.create(   'num grains',      10,   1,  100   ));
  appendParameter( SParamFloat.create( 'buf size (ms)',   1000, 1,  1000 ));
  appendParameter( SParamFloat.create( 'freeze',          0     )); // text
  appendParameter( SParamFloat.create( 'grain dist (ms)', 20,   0,  100  ));
  appendParameter( SParamFloat.create( 'grain size (ms)', 30,   1,  100  ));
  appendParameter( SParamFloat.create( 'grain dur (ms)',  300,  1,  1000 ));
  appendParameter( SParamFloat.create( 'grain pitch',     1,    0,  10   ));
  appendParameter( SParamFloat.create( 'grain env',       0 ));
  appendParameter( SParamFloat.create( 'dist jitter',     0.2 ));
  appendParameter( SParamFloat.create( 'pitch jutter',    0.2 ));
  appendParameter( SParamFloat.create( 'size jitter',     0.2 ));
  appendParameter( SParamFloat.create( 'dur jitter',      0.2 ));
  //
  index := 0;
  countdown := 0;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
begin
  case AIndex of
    0:  master      := power(2,(AValue/6));
    1:  numgrains   := trunc( AValue );
    2:  buffersize  := trunc( (AValue/1000) * _sampleRate );
    3:  if AValue > 0.5 then freeze := True else freeze := false;
    4:  graindist   := (AValue/1000) * _sampleRate;
    5:  grainsize   := (AValue/1000) * _sampleRate;
    6:  graindur    := (AValue/1000) * _sampleRate;
    7:  grainpitch  := AValue;
    8:  grainenv    := AValue;
    9:  startjit    := AValue;
    10: pitchjit    := AValue;
    11: sizejit     := AValue;
    12: durjit      := AValue;
  end;
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  i:longint;
  spl0,spl1 : single;
  in0,in1 : single;
  out0,out1 : single;
  newgrain : longint;
  gvol : single;
  dvol : single;
  startrnd : single;
  pitchrnd : single;
  sizernd : single;
  durrnd : single;
  siz,st,en,du : single;
  temp : longint;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;

  in0 := spl0;
  in1 := spl1;
  out0 := 0;
  out1 := 0;
  newgrain := -1;
  //G = GRAINS;
  //loop( numgrains,
  if numgrains > 0 then
  begin
    for i := 0 to numgrains-1 do
    begin
      if (FGrains[i]._on) then
      begin
        FGrains[i]._pos += FGrains[i]._spd;
        if FGrains[i]._pos >= FGrains[i]._end then FGrains[i]._pos := FGrains[i]._sta;
        if FGrains[i]._pos >= buffersize then FGrains[i]._pos -= buffersize;
        FGrains[i]._ph += (FGrains[i]._pa*2);
        if FGrains[i]._ph >= 2 then FGrains[i]._ph -= 2;
        FGrains[i]._dh += (FGrains[i]._da*2);
        if FGrains[i]._dh >=2 then FGrains[i]._dh -= 2;
        FGrains[i]._dur -= 1;
        if FGrains[i]._dur <= 0 then FGrains[i]._on := false;
        gvol := FGrains[i]._ph * (2-abs(FGrains[i]._ph)); // abs-neg ?
        dvol := FGrains[i]._dh * (2-abs(FGrains[i]._dh));
        temp := trunc( FGrains[i]._pos );
        temp *= 2;
        out0 += BUFFER[ temp ] * dvol * gvol;
        out1 += BUFFER[ temp+1 ] * dvol * gvol;
      end
      else newgrain := i;
    end;
  end;

  if countdown <= 0 then
  begin
    countdown := graindist;
    if newgrain > 0 then
    begin
      startrnd := 1 + (startjit * ((SRandom*2)-1));
      pitchrnd := 1 + (pitchjit * ((SRandom*2)-1));
      sizernd  := 1 + (sizejit  * ((SRandom*2)-1));
      durrnd   := 1 + (durjit   * ((SRandom*2)-1));
      siz := (grainsize*sizernd);
      st := index * startrnd;
      if st >= buffersize then  st -= buffersize;
      if st <  0 then st += buffersize;
      en := st + siz;
      if en >= buffersize then en := buffersize; // clamp
      if en <  0 then en := 0;
      du := graindur*durrnd;
      FGrains[newgrain]._on := true;
      FGrains[newgrain]._pos := st;
      FGrains[newgrain]._sta := st;
      FGrains[newgrain]._end := en;
      FGrains[newgrain]._dur := du;
      FGrains[newgrain]._spd := grainpitch * pitchrnd;
      FGrains[newgrain]._ph := 0;
      FGrains[newgrain]._pa := 1 / siz;
      FGrains[newgrain]._dh := 0;
      FGrains[newgrain]._da := 1 / du;
    end;
  end;
  countdown -= 1;
  if not freeze then
  begin
    BUFFER[index*2] := in0;
    BUFFER[index*2+1] := in1;
  end;
  index += 1;
  if index >= buffersize then index -= buffersize;

  spl0 := out0 * master;
  spl1 := out1 * master;

  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.

