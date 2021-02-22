unit goertzel;


{ An object that can calculate various strengths of frequencies in a sample-buffer.  The
implementation is based on the Goertzel-algorithm as specified at:

	 http://www.embedded.com/story/OEG20020819S0057

This class is implemented in a way to allow for async operation between putting samples
into the class and the moment when this data is to be evaluated (the Goertzel applied)

Copyright by Tobias Erichsen (c) 2008
t.erichsen<at>gmx.de

}

interface

uses windows, SyncObjs, classes;

type

  PGoertzelBin = ^TGoertzelBin;
  TGoertzelBin = record
    FFrequency: single;             // center-frequency of this bin
    FWidth: single;                 // width of this bins band
    FCoeff: double;                 // pre-computed Goertzel-coefficient for this bin
    FSize: dword;                   // size of this bin
  end;

  TGoertzel = class(TObject)
    FSamplerate: dword;             // samplerate for this Goertzel instance
    FCritical: TCriticalSection;    // secure AddSamples & Getresults from another
    FSize: dword;                   // size of full sample-buffer
    FValues: psingle;               // pointer to sample-buffer
    FCount: dword;                  // count of samples in buffer
    FBins: TList;                   // Bins added to this Goertzel-instance
    FLargest: dword;                // size of largest bin

    constructor Create(Samplerate: dword);  virtual;
    destructor Destroy;override;
    function AddBin(Frequency, Width: double): integer;
    function GetResult(binindex: dword): double;
    procedure AddSamples(values: psingle; count: dword);
    procedure Flush();
  end;


implementation

uses math, sysutils;


{ TGoertzel }

// Create a Goertzel instance for a specific samplerate
constructor TGoertzel.Create(Samplerate: dword);
//var Fk: double;
begin
  FSamplerate:=Samplerate;                              // store samplerate
  FCritical:=TCriticalSection.Create();                 // create critical section
  FBins:=TList.create();                                // create storage for bin-parameters
  GetMem(FValues,SampleRate*sizeof(single));            // get memory for sample-buffer, min bandwidth 1 Hz  N=(Samplerate/Width)
  FCount:=0;                                            // current amount of samples in buffer
  FSize:=Samplerate;                                    // room in sample-buffer
  FLargest:=0;
end;

// free all resources
destructor TGoertzel.Destroy;
begin
  FreeMem(FValues);
  FBins.free;
  FCritical.free;
  inherited;
end;

// Add a new frequency to be detected
function TGoertzel.AddBin(Frequency, Width: double): integer;
var newbin: PGoertzelBin;
    FSizeFloat: double;
    FOmega: double;
    FSine: double;
    FCosine: double;
    Fk: integer;
begin
  GetMem(newbin,sizeof(TGoertzelBin));                          // get memory for this new bin

  if Frequency>1000 then
    begin
//      OutputDEbugString('tst');
    end;

  if Width<1 then                                               // restrict width to be larger than 1 Hz
    Width:=1
  else if Width>(FSamplerate div 2) then                         // Width may also be not higher than half the sample-rate
    Width:=FSamplerate div 2;


  // calculation of coefficient for this bin
  FSizeFloat:=FSamplerate/Width;
  Fk := trunc(0.5+((FSizeFloat * Frequency) / FSamplerate));
  Fomega := (2.0*PI*Fk)/FSizeFloat;

  Fsine := sin(Fomega);
  Fcosine := cos(Fomega);

  with newbin^ do
    begin
      FFrequency:=Frequency;
      FWidth:=Width;
      FSize:=round(FSizeFloat);
      Fcoeff:=2.0*Fcosine;                  // last step of coefficient calculation
      if FSize>FLargest then
        FLargest:=FSize;
    end;

  result:=FBins.Add(newbin);                // add bin to list & return index

//  OutputDebugString(pchar('goertzel-bin created: '+inttostr(trunc(Frequency))+' ('+inttostr(trunc(Width))+') -> '+inttostr(newbin^.FSize)));
end;


// add an certain amount of samples to the Goertzel-instance
// this implementation is not very efficient & does lots of copies!
procedure TGoertzel.AddSamples(values: psingle; count: dword);
var src,dest: psingle;
    moveback: dword;
begin
  FCritical.Enter();

  if count>=FSize then                                              // buffer is fully replaced, we just use the "newest part" of our samples
    begin
      src:=values;
      Inc(src,Count-FSize);
      CopyMemory(FValues,src,FSize*sizeof(single));
      FCount:=FSize;
    end
  else
    begin
      if (count+FCount)<=FSize then                                 // we add just our newest samples to the buffer
        begin
          dest:=FValues;
          Inc(dest,FCount);
          CopyMemory(dest,values,count*sizeof(single));
          Inc(FCount,count);
        end
      else                                                          // we need to move stuff to front to make space for new data
        begin
          moveback:=count-(FSize-FCount);                           // old samples remaining must be moved by this amount
          src:=FValues;
          Inc(src,moveback);
          MoveMemory(Fvalues,src,(FSize-moveback)*sizeof(single));  // FSize-count old samples moved back to make space
          dest:=FValues;
          Inc(dest,FSize-count);
          CopyMemory(dest,values,count*sizeof(single));             // new data copied behind old data
          FCount:=FSize;
        end;
    end;
  FCritical.Leave();
end;

//this calculates the strength of a certain bin and returns result
function TGoertzel.GetResult(binindex: dword): double;
var Q0,Q1,Q2: double;
    i: integer;
    sample: psingle;
    bin: PGoertzelBin;
    count: dword;
begin
  // sanity check
  if binindex>=FBins.Count then
    begin
      result:=0;
      exit;
    end;
  bin:=FBins.Items[binindex];
  // initialize intermediate variables to zero
  Q1:=0;
  Q2:=0;
  // get pointer to first sample to process
  sample:=FValues;
  if FCount>bin^.FSize then
    begin
      Inc(sample,FCount-bin^.FSize);
      count:=bin^.FSize;
    end
  else
    count:=FCount;
  //make sure that our samples are not moved from another thread while we are processing
  FCritical.Enter();
  // Actual Goertzel calculation over N points
  for i:=0 to bin^.FSize-1 do
    begin
      Q0:=bin^.FCoeff*Q1-Q2+sample^;
      Q2:=Q1;
      Q1:=Q0;
      Inc(sample);
    end;
  FCritical.Leave();
  // "optimized" Goertzel cause we are not interested in the phase information
  // we need to divide by binsize^2 to compensate for the different bin-sizes
  result:=(Q1*Q1+Q2*Q2-Q1*Q2*bin^.FCoeff)/(bin^.FSize*bin^.FSize);
end;

// allow the buffer to be flushed (change of soundcard, samplerate...)
procedure TGoertzel.Flush;
begin
  FCritical.Enter;
  FCount:=0;
  FCritical.Leave;
end;

end.
