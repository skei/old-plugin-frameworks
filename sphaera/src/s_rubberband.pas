{$incude sphaera.inc}
unit s_rubberband;

interface

implementation

end.

(*
{

  This file is part of HuMuS.

  WiSeSLAp is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  WiSeSLAp is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with HuMuS.  If not, see <http:  www.gnu.org/licenses/>.


  This is a translation ('binding') of a C header originally included
  in another package. The original copyright follows:


  Rubber Band
  An audio time-stretching and pitch-shifting library.
  Copyright 2007-2008 Chris Cannam.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.  See the file
  COPYING included with this distribution for more information.

}

unit RubberBand;


{$MODE FPC}
{$CALLING CDECL}
{$MACRO ON}

interface


uses
  CTypes;



{$DEFINE EXTERNALSPEC := external 'rubberband' name}

{$IFDEF LINKOBJECT}
  {$LINKLIB rubberband.o}
{$ENDIF}



const
  RUBBERBAND_VERSION = '1.3.0-gpl';
  RUBBERBAND_API_MAJOR_VERSION = 2;
  RUBBERBAND_API_MINOR_VERSION = 0;


type
  RubberBandOption = CInt;
  RubberBandOptions = CInt;

const
  RubberBandOptionProcessOffline       : RubberBandOption = $00000000;
  RubberBandOptionProcessRealTime      : RubberBandOption = $00000001;
  RubberBandOptionStretchElastic       : RubberBandOption = $00000000;
  RubberBandOptionStretchPrecise       : RubberBandOption = $00000010;
  RubberBandOptionTransientsCrisp      : RubberBandOption = $00000000;
  RubberBandOptionTransientsMixed      : RubberBandOption = $00000100;
  RubberBandOptionTransientsSmooth     : RubberBandOption = $00000200;
  RubberBandOptionPhaseLaminar         : RubberBandOption = $00000000;
  RubberBandOptionPhaseIndependent     : RubberBandOption = $00002000;
  RubberBandOptionThreadingAuto        : RubberBandOption = $00000000;
  RubberBandOptionThreadingNever       : RubberBandOption = $00010000;
  RubberBandOptionThreadingAlways      : RubberBandOption = $00020000;
  RubberBandOptionWindowStandard       : RubberBandOption = $00000000;
  RubberBandOptionWindowShort          : RubberBandOption = $00100000;
  RubberBandOptionWindowLong           : RubberBandOption = $00200000;
  RubberBandOptionFormantShifted       : RubberBandOption = $00000000;
  RubberBandOptionFormantPreserved     : RubberBandOption = $01000000;
  RubberBandOptionPitchHighQuality     : RubberBandOption = $00000000;
  RubberBandOptionPitchHighSpeed       : RubberBandOption = $02000000;
  RubberBandOptionPitchHighConsistency : RubberBandOption = $04000000;



type
  RubberBandState = type Pointer;


function rubberband_new(sampleRate : CUInt;
  channels : CUInt; options : RubberBandOptions;
  initialTimeRatio : CDouble; initialPitchScale : CDouble) : RubberBandState;
  EXTERNALSPEC 'rubberband_new';
procedure rubberband_delete(AState : RubberBandState);
  EXTERNALSPEC 'rubberband_delete';
procedure rubberband_reset(AState : RubberBandState);
  EXTERNALSPEC 'rubberband_reset';
procedure rubberband_set_time_ratio(AState : RubberBandState; ratio : CDouble);
  EXTERNALSPEC 'rubberband_set_time_ratio';
procedure rubberband_set_pitch_scale(AState : RubberBandState; scale : CDouble);
  EXTERNALSPEC 'rubberband_set_pitch_scale';
function rubberband_get_time_ratio(AState : RubberBandState) : CDouble;
  EXTERNALSPEC 'rubberband_get_time_ratio';
function rubberband_get_pitch_scale(AState : RubberBandState) : CDouble;
  EXTERNALSPEC 'rubberband_get_pitch_scale';
function rubberband_get_latency(AState : RubberBandState) : CUInt;
  EXTERNALSPEC 'rubberband_get_latency';
procedure rubberband_set_transients_option(AState : RubberBandState;
  options : RubberBandOptions); EXTERNALSPEC 'rubberband_set_transients_option';
procedure rubberband_set_phase_option(AState : RubberBandState;
  options : RubberBandOptions); EXTERNALSPEC 'rubberband_set_phase_option';
procedure rubberband_set_formant_option(AState : RubberBandState;
  options : RubberBandOptions); EXTERNALSPEC 'rubberband_set_formant_option';
procedure rubberband_set_pitch_option(AState : RubberBandState;
  options : RubberBandOptions); EXTERNALSPEC 'rubberband_set_pitch_option';
procedure rubberband_set_expected_input_duration(AState : RubberBandState;
  samples : CUInt); EXTERNALSPEC 'rubberband_set_expected_input_duration';
function rubberband_get_samples_required(AState : RubberBandState) :
  CUInt; EXTERNALSPEC 'rubberband_get_samples_required';
procedure rubberband_set_max_process_size(AState : RubberBandState;
  samples : CUInt); EXTERNALSPEC 'rubberband_set_max_process_size';
procedure rubberband_study(AState : RubberBandState; input : PCFloat;
  samples: CUInt; final_ : CInt); EXTERNALSPEC 'rubberband_study';
procedure rubberband_process(AState : RubberBandState; input : PCFloat;
  samples: CUInt; final_ : CInt); EXTERNALSPEC 'rubberband_process';
function rubberband_available(AState : RubberBandState) : CInt;
  EXTERNALSPEC 'rubberband_available';
function rubberband_retrieve(AState : RubberBandState; output : PCFloat;
  samples : CUInt) : CUInt; EXTERNALSPEC 'rubberband_retrieve';
function rubberband_get_channel_count(AState : RubberBandState) : CUInt;
  EXTERNALSPEC 'rubberband_get_channel_count';
procedure rubberband_calculate_stretch(AState : RubberBandState);
  EXTERNALSPEC 'rubberband_calculate_stretch';
procedure rubberband_set_debug_level(AState : RubberBandState; level : CInt);
  EXTERNALSPEC 'rubberband_set_debug_level';
procedure rubberband_set_default_debug_level(level : CInt);
  EXTERNALSPEC 'rubberband_set_default_debug_level';



implementation

end.
*)
