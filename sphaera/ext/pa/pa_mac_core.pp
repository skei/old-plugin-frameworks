{
  PortAudio bindings for FPC by the HuMuS team.
  Latest version available at: http://sourceforge.net/apps/trac/humus/

  Copyright (c) 2009-2010 HuMuS Project
  Maintained by Roland Schaefer

  PortAudio Portable Real-Time Audio Library
  Macintosh Core Audio specific extensions
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


{ This code is an untested guess. We (HuMuS) don't support MacOS at all.
  Feel free to contribute. }


unit PA_Mac_Core;



interface

{$MODE FPC}
{$CALLING CDECL}
{$MACRO ON}


uses
  CTypes,
  //AudioUnit,   // Is this available in FPC? It provides AudioDeviceID.
  PortAudio;

{$DEFINE EXTERNALSPEC := external 'portaudio' name}

{$IFDEF LINKOBJECT}
  {$LINKLIB portaudio.o}
{$ENDIF}


type
  SInt32 = LongInt;
  PCSInt32 = ^SInt32;

  PaMacCoreStreamInfo = record
    size : CULong;
    hostApiType : PaHostApiTypeId;
    version : CULong;
    flags : CULong;
    channelMap : PCSInt32;
    channelMapSize : CULong;
  end;
  PPaMacCoreStreamInfo = ^PaMacCoreStreamInfo;


procedure PaMacCore_SetupStreamInfo(data : PPaMacCoreStreamInfo;
  flags : CULong);
  EXTERNALSPEC 'PaMacCore_SetupStreamInfo';

procedure PaMacCore_SetupChannelMap(data : PPaMacCoreStreamInfo;
  channelMap : PCSInt32; channelMapSize : CULong);
  EXTERNALSPEC 'PaMacCore_SetupChannelMap';

function PaMacCore_GetStreamInputDevice(s : PPaStream) : AudioDeviceID;
  EXTERNALSPEC 'PaMacCore_GetStreamInputDevice';
  
function PaMacCore_GetStreamOutputDevice(s : PPaStream) : AudioDeviceID;
  EXTERNALSPEC 'PaMacCore_GetStreamOutputDevice';

function PaMacCore_GetChannelName(device : CInt; channelIndex : CInt;
  input : Boolean) : PChar;
  EXTERNALSPEC 'PaMacCore_GetChannelName';

const
  paMacCoreChangeDeviceParameters    = $01;
  paMacCoreFailIfConversionRequired  = $02;
  paMacCoreConversionQualityMin      = $0100;
  paMacCoreConversionQualityMedium   = $0200;
  paMacCoreConversionQualityLow      = $0300;
  paMacCoreConversionQualityHigh     = $0400;
  paMacCoreConversionQualityMax      = $0000;
  paMacCorePlayNice                  = $00;
  paMacCorePro                       = $01;
  paMacCoreMinimizeCPUButPlayNice    = $0100;
  paMacCoreMinimizeCPU               = $0101;



implementation





end.
