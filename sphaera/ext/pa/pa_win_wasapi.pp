{
  PortAudio bindings for FPC by the HuMuS team.
  Latest version available at: http://sourceforge.net/apps/trac/humus/

  Copyright (c) 2009-2010 HuMuS Project
  Maintained by Roland Schaefer

  PortAudio Portable Real-Time Audio Library
  DirectSound specific extensions
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




unit PA_Win_WASAPI;



interface

{$MODE FPC}
{$CALLING CDECL}
{$MACRO ON}


uses
  CTypes,
  PortAudio,
  PA_Win_Waveformat;

{$DEFINE EXTERNALSPEC := external 'portaudio' name}

{$IFDEF LINKOBJECT}
  {$LINKLIB portaudio.o}
{$ENDIF}


type
  PaWasapiFlags = CUInt;

const
  paWinWasapiExclusive                : PaWasapiFlags = (1 shl 0);
  paWinWasapiRedirectHostProcessor    : PaWasapiFlags = (1 shl 1);
  paWinWasapiUseChannelMask           : PaWasapiFlags = (1 shl 2);
  paWinWasapiPolling                  : PaWasapiFlags = (1 shl 3);

{                                                                       // TODO RS ???
  #define paWinWasapiExclusive             (paWinWasapiExclusive)
  #define paWinWasapiRedirectHostProcessor (paWinWasapiRedirectHostProcessor)
  #define paWinWasapiUseChannelMask        (paWinWasapiUseChannelMask)
  #define paWinWasapiPolling               (paWinWasapiPolling)
}

type
  PaWasapiHostProcessorCallback = procedure(inputBuffer : Pointer;
    inputFrames : CLong; outputBuffer : Pointer; outputFrames : CLong;
    userData : Pointer);


type
  PaWasapiStreamInfo = record
    size : CULong;
    hostApiType : PaHostApiTypeId;
    version : CULong;
    flags : CULong;
    channelMask : PaWinWaveFormatChannelMask;
    hostProcessorOutput : PaWasapiHostProcessorCallback;
    hostProcessorInput : PaWasapiHostProcessorCallback;
  end;


function PaWasapi_GetDeviceDefaultFormat(pFormat : Pointer;
  nFormatSize : CInt; nDevice : PaDeviceIndex) : CInt;
  EXTERNALSPEC 'PaWasapi_GetDeviceDefaultFormat';


type
  PaWasapiDeviceRole = (
    eRoleRemoteNetworkDevice = 0,
    eRoleSpeakers,
    eRoleLineLevel,
    eRoleHeadphones,
    eRoleMicrophone,
    eRoleHeadset,
    eRoleHandset,
    eRoleUnknownDigitalPassthrough,
    eRoleSPDIF,
    eRoleHDMI,
    eRoleUnknownFormFactor
  );


function PaWasapi_GetDeviceRole(nDevice : PaDeviceIndex) : CInt { PaWasapiDeviceRole };
  EXTERNALSPEC 'PaWasapi_GetDeviceRole';


function PaWasapi_ThreadPriorityBoost(hTask : PPointer;
  nPriorityClass : CInt) : PaError;
  EXTERNALSPEC 'PaWasapi_ThreadPriorityBoost';


function PaWasapi_ThreadPriorityRevert(hTask : Pointer) : PaError;
  EXTERNALSPEC 'PaWasapi_ThreadPriorityRevert';


implementation





end.
