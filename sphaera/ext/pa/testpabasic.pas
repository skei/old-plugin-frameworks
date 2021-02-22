{
  PortAudio bindings for FPC by the HuMuS team - Basic test program
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

program TestPABasic;


uses
  PortAudio,
  SysUtils,
  CTypes;

var
  i : integer;
  HaCount: CInt32;
  HaInfo: PPaHostApiInfo;

begin
  // the only information we can get without initing first:
  WriteLn('PA version int: ', IntToStr(Pa_GetVersion));
  WriteLn('PA version text: ', Pa_GetVersionText);
  
  // init PA
  WriteLn('Doing PA init: ', Pa_GetErrorText(CInt32(Pa_Initialize)));

  // print some system information provided by PA
  HaCount := Pa_GetHostApiCount;
  WriteLn('Host API count (OK if positive): ', IntToStr(HaCount));
  WriteLn('Default host API: ', IntToStr(Pa_GetDefaultHostApi));

  for i := 0 to (HaCount-1) do begin
    HaInfo := Pa_GetHostApiInfo ( i );
    WriteLn('Found host API ', i, ' which is ', HaInfo^._name, 
        ' (devices: ', IntToStr(HaInfo^.deviceCount), ')');
  end;

  WriteLn('Default output device: ', IntToStr(Pa_GetDefaultOutputDevice));

  // end all PA activity
  WriteLn('Doing PA termination: ',
    Pa_GetErrorText(CInt32(Pa_Terminate)));
end.