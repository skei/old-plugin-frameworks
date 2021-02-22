{$include sphaera.inc}
unit s_pitchshift;

{
  see /src/ext/smbPitchShift !
  and
  "Pitch Shifting Using The Fourier Transform"
  http://www.dspdimension.com/admin/pitch-shifting-using-the-ft/

  TODO:
    (optimize)
    - precalc windowing table, and reuse 'window' value
    - separate loops for all allowed buffer sizes (2..8192 = 13)
    - if --"-- unroll loops, duff's device (big?)
    - if --"--, get rid of consts or precalc-able, known/'const' sizes
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

const
  MAX_FRAME_LENGTH = 8192;

type

  SPitchShift = class
  private
    fftFrameSize2,stepSize: Longint;
    inFifoLatency: Longint;
    freqPerBin, expct : Double;
    window_table  : array[0..MAX_FRAME_LENGTH - 1] of Double;
    fftFrameSize : longint;
    osamp : longint;
  private
    gInFIFO       : array[0..MAX_FRAME_LENGTH - 1] of Single;
    gOutFIFO      : array[0..MAX_FRAME_LENGTH - 1] of Single;
    gFFTworksp    : array[0..2 * MAX_FRAME_LENGTH - 1] of Single;
    gLastPhase    : array[0..MAX_FRAME_LENGTH div 2] of Single;
    gSumPhase     : array[0..MAX_FRAME_LENGTH div 2] of Single;
    gOutputAccum  : array[0..2 * MAX_FRAME_LENGTH - 1] of Single;
    gAnaFreq      : array[0..MAX_FRAME_LENGTH - 1] of Single;
    gAnaMagn      : array[0..MAX_FRAME_LENGTH - 1] of Single;
    gSynFreq      : array[0..MAX_FRAME_LENGTH - 1] of Single;
    gSynMagn      : array[0..MAX_FRAME_LENGTH - 1] of Single;
    gRover        : Longint;// = 0;
    gInit         : Longint;// = 0;
  public
    constructor   create;
    destructor    destroy; override;
    procedure     reset;
    procedure     setup(fftsize,os:Longint; sampleRate:Single);
    procedure     PitchShift(pitchShift:Single; numSampsToProcess{,fftFrameSize,osamp}:Longint; {sampleRate:Single;} indata,outdata:PSingle);
  private
    procedure     smbFFT(fftBuffer:PSingle; {fftFrameSize,}sign:Longint);
    //function      smbAtan2(x,y:Double) : Double;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses Math;
{$define ATAN2:=Math.ArcTan2}
{.$define ATAN2:=smbAtan2}

//----------

constructor SPitchShift.create;
begin
  inherited;
  gRover := 0;
  gInit := 0;
end;

destructor SPitchShift.destroy;
begin
  inherited;
end;

procedure SPitchShift.reset;
begin
  gRover := 0;
  gInit := 0;
end;

//------------------------------

procedure SPitchShift.setup(fftsize,os: Longint; sampleRate:Single);
var
  i,j,k : longint;
begin
  fftFrameSize := fftsize;
  osamp := os;
  //sampleRate := rate;
  { set up some handy variables }
  fftFrameSize2 := fftFrameSize div 2;
  stepSize      := fftFrameSize div osamp;
  freqPerBin    := sampleRate / fftFrameSize;
  expct         := 2.0 * Pi * stepSize / fftFrameSize;
  inFifoLatency := fftFrameSize - stepSize;
  //if gInit = 0 then
  begin
    FillChar(gInFIFO, SizeOf(gInFIFO), 0);
    FillChar(gOutFIFO, SizeOf(gOutFIFO), 0);
    FillChar(gFFTworksp, SizeOf(gFFTWorksp), 0);
    FillChar(gLastPhase, SizeOf(gLastPhase), 0);
    FillChar(gSumPhase, SizeOf(gSumPhase), 0);
    FillChar(gOutputAccum, SizeOf(gOutputAccum), 0);
    FillChar(gAnaFreq, SizeOf(gAnaFreq), 0);
    FillChar(gAnaMagn, SizeOf(gAnaMagn), 0);
    gInit := 1;
  end;

  for k := 0 to fftFrameSize -1 do
  begin
    window_table[k] := -0.5 * cos(2.0 * Pi * k/fftFrameSize) + 0.5;
  end;

  gRover := 0;

end;

//------------------------------

// Routine smbPitchShift(). See top of file for explanation
// Purpose: doing pitch shifting while maintaining duration using the Short
// Time Fourier Transform.
// Author: (c)1999-2009 Stephan M. Bernsee <smb@dspdimension.com>

// DESCRIPTION: The routine takes a pitchShift factor value which is       //
// between 0.5 (one octave down) and 2. (one octave up). A value of        //
// exactly 1 does not change the pitch. numSampsToProcess tells the        //
// routine how many samples in indata[0..numSampsToProcess-1] should       //
// be pitch shifted and moved to outdata[0..numSampsToProcess-1].          //
// The two buffers can be identical (ie. it can process the data           //
// in-place). fftFrameSize defines the FFT frame size used for the         //
// processing. Typical values are 1024, 2048 and 4096. It may be           //
// any value <= MAX_FRAME_LENGTH but it MUST be a power of 2. osamp        //
// is the STFT oversampling factor which also determines the overlap       //
// between adjacent STFT frames. It should at least be 4 for moderate      //
// scaling ratios. A value of 32 is recommended for best quality.          //
// sampleRate takes the sample rate for the signal in unit Hz, ie. 44100   //
// for 44.1 kHz audio. The data passed to the routine in indata[] should   //
// be in the range [-1.0, 1.0), which is also the output range for the     //
// data, make sure you scale the data accordingly (for 16bit signed        //
// integers you would have to divide (and multiply) by 32768).             //


procedure SPitchShift.PitchShift(pitchShift: Single; numSampsToProcess{, fftFrameSize, osamp}: Longint; {sampleRate: Single;} indata, outdata: PSingle);
var
  magn, phase, tmp, window, real, imag: Double;
  i,k, qpd, index: Longint;
begin

  (*
  { set up some handy variables }
  fftFrameSize2 := fftFrameSize div 2;
  stepSize      := fftFrameSize div osamp;
  freqPerBin    := sampleRate / fftFrameSize;
  expct         := 2.0 * Pi * stepSize / fftFrameSize;
  inFifoLatency := fftFrameSize - stepSize;
  *)

  if (gRover = 0) then gRover := inFifoLatency;

  (*
  { initialize our static arrays }
  if gInit = 0 then
  begin
    FillChar(gInFIFO, SizeOf(gInFIFO), 0);
    FillChar(gOutFIFO, SizeOf(gOutFIFO), 0);
    FillChar(gFFTworksp, SizeOf(gFFTWorksp), 0);
    FillChar(gLastPhase, SizeOf(gLastPhase), 0);
    FillChar(gSumPhase, SizeOf(gSumPhase), 0);
    FillChar(gOutputAccum, SizeOf(gOutputAccum), 0);
    FillChar(gAnaFreq, SizeOf(gAnaFreq), 0);
    FillChar(gAnaMagn, SizeOf(gAnaMagn), 0);
    gInit := 1;
  end;
  *)

  //----------

  { main processing loop }
  for i := 0 to numSampsToProcess - 1 do
  begin

    { As long as we have not yet collected enough data just read in }
    gInFIFO[gRover] := indata[i];
    outdata[i] := gOutFIFO[gRover - inFifoLatency];
    Inc(gRover);

    { now we have enough data for processing }
    if gRover >= fftFrameSize then
    begin
      gRover := inFifoLatency;

//----------------------------------------------------------------------

      { do windowing and re,im interleave }
      for k := 0 to fftFrameSize -1 do
      begin

        window := window_table[k];//-0.5 * cos(2.0 * Pi * k/fftFrameSize) + 0.5;                  // cos

        gFFTworksp[2 * k] := gInFIFO[k] * window;
        gFFTworksp[2 * k + 1] := 0.0;
        // imag = 0 ?, can we optimize the fft routine, when we know all = 0?
      end;

      { ***************** ANALYSIS ******************* }

      { do transform }
      smbFft(@gFFTworksp[0], {fftFrameSize,} -1);                                 // fft

      { this is the analysis step }
      for k := 0 to fftFrameSize2 - 1 do
      begin
        { de-interlace FFT buffer }
        real := gFFTworksp[2 * k];
        imag := gFFTworksp[2 * k + 1];
        { compute magnitude and phase }
        magn := 2.0 * Sqrt(real * real + imag * imag);                          // sqrt
        phase := ATAN2(imag, real);                                             // atan2

        // magn_buffer
        // phase_buffer

        { compute phase difference }
        tmp := phase - gLastPhase[k];
        gLastPhase[k] := phase;
        { subtract expected phase difference }
        tmp := tmp - k * expct;
        { map delta phase into +/- Pi interval }
        qpd := Trunc(tmp / Pi);
        if qpd >= 0 then Inc(qpd, qpd and 1)
        else Dec(qpd, qpd and 1);
        tmp := tmp - Pi * qpd;
        { get deviation from bin frequency from the +/- Pi interval }
        tmp := osamp * tmp / (2.0 * Pi);
        { compute the k-th partials' true frequency }
        tmp := k * freqPerBin + tmp * freqPerBin;
        { store magnitude and true frequency in analysis arrays }
        gAnaMagn[k] := magn;
        gAnaFreq[k] := tmp;
      end;

      { ***************** PROCESSING ******************* }
      { this does the actual pitch shifting }

      FillChar(gSynMagn, SizeOf(gSynMagn), 0);                                  // memset
      FillChar(gSynFreq, SizeOf(gSynFreq), 0);                                  // memset
      //max = (fftFrameSize2-1) * pitchShift

      for k := 0 to fftFrameSize2 - 1 do
      begin
        index := Trunc(k * pitchShift);
        if index <= fftFrameSize2 then
        begin
          gSynMagn[index] := gSynMagn[index] + gAnaMagn[k];
          gSynFreq[index] := gAnaFreq[k] * pitchShift;
        end
      end;

      { ***************** SYNTHESIS ******************* }
      { this is the synthesis step }

      for k := 0 to fftFrameSize2 - 1 do
      begin
        { get magnitude and true frequency from synthesis arrays }
        magn := gSynMagn[k];
        tmp := gSynFreq[k];
        { subtract bin mid frequency }
        tmp := tmp - k * freqPerBin;
        { get bin deviation from freq deviation }
        tmp := tmp / freqPerBin;
        { take osamp into account }
        tmp := 2.0 * Pi * tmp / osamp;
        { add the overlap phase advance back in }
        tmp := tmp + k * expct;
        { accumulate delta phase to get bin phase }
        gSumPhase[k] := gSumPhase[k] + tmp;
        phase := gSumPhase[k];
        { get real and imag part and re-interleave }
        gFFTworksp[2 * k] := magn * Cos(phase);                                 // cos
        gFFTworksp[2 * k + 1] := magn * Sin(phase);                             // sin
      end;

      { zero negative frequencies }
      for k := fftFrameSize+2 to 2 * fftFrameSize - 1 do gFFTworksp[k] := 0.0;

      { do inverse transform }
      smbFFT(@gFFTworksp[0], {fftFrameSize,} 1);                                  // ifft

      { do windowing and add to output accumulator }
      for k :=0 to fftFrameSize - 1 do
      begin

        window := window_table[k]; //-0.5 * Cos(2.0 * Pi * k/fftFrameSize) + 0.5;                  // cos

        gOutputAccum[k] := gOutputAccum[k] + 2.0 * window * gFFTworksp[2 * k] / (fftFrameSize2 * osamp);
      end;
      for k := 0 to stepSize - 1 do gOutFIFO[k] := gOutputAccum[k];
      { shift accumulator }
      Move( gOutputAccum[stepSize], gOutputAccum[0], fftFrameSize*sizeof(Single) ); // memmove
      { move input FIFO }
      for k := 0 to inFifoLatency - 1 do gInFIFO[k] := gInFIFO[k + stepSize];

      // end synthesis

    end; // gRover >= fftFrameSize

    //----------------------------------------------------------------------

  end; // for i := 0 to numSampsToProcess - 1 do

end;

//----------

{ TODO
  - all 2^n sizes, 2..8192 = 13 variants?
  - unroll loops, duff's device
  - get rid of const values
  - precalc
  - sin/cos table, approximations? precision?
}

procedure SPitchShift.smbFFT(fftBuffer: PSingle; {fftFrameSize,} sign: Longint);

// FFT routine, (C)1996 S.M.Bernsee. Sign = -1 is FFT, 1 is iFFT (inverse)
// Fills fftBuffer[0..2*fftFrameSize-1] with the Fourier transform of the
// time domain data in fftBuffer[0..2*fftFrameSize-1]. The FFT array takes
// and returns the cosine and sine parts in an interleaved manner, ie.
// fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. fftFrameSize
// must be a power of 2. It expects a complex input signal (see footnote 2),
// ie. when working with 'common' audio signals our input signal has to be
// passed as beginin[0],0.,in[1],0.,in[2],0.,...end asf. In that case, the
// transform of the frequencies of interest is in fftBuffer[0...fftFrameSize].

var
  wr, wi, arg, temp: Single;
  p1, p2: PSingle;
  tr, ti, ur, ui: Single;
  p1r, p1i, p2r, p2i: PSingle;
  i, bitm, j, le, le2, k: Longint;
begin
  i := 2;
  while i < 2 * fftFrameSize - 2 do
  begin
    bitm := 2;
    j := 0;
    while bitm < 2 * fftFrameSize do
    begin
      if (i and bitm) <> 0 then Inc(j);
      j := j shl 1;
      bitm := bitm shl 1;
    end;
    if i < j then
    begin
      p1 := fftBuffer + i;
      p2 := fftBuffer + j;
      temp := p1^;
      p1^ := p2^;
      p2^ := temp;
      Inc(p1);
      Inc(p2);
      temp := p1^;
      p1^ := p2^;
      p2^ := temp;
    end;
    Inc(i, 2);
  end;
  le := 2;
  for k := 0 to Trunc(Ln(fftFrameSize)/Ln(2.0) + 0.5) - 1 do
  begin
    le := le shl 1;
    le2 := le shr 1;
    ur := 1.0;
    ui := 0.0;
    arg := Pi / (le2 shr 1);
    wr := Cos(arg);                                                             // cos
    wi := sign * Sin(arg);                                                      // sin
    j := 0;
    while j < le2 do
    begin
      p1r := fftBuffer + j;
      p1i := p1r + 1;
      p2r := p1r + le2;
      p2i := p2r + 1;
      i := j;
      while i < 2 * fftFrameSize do
      begin
        tr := p2r^ * ur - p2i^ * ui;
        ti := p2r^ * ui + p2i^ * ur;
        p2r^ := p1r^ - tr;
        p2i^ := p1i^ - ti;
        p1r^ := p1r^ + tr;
        p1i^ := p1i^ + ti;
        Inc(p1r, le);
        Inc(p1i, le);
        Inc(p2r, le);
        Inc(p2i, le);
        Inc(i, le);
      end;
      tr := ur * wr - ui * wi;
      ui := ur * wi + ui * wr;
      ur := tr;
      Inc(j, 2);
    end
  end
end;

//----------

// 12/12/02, smb
//
// PLEASE NOTE:
//
// There have been some reports on domain errors when the atan2() function
// was used as in the above code. Usually, a domain error should not
// interrupt the program flow (maybe except in Debug mode) but rather be
// handled "silently" and a global variable should be set according to
// this error. However, on some occasions people ran into this kind of
// scenario, so a replacement atan2() function is provided here.
//
// If you are experiencing domain errors and your program stops, simply
// replace all instances of atan2() with calls to the smbAtan2()
// function below.

{

uses
  math;

function SPitchShift.smbAtan2(x, y: Double): Double;
var
  signx: Double;
begin
  if x > 0.0 then signx := 1.0
  else signx := -1.0;
  if x = 0.0 then Result := 0.0
  else if y = 0.0 then Result := signx * Pi / 2.0
  else Result := ArcTan2(x, y);                                            // arctan2
end;

}

//----------------------------------------------------------------------
end.

