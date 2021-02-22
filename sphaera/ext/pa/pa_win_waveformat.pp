{
  PortAudio bindings for FPC by the HuMuS team.
  Latest version available at: http://sourceforge.net/apps/trac/humus/

  Copyright (c) 2009-2010 HuMuS Project
  Maintained by Roland Schaefer

  PortAudio Portable Real-Time Audio Library
  Windows WAVEFORMAT* data structure utilities
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


unit PA_Win_Waveformat;


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
  PaWinWaveFormatChannelMask = CULong;

const
  PAWIN_SPEAKER_FRONT_LEFT             : PaWinWaveFormatChannelMask = $1;
  PAWIN_SPEAKER_FRONT_RIGHT            : PaWinWaveFormatChannelMask = $2;
  PAWIN_SPEAKER_FRONT_CENTER           : PaWinWaveFormatChannelMask = $4;
  PAWIN_SPEAKER_LOW_FREQUENCY          : PaWinWaveFormatChannelMask = $8;
  PAWIN_SPEAKER_BACK_LEFT              : PaWinWaveFormatChannelMask = $10;
  PAWIN_SPEAKER_BACK_RIGHT             : PaWinWaveFormatChannelMask = $20;
  PAWIN_SPEAKER_FRONT_LEFT_OF_CENTER   : PaWinWaveFormatChannelMask = $40;
  PAWIN_SPEAKER_FRONT_RIGHT_OF_CENTER  : PaWinWaveFormatChannelMask = $80;
  PAWIN_SPEAKER_BACK_CENTER            : PaWinWaveFormatChannelMask = $100;
  PAWIN_SPEAKER_SIDE_LEFT              : PaWinWaveFormatChannelMask = $200;
  PAWIN_SPEAKER_SIDE_RIGHT             : PaWinWaveFormatChannelMask = $400;
  PAWIN_SPEAKER_TOP_CENTER             : PaWinWaveFormatChannelMask = $800;
  PAWIN_SPEAKER_TOP_FRONT_LEFT         : PaWinWaveFormatChannelMask = $1000;
  PAWIN_SPEAKER_TOP_FRONT_CENTER       : PaWinWaveFormatChannelMask = $2000;
  PAWIN_SPEAKER_TOP_FRONT_RIGHT        : PaWinWaveFormatChannelMask = $4000;
  PAWIN_SPEAKER_TOP_BACK_LEFT          : PaWinWaveFormatChannelMask = $8000;
  PAWIN_SPEAKER_TOP_BACK_CENTER        : PaWinWaveFormatChannelMask = $10000;
  PAWIN_SPEAKER_TOP_BACK_RIGHT         : PaWinWaveFormatChannelMask = $20000;
  PAWIN_SPEAKER_RESERVED               : PaWinWaveFormatChannelMask = $7FFC0000;
  PAWIN_SPEAKER_ALL                    : PaWinWaveFormatChannelMask = $80000000;

  PAWIN_SPEAKER_DIRECTOUT              : PaWinWaveFormatChannelMask = $0;


function PAWIN_SPEAKER_MONO                 : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_STEREO               : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_QUAD                 : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_SURROUND             : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_5POINT1              : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_7POINT1              : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_5POINT1_SURROUND     : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_7POINT1_SURROUND     : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_5POINT1_BACK         : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_7POINT1_WIDE         : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_GROUND_FRONT_LEFT    : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_GROUND_FRONT_CENTER  : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_GROUND_FRONT_RIGHT   : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_GROUND_REAR_LEFT     : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_GROUND_REAR_RIGHT    : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_TOP_MIDDLE           : PaWinWaveFormatChannelMask; inline;
function PAWIN_SPEAKER_SUPER_WOOFER         : PaWinWaveFormatChannelMask; inline;


const
  PAWIN_SIZEOF_WAVEFORMATEX          = 18;
  PAWIN_SIZEOF_WAVEFORMATEXTENSIBLE  = 40; // PAWIN_SIZEOF_WAVEFORMATEX + 22

type
  PaWinWaveFormatFields = array[0..39] of CUChar; // PAWIN_SIZEOF_WAVEFORMATEXTENSIBLE - 1

  PaWinWaveFormat = record
    fields : PaWinWaveFormatFields;
    extraLongForAlignment : CULong;
  end;
  PPaWinWaveFormat = ^PaWinWaveFormat;


function PAWIN_INDEXOF_WVALIDBITSPERSAMPLE : CULong; inline;

function PAWIN_INDEXOF_DWCHANNELMASK : CULong; inline;

function PAWIN_INDEXOF_SUBFORMAT : CULong; inline;

const
  PAWIN_WAVE_FORMAT_PCM             = 1;
  PAWIN_WAVE_FORMAT_IEEE_FLOAT      = 3;
  PAWIN_WAVE_FORMAT_DOLBY_AC3_SPDIF = $92;
  PAWIN_WAVE_FORMAT_WMA_SPDIF       = $164;



function PaWin_SampleFormatToLinearWaveFormatTag(sampleFormat : PaSampleFormat) : CInt;
  EXTERNALSPEC 'PaWin_SampleFormatToLinearWaveFormatTag';

procedure PaWin_InitializeWaveFormatEx(waveFormat : PPaWinWaveFormat; 
  numChannels : CInt; sampleFormat : PaSampleFormat; waveFormatTag : CInt;
  sampleRate : CDouble);
  EXTERNALSPEC 'PaWin_InitializeWaveFormatEx';

procedure PaWin_InitializeWaveFormatExtensible(waveFormat : PPaWinWaveFormat; 
  numChannels : CInt; sampleFormat : PaSampleFormat; waveFormatTag : CInt;
  sampleRate : CDouble; channelMask : PaWinWaveFormatChannelMask);
  EXTERNALSPEC 'PaWin_InitializeWaveFormatExtensible';

function PaWin_DefaultChannelMask(numChannels : CInt) : PaWinWaveFormatChannelMask;
  EXTERNALSPEC 'PaWin_DefaultChannelMask';



implementation




function PAWIN_SPEAKER_MONO : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_MONO := PAWIN_SPEAKER_FRONT_CENTER;
end;


function PAWIN_SPEAKER_STEREO : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_STEREO := PAWIN_SPEAKER_FRONT_LEFT or PAWIN_SPEAKER_FRONT_RIGHT;
end;


function PAWIN_SPEAKER_QUAD : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_QUAD := PAWIN_SPEAKER_FRONT_LEFT or PAWIN_SPEAKER_FRONT_RIGHT or 
    PAWIN_SPEAKER_BACK_LEFT  or PAWIN_SPEAKER_BACK_RIGHT;
end;


function PAWIN_SPEAKER_SURROUND : PaWinWaveFormatChannelMask; inline;
begin
   PAWIN_SPEAKER_SURROUND := PAWIN_SPEAKER_FRONT_LEFT or PAWIN_SPEAKER_FRONT_RIGHT or 
     PAWIN_SPEAKER_FRONT_CENTER or PAWIN_SPEAKER_BACK_CENTER;
end;


function PAWIN_SPEAKER_5POINT1 : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_5POINT1 := PAWIN_SPEAKER_FRONT_LEFT or PAWIN_SPEAKER_FRONT_RIGHT or 
    PAWIN_SPEAKER_FRONT_CENTER or PAWIN_SPEAKER_LOW_FREQUENCY or 
    PAWIN_SPEAKER_BACK_LEFT  or PAWIN_SPEAKER_BACK_RIGHT;
end;


function PAWIN_SPEAKER_7POINT1 : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_7POINT1 := PAWIN_SPEAKER_FRONT_LEFT or PAWIN_SPEAKER_FRONT_RIGHT or 
    PAWIN_SPEAKER_FRONT_CENTER or PAWIN_SPEAKER_LOW_FREQUENCY or 
    PAWIN_SPEAKER_BACK_LEFT or PAWIN_SPEAKER_BACK_RIGHT or 
    PAWIN_SPEAKER_FRONT_LEFT_OF_CENTER or PAWIN_SPEAKER_FRONT_RIGHT_OF_CENTER;
end;


function PAWIN_SPEAKER_5POINT1_SURROUND : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_5POINT1_SURROUND := PAWIN_SPEAKER_FRONT_LEFT or PAWIN_SPEAKER_FRONT_RIGHT or 
    PAWIN_SPEAKER_FRONT_CENTER or PAWIN_SPEAKER_LOW_FREQUENCY or 
    PAWIN_SPEAKER_SIDE_LEFT  or PAWIN_SPEAKER_SIDE_RIGHT;
end;


function PAWIN_SPEAKER_7POINT1_SURROUND : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_7POINT1_SURROUND := PAWIN_SPEAKER_FRONT_LEFT or PAWIN_SPEAKER_FRONT_RIGHT or
    PAWIN_SPEAKER_FRONT_CENTER or PAWIN_SPEAKER_LOW_FREQUENCY or 
    PAWIN_SPEAKER_BACK_LEFT or PAWIN_SPEAKER_BACK_RIGHT or 
    PAWIN_SPEAKER_SIDE_LEFT or PAWIN_SPEAKER_SIDE_RIGHT;
end;


function PAWIN_SPEAKER_5POINT1_BACK : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_5POINT1_BACK := PAWIN_SPEAKER_5POINT1;
end;


function PAWIN_SPEAKER_7POINT1_WIDE : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_7POINT1_WIDE := PAWIN_SPEAKER_7POINT1;
end;


function PAWIN_SPEAKER_GROUND_FRONT_LEFT : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_GROUND_FRONT_LEFT := PAWIN_SPEAKER_FRONT_LEFT;
end;


function PAWIN_SPEAKER_GROUND_FRONT_CENTER : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_GROUND_FRONT_CENTER := PAWIN_SPEAKER_FRONT_CENTER;
end;


function PAWIN_SPEAKER_GROUND_FRONT_RIGHT : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_GROUND_FRONT_RIGHT := PAWIN_SPEAKER_FRONT_RIGHT;
end;


function PAWIN_SPEAKER_GROUND_REAR_LEFT : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_GROUND_REAR_LEFT := PAWIN_SPEAKER_BACK_LEFT;
end;


function PAWIN_SPEAKER_GROUND_REAR_RIGHT : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_GROUND_REAR_RIGHT := PAWIN_SPEAKER_BACK_RIGHT;
end;


function PAWIN_SPEAKER_TOP_MIDDLE : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_TOP_MIDDLE := PAWIN_SPEAKER_TOP_CENTER;
end;


function PAWIN_SPEAKER_SUPER_WOOFER : PaWinWaveFormatChannelMask; inline;
begin
  PAWIN_SPEAKER_SUPER_WOOFER := PAWIN_SPEAKER_LOW_FREQUENCY;
end;


function PAWIN_INDEXOF_WVALIDBITSPERSAMPLE : CULong; inline;
begin
  PAWIN_INDEXOF_WVALIDBITSPERSAMPLE := PAWIN_SIZEOF_WAVEFORMATEX+0;
end;


function PAWIN_INDEXOF_DWCHANNELMASK : CULong; inline;
begin
  PAWIN_INDEXOF_DWCHANNELMASK := PAWIN_SIZEOF_WAVEFORMATEX+2;
end;


function PAWIN_INDEXOF_SUBFORMAT : CULong; inline;
begin
  PAWIN_INDEXOF_SUBFORMAT := PAWIN_SIZEOF_WAVEFORMATEX+6;
end;




end.
