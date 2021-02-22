{
  PortAudio bindings for FPC by the HuMuS team.
  Latest version available at: http://sourceforge.net/apps/trac/humus/

  Copyright (c) 2009-2010 HuMuS Project
  Maintained by Roland Schaefer

  PortAudio Portable Real-Time Audio Library
  MME specific extensions
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




unit PA_Win_WMME;



interface

{$MODE FPC}
{$CALLING CDECL}
{$MACRO ON}


uses
  CTypes,
  MMSystem,
  PortAudio,
  PA_Win_Waveformat;

{$DEFINE EXTERNALSPEC := external 'portaudio' name}

{$IFDEF LINKOBJECT}
  {$LINKLIB portaudio.o}
{$ENDIF}


const
  paWinMmeUseLowLevelLatencyParameters             = $01;
  paWinMmeUseMultipleDevices                       = $02;
  paWinMmeUseChannelMask                           = $04;
  paWinMmeDontThrottleOverloadedProcessingThread   = $08;
  paWinMmeWaveFormatDolbyAc3Spdif                  = $10;
  paWinMmeWaveFormatWmaSpdif                       = $20;

type
  PaWinMmeDeviceAndChannelCount = record
    device : PaDeviceIndex;
    channelCount : CInt;
  end;
  PPaWinMmeDeviceAndChannelCount = ^PaWinMmeDeviceAndChannelCount;


type PaWinMmeStreamInfo = record
    size : CULong;
    hostApiType : PaHostApiTypeId;
    version : CULong;
    flags : CULong;
    framesPerBuffer : CULong;
    bufferCount : CULong;
    devices : PPaWinMmeDeviceAndChannelCount;
    deviceCount : CULong;
    channelMask : PaWinWaveFormatChannelMask;
  end;


function PaWinMME_GetStreamInputHandleCount(stream : PPaStream) : CInt;
  EXTERNALSPEC 'PaWinMME_GetStreamInputHandleCount';

function PaWinMME_GetStreamInputHandle(stream : PPaStream;
  handleIndex : CInt) : HWAVEIN;
  EXTERNALSPEC 'PaWinMME_GetStreamInputHandle';

function PaWinMME_GetStreamOutputHandleCount(stream : PPaStream) : CInt;
  EXTERNALSPEC 'PaWinMME_GetStreamOutputHandleCount';

function PaWinMME_GetStreamOutputHandle(stream : PPaStream;
  handleIndex : CInt) : HWAVEOUT;
  EXTERNALSPEC 'PaWinMME_GetStreamOutputHandle';


implementation


end.
