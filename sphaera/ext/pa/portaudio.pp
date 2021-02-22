{
  PortAudio bindings for FPC by the HuMuS team.
  Latest version available at: http://sourceforge.net/apps/trac/humus/

  Copyright (c) 2009-2010 HuMuS Project
  Maintained by Roland Schaefer

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


unit PortAudio;



interface

{$MODE FPC}
{$CALLING CDECL}
{$MACRO ON}


uses
  CTypes;

{$DEFINE EXTERNALSPEC := external 'portaudio' name}

{$IFDEF LINKOBJECT}
  {$LINKLIB portaudio.o}
{$ENDIF}


type
  PaError = CInt32;
  PaErrorCode =(
    paNotInitialized := -10000,
    paUnanticipatedHostError,
    paInvalidChannelCount,
    paInvalidSampleRate,
    paInvalidDevice,
    paInvalidFlag,
    paSampleFormatNotSupported,
    paBadIODeviceCombination,
    paInsufficientMemory,
    paBufferTooBig,
    paBufferTooSmall,
    paNullCallback,
    paBadStreamPtr,
    paTimedOut,
    paInternalError,
    paDeviceUnavailable,
    paIncompatibleHostApiSpecificStreamInfo,
    paStreamIsStopped,
    paStreamIsNotStopped,
    paInputOverflowed,
    paOutputUnderflowed,
    paHostApiNotFound,
    paInvalidHostApi,
    paCanNotReadFromACallbackStream,
    paCanNotWriteToACallbackStream,
    paCanNotReadFromAnOutputOnlyStream,
    paCanNotWriteToAnInputOnlyStream,
    paIncompatibleStreamHostApi,
    paBadBufferPtr,
    paNoError := 0
  );


  PaDeviceIndex = CInt32;
  
  PaHostApiIndex = CInt32;
  
  
  PaHostApiTypeId =(paInDevelopment := 0,
    paDirectSound := 1,    
    paMME := 2,
    paASIO := 3,
    paSoundManager := 4,
    paCoreAudio := 5,
    paOSS := 7,
    paALSA := 8,
    paAL := 9,
    paBeOS := 10,
    paWDMKS := 11,
    paJACK := 12,
    paWASAPI := 13,
    paAudioScienceHPI := 14
  );
  
  
  PaHostApiInfo = record
    structVersion : CInt32;
    _type : PaHostApiTypeId ;
    _name : PChar;
    deviceCount : CInt32;
    defaultInputDevice : PaDeviceIndex;
    defaultOutputDevice : PaDeviceIndex;
  end;
  PPaHostApiInfo = ^PaHostApiInfo;
  
  
  PaHostErrorInfo = record
    hostApiType : PaHostApiTypeId;
    errorCode : CLong;
    errorText : PChar;
  end;
  PPaHostErrorInfo = ^PaHostErrorInfo;
  
  
  PaTime = CDouble;
  
  PaSampleFormat = pCULongLong;
  
  
  PaDeviceInfo = record
    structVersion : CInt32;
    _name : PChar;
    hostApi : PaHostApiIndex;
    maxInputChannels : CInt32;
    maxOutputChannels : CInt32;
    defaultLowInputLatency : PaTime;
    defaultLowOutputLatency : PaTime;
    defaultHighInputLatency : PaTime;
    defaultHighOutputLatency : PaTime;
    defaultSampleRate : CDouble;
  end;
  PPaDeviceInfo = ^PaDeviceInfo;


  PaStreamParameters = record
    device : PaDeviceIndex;
    channelCount : CInt32;
    sampleFormat : PaSampleFormat;
    suggestedLatency : PaTime;
    hostApiSpecificStreamInfo : Pointer;
  end;
  PPaStreamParameters = ^PaStreamParameters;


  // ************************* Streaming types *************************

  PaStream = Pointer;
  PPaStream = ^PaStream;
  PPPaStream = ^PPaStream;

  PaStreamFlags = CULong;

  PaStreamCallbackTimeInfo = record
    inputBufferAdcTime : PaTime;
    currentTime : PaTime;
    outputBufferDacTime : PaTime;
  end;
  PPaStreamCallbackTimeInfo = ^PaStreamCallbackTimeInfo;
  

  PaStreamCallbackFlags = CULong;


  PaStreamCallbackResult =(
    paContinue := 0,
    paComplete := 1,
    paAbort := 2);
  
  
  PaStreamCallback = function(
    input : Pointer;
    output : Pointer;
    frameCount : CULong;
    timeInfo : PPaStreamCallbackTimeInfo;
    statusFlags : PaStreamCallbackFlags;
    userData : Pointer) : CInt32;
  PPaStreamCallback = ^PaStreamCallback;  


  PaStreamFinishedCallback = procedure(userData : Pointer);
  PPaStreamFinishedCallback = ^PaStreamFinishedCallback;


  PaStreamInfo = record
    structVersion : CInt32;
    inputLatency : PaTime;
    outputLatency : PaTime;
    sampleRate : CDouble;
  end;
  PPaStreamInfo = ^PaStreamInfo;



const
  paFormatIsSupported = 0;
  paFramesPerBufferUnspecified = 0;
  paNoDevice = PaDeviceIndex(-1);
  paUseHostApiSpecificDeviceSpecification = PaDeviceIndex(-2);
  paFloat32 = PaSampleFormat($00000001);
  paInt32 = PaSampleFormat($00000002);
  paInt24 = PaSampleFormat($00000004);
  paInt16 = PaSampleFormat($00000008);
  paInt8 = PaSampleFormat($00000010);
  paUInt8 = PaSampleFormat($00000020);
  paCustomFormat = PaSampleFormat($00010000);
  paNonInterleaved = PaSampleFormat($80000000);
  paNoFlag = PaStreamFlags(0);
  paClipOff = PaStreamFlags($00000001);
  paDitherOff = PaStreamFlags($00000002);
  paNeverDropInput = PaStreamFlags($00000004);
  paPrimeOutputBuffersUsingStreamCallback = PaStreamFlags($00000008);
  paPlatformSpecificFlags = PaStreamFlags($FFFF0000);
  paInputUnderflow = PaStreamCallbackFlags($00000001);
  paInputOverflow = PaStreamCallbackFlags($00000002);
  paOutputUnderflow = PaStreamCallbackFlags($00000004);
  paOutputOverflow = PaStreamCallbackFlags($00000008);
  paPrimingOutput = PaStreamCallbackFlags($00000010);


// *************************** functions *******************************


function Pa_GetVersion : CInt32;
  EXTERNALSPEC 'Pa_GetVersion';

function Pa_GetVersionText : PChar;
  EXTERNALSPEC 'Pa_GetVersionText';

function Pa_GetErrorText(errorCode : PaError) : PChar;
  EXTERNALSPEC 'Pa_GetErrorText';

function Pa_Initialize : PaError;
  EXTERNALSPEC 'Pa_Initialize';

function Pa_Terminate : PaError;
  EXTERNALSPEC 'Pa_Terminate';

function Pa_GetHostApiCount : PaHostApiIndex;
  EXTERNALSPEC 'Pa_GetHostApiCount';

function Pa_GetDefaultHostApi : PaHostApiIndex;
  EXTERNALSPEC 'Pa_GetDefaultHostApi';

function Pa_GetHostApiInfo(hostApi : PaHostApiIndex) : PPaHostApiInfo;
  EXTERNALSPEC 'Pa_GetHostApiInfo';

function Pa_HostApiTypeIdToHostApiIndex(_type : PaHostApiTypeId) :
  PaHostApiIndex;
  EXTERNALSPEC 'Pa_HostApiTypeIdToHostApiIndex';

function Pa_HostApiDeviceIndexToDeviceIndex(hostApi : PaHostApiIndex;
  hostApiDeviceIndex : CInt32) : PaDeviceIndex;
  EXTERNALSPEC 'Pa_HostApiDeviceIndexToDeviceIndex';

function Pa_GetLastHostErrorInfo : PPaHostErrorInfo;
  EXTERNALSPEC 'Pa_GetLastHostErrorInfo';


// ************** Device enumeration and capabilities ******************


function Pa_GetDeviceCount : PaDeviceIndex;
  EXTERNALSPEC 'Pa_GetDeviceCount';

function Pa_GetDefaultInputDevice : PaDeviceIndex;
  EXTERNALSPEC 'Pa_GetDefaultInputDevice';

function Pa_GetDefaultOutputDevice : PaDeviceIndex;
  EXTERNALSPEC 'Pa_GetDefaultOutputDevice';

function Pa_GetDeviceInfo(device : PaDeviceIndex) : PPaDeviceInfo;
  EXTERNALSPEC 'Pa_GetDeviceInfo';

function Pa_IsFormatSupported(inputParameters,
  outputParameters : PaStreamParameters; sampleRate : CDouble) :
  PaError;
  EXTERNALSPEC 'Pa_IsFormatSupported';

// *********************** Stream function *****************************

function Pa_OpenStream(stream : PPPaStream;
  inputParameters : PPaStreamParameters;
  outputParameters : PPaStreamParameters;
  sampleRate : CDouble;
  framesPerBuffer : CULong;
  streamFlags : PaStreamFlags;
  streamCallback : PPaStreamCallback;
  userData : Pointer) : PaError;
  EXTERNALSPEC 'Pa_OpenStream';

function Pa_OpenDefaultStream(stream : PPPaStream;
  numInputChannels : CInt32;
  numOutputChannels : CInt32;
  sampleFormat : PaSampleFormat;
  sampleRate : CDouble;
  framesPerBuffer : CULong;
  streamCallback : PPaStreamCallback;
  userData : Pointer) : PaError;
  EXTERNALSPEC 'Pa_OpenDefaultStream';

function Pa_CloseStream(stream : PPaStream) : PaError;
  EXTERNALSPEC 'Pa_CloseStream';

function Pa_SetStreamFinishedCallback(stream : PPaStream;
  streamFinishedCallback : PPaStreamFinishedCallback) : PaError;
  EXTERNALSPEC 'Pa_SetStreamFinishedCallback';

function Pa_StartStream(stream : PPaStream) : PaError;
  EXTERNALSPEC 'Pa_StartStream'; 

function Pa_StopStream(stream : PPaStream) : PaError;
  EXTERNALSPEC 'Pa_StopStream'; 

function Pa_AbortStream(stream : PaStream) : PaError;
  EXTERNALSPEC 'Pa_AbortStream'; 

function Pa_IsStreamStopped(stream : PPaStream) : PaError;
  EXTERNALSPEC 'Pa_IsStreamStopped'; 

function Pa_IsStreamActive(stream : PPaStream) : PaError;
  EXTERNALSPEC 'Pa_IsStreamActive'; 

function Pa_GetStreamInfo(stream : PPaStream) : PPaStreamInfo;
  EXTERNALSPEC 'Pa_GetStreamInfo'; 

function Pa_GetStreamTime(stream : PPaStream) : PaTime;
  EXTERNALSPEC 'Pa_GetStreamTime'; 

function Pa_GetStreamCpuLoad(stream : PPaStream) : CDouble;
  EXTERNALSPEC 'Pa_GetStreamCpuLoad'; 

function Pa_ReadStream(stream : PPaStream; buffer : Pointer;
  frames : CULong) : PaError;
  EXTERNALSPEC 'Pa_ReadStream'; 

function Pa_WriteStream(stream : PPaStream; buffer : Pointer;
  frames : CULong) : PaError;
  EXTERNALSPEC 'Pa_WriteStream'; 

function Pa_GetStreamReadAvailable(stream : PPaStream) : CSLong;
  EXTERNALSPEC 'Pa_GetStreamReadAvailable'; 

function Pa_GetStreamWriteAvailable(stream : PPaStream) : CSLong;
  EXTERNALSPEC 'Pa_GetStreamWriteAvailable'; 


// ****************** Miscellaneous utilities **************************


function Pa_GetSampleSize(format : PaSampleFormat) : PaError;
  EXTERNALSPEC 'Pa_GetSampleSize'; 

procedure Pa_Sleep(msec : CLong);
  EXTERNALSPEC 'Pa_Sleep';


implementation

end.

