{
  PortAudio bindings for FPC by the HuMuS team.
  Latest version available at: http://sourceforge.net/apps/trac/humus/

  Copyright (c) 2009-2010 HuMuS Project
  Maintained by Roland Schaefer

  PortAudio Portable Real-Time Audio Library
  ALSA-specific extensions
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




unit PA_Linux_ALSA;



interface

{$MODE FPC}
{$CALLING CDECL}
{$MACRO ON}


uses
  CTypes,
  PortAudio;

{$DEFINE EXTERNALSPEC := external 'portaudio' name}

{$IFDEF LINKOBJECT}
  {$LINKLIB portaudio.o}
{$ENDIF}


type
  PaAlsaStreamInfo = record
    size : CULong;
    hostApiType : PaHostApiTypeId;
    version : CULong;
    deviceString : PChar;
  end;
  PPaAlsaStreamInfo = ^PaAlsaStreamInfo;

procedure PaAlsa_InitializeStreamInfo(info : PPaAlsaStreamInfo);
  EXTERNALSPEC 'PaAlsa_InitializeStreamInfo';

procedure PaAlsa_EnableRealtimeScheduling(s : PPaStream; enable : CInt);
  EXTERNALSPEC 'PaAlsa_EnableRealtimeScheduling';

function PaAlsa_GetStreamInputCard(s : PPAStream; card : PCInt) : PaError;
  EXTERNALSPEC 'PaAlsa_GetStreamInputCard';

function PaAlsa_GetStreamOutputCard(s : PPAStream; card : PCInt) : PaError;
  EXTERNALSPEC 'PaAlsa_GetStreamOutputCard';

function PaAlsa_SetNumPeriods(numPeriods : CInt) : PaError;
  EXTERNALSPEC 'PaAlsa_SetNumPeriods';

function PaAlsa_SetRetriesBusy(retries : CInt) : PaError;
  EXTERNALSPEC 'PaAlsa_SetRetriesBusy';


implementation


end.
