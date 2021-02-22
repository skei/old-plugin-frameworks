{
  PortAudio bindings for FPC by the HuMuS team - Sine test program
  Latest version available at: http://sourceforge.net/apps/trac/humus/

  Copyright (c) 2009-2010 Roland Schaefer

  PortAudio Portable Real-Time Audio Library
  Latest version available at: http://www.portaudio.com

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
  ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  The text above constitutes the entire PortAudio license; however, 
  the PortAudio community also makes the following non-binding requests:

  Any person wishing to distribute modifications to the Software is
  requested to send the modifications to the original developer so that
  they can be incorporated into the canonical version. It is also 
  requested that these non-binding requests be included along with the 
  license above.
}



program TestPASine;


uses
  PortAudio,
  CTypes,
  SysUtils,
  Math;


const
  SampleRate = 44100;
  FramesPerBuffer = 64;

  // How long you want to play the test sine:
  NumSecs = 1;

  // Wavetable size. Influences your pitch:
  TableSize = 200;



type

  { A type which holds a wavetable, two integers keeping track of
    at which offset in the wavetable each channel is currently
    playing (= phase), and a message: }

  PaTestData = record
    Sine : array[0..TableSize] of CFloat;
    LeftPhase : CInt32;
    RightPhase : CInt32;
    AMessage : PChar;
  end;
  PPaTestData = ^PaTestData;



var
  OutputParameters : PaStreamParameters;
  
  { This pointer points to a pointer which will be returned by
    Pa_OpenStream: }
  Stream : PPaStream;
  Err : PaError;
  Data : PaTestData;
  DataPointer : PPaTestData;
  j : CInt32;



{ The callback function which is called by PA everytime new Data is
  needed.
  Remember: ALWAYS USE CDECL or your pointers will be messed up!
  Pointers to this function must be castable to PPaStreamCallback: }

function PaTestCallback( inputBuffer : pointer; OutputBuffer : pointer;
      framesPerBuffer : culong; timeInfo : PPaStreamCallbackTimeInfo;
      statusFlags : PaStreamCallbackFlags; UserData : pointer ) : CInt32;
      cdecl;
var
  OutBuffer : PCFloat;
  i : culong;
  LocalDataPointer : PPaTestData;
begin
  OutBuffer := PCFloat(OutputBuffer);
  LocalDataPointer := PPaTestData(UserData);

  // Fill the buffer...
  for i := 0 to (FramesPerBuffer-1) do begin

    OutBuffer^ := LocalDataPointer^.Sine[LocalDataPointer^.LeftPhase];
    Inc(OutBuffer);

    OutBuffer^ := LocalDataPointer^.Sine[LocalDataPointer^.RightPhase];
    Inc(OutBuffer);

    Inc(LocalDataPointer^.LeftPhase);    
    if (LocalDataPointer^.LeftPhase >= TableSize ) then
        LocalDataPointer^.LeftPhase :=
          (LocalDataPointer^.LeftPhase - TableSize);

    Inc(LocalDataPointer^.RightPhase); 
    Inc(LocalDataPointer^.RightPhase);
    if ( LocalDataPointer^.RightPhase >= TableSize ) then
        LocalDataPointer^.RightPhase :=
          (LocalDataPointer^.RightPhase - TableSize);
  end;

  PaTestCallback := CInt32(0);
end;




{ This is called when playback is finished.
  Remember: ALWAYS USE CDECL or your pointers will be messed up!
  Pointers to this function must be castable to PPaStreamFinishedCallback: }

procedure StreamFinished( UserData : pointer ); cdecl;
var
  LocalDataPointer : PPaTestData;
begin
  LocalDataPointer := PPaTestData(UserData);
  WriteLn('Stream Completed: ', LocalDataPointer^.AMessage);
end;


procedure Error;
begin
  Pa_Terminate();
  WriteLn('An error occured while using the portaudio Stream');
  WriteLn('Error number: ', Err );
  WriteLn('Error message: ', Pa_GetErrorText( Err ) );
  halt;
end;



begin
  WriteLn('PortAudio Test: Output Sine wave. SR = ',
    SampleRate, ', Buffer size = ', FramesPerBuffer);

  DataPointer := @Data;

  // Fill a Sine wavetable (Float Data -1 .. +1)
  for j := 0 to TableSize-1 do begin
    Data.Sine[j] :=
      CFloat((Sin( ( CFloat(j)/CFloat(TableSize) ) * Pi * 2 )));
  end;

  Data.LeftPhase := 0;
  Data.RightPhase := 0;

  Err := Pa_Initialize;
  if not Err = 0 then Error;

  OutputParameters.Device := Pa_GetDefaultOutputDevice;
  OutputParameters.ChannelCount := CInt32(2); 
  OutputParameters.SampleFormat := paFloat32;
  OutputParameters.SuggestedLatency :=
    (Pa_GetDeviceInfo(OutputParameters.device)^.defaultHighOutputLatency)*1;
  OutputParameters.HostApiSpecificStreamInfo := nil;

  WriteLn('Latency ', FloatToStr(
    Pa_GetDeviceInfo(OutputParameters.device)^.defaultHighOutputLatency)
  );

  Err := Pa_OpenStream(@Stream, nil, @OutputParameters, SampleRate,
    FramesPerBuffer, paClipOff, PPaStreamCallback(@PaTestCallback),
    DataPointer);
    
  if not Err = 0 then Error;

  Data.AMessage := 'No Message'#0;

  Err := Pa_SetStreamFinishedCallback(Stream,
    PPaStreamFinishedCallback(@StreamFinished));
  if not Err = 0 then Error;

  WriteLn('Press <RETURN> to attempt start playing a Sine for ',
    IntToStr(NumSecs) ,' sec.');
  ReadLn;

  Err := Pa_StartStream( Stream );
  if not Err = 0 then Error;

  WriteLn('Play for ', NumSecs, ' seconds.');
  Pa_Sleep( NumSecs * 1000 );

  Err := Pa_StopStream( Stream );
  if not Err = 0 then Error;

  Err := Pa_CloseStream( Stream );
  if not Err = 0 then Error;

  Pa_Terminate;
  WriteLn('Test finished.');

end.
