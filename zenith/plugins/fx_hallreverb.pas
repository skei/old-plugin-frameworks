unit fx_hallreverb;
{$include zenith.inc}

// http://forum.cockos.com/showthread.php?t=88409

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  zenith,
  z_const,
  z_types,
  z_descriptor,
  z_instance,
  z_parameter;

const
  BUFFER_SIZE = 1024*1024;

type

  myDescriptor = class(ZDescriptor)
  public
    procedure on_create; override;
  end;

  myInstance = class(ZInstance)
  private
    BUF : array[0..BUFFER_SIZE] of single;
    tmp1,tmp2 : single;
    Rtmp1,Rtmp2 : single;
    slider1,slider2,slider3,slider4 : single;
    needrecalc : boolean;
    //
    l0, l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12  : LongWord;
    Rl0,Rl1,Rl2,Rl3,Rl4,Rl5,Rl6,Rl7,Rl8,Rl9,Rl10,Rl11,Rl12 : LongWord;
    b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12  : LongWord;
    Rb0,Rb1,Rb2,Rb3,Rb4,Rb5,Rb6,Rb7,Rb8,Rb9,Rb10,Rb11,Rb12 : LongWord;
        g1, g2, g3, g4, g5,     g7,     g9,      g11       : single;
    f,d : single;
    dry,wet : single;
    //
    in0, in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12  : single;
    out0,out1,out2,out3,out4,out5,out6,out7,out8,out9,out10,out11,out12 : single;
    p0,  p1,  p2,  p3,  p4,  p5,  p6,  p7,  p8,  p9,  p10,  p11,  p12   : LongWord;
    t1,t2 : single;
    //
    Rin0, Rin1, Rin2, Rin3, Rin4, Rin5, Rin6, Rin7, Rin8, Rin9, Rin10, Rin11, Rin12  : single;
    Rout0,Rout1,Rout2,Rout3,Rout4,Rout5,Rout6,Rout7,Rout8,Rout9,Rout10,Rout11,Rout12 : single;
    Rp0,  Rp1,  Rp2,  Rp3,  Rp4,  Rp5,  Rp6,  Rp7,  Rp8,  Rp9,  Rp10,  Rp11,  Rp12   : LongWord;
    Rt1,Rt2 : single;
  public
    procedure on_create; override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processBlock({%H-}AInputs,{%H-}AOutputs:PPZSample; {%H-}ALength:LongWord); override;
    procedure on_processSample(AInputs,AOutputs:PPZSample); override;
  end;

procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Math, // Power
  z_util,
  z_memory;
  //z_instance_vst;

//----------------------------------------------------------------------

procedure zenith_prepare;
begin
  zenith_register(myDescriptor);
  zenith_register(myInstance);
end;

//----------------------------------------------------------------------

procedure myDescriptor.on_create;
begin
  FName     := 'fx_hallreverb';
  FAuthor   := 'michael gruhn';
  FProduct  := 'powered by zenith';
  FVersion  := 0;
  FUniqueId := Z_MAGIC + $0104;

  ZSetFlag( FFlags,zpf_perSample);
  appendParameter( ZParamFloat.create('Dry',       0,  -48,  0   ) );
  appendParameter( ZParamFloat.create('Wet',      -12, -48,  0   ) );
  appendParameter( ZParamFloat.create('Length',    1,   0.01, 3   ) );
  appendParameter( ZParamFloat.create('PreDelay',  10,  0,    100 ) );
end;

//----------------------------------------------------------------------

procedure myInstance.on_create;
begin
  tmp1 := 0;
  tmp2 := 0;
  ZMemset(@BUF,0,sizeof(BUF));
  needrecalc := true;;
end;

//----------

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  param : ZParameter;
  //index : LongWord;
  value : single;
begin
  param := FParameters[AIndex];
  value := param.from01(AValue);
  //index := AParameter.getParIndex;
  case AIndex of
    0:  begin
          slider1 := value;
          needrecalc:=true;
        end;
    1:  begin
          slider2 := value;
          needrecalc:=true;
        end;
    2:  begin
          slider3 := value;
          needrecalc:=true;
        end;
    3:  begin
          slider4 := value;
          needrecalc:=true;
        end;
  end;
end;

//----------

//function Log(X,Base:single) : single;
//begin
//  result := Ln(X) / Ln(Base);
//end;

procedure myInstance.on_processBlock(AInputs,AOutputs:PPZSample; ALength:LongWord);
begin
  if needrecalc then
  begin
    l0   := trunc( slider4/1000*FSampleRate+1 );
    l1   := 3;
    l2   := 13;
    l3   := 31;
    l4   := 103;
    l5   := 331;
    l6   := 853;
    l7   := 1709;
    l8   := 1117;
    l9   := 631;
    l10  := 887;
    l11  := 1213;
    l12  := 1553;
    Rl0  := trunc( slider4/1000*FSampleRate+1 );
    Rl1  := 3;
    Rl2  := 11;
    Rl3  := 37;
    Rl4  := 113;
    Rl5  := l5+37;
    Rl6  := l6-37;
    Rl7  := l7+37;
    Rl8  := l8-37;
    Rl9  := l9+37;
    Rl10 := l10-37;
    Rl11 := l11+37;
    Rl12 := l12-37;
    b0   := 0;
    b1   := b0+l0;
    b2   := b1+l1;
    b3   := b2+l2;
    b4   := b3+l3;
    b5   := b4+l4;
    b6   := b5+l5;
    b7   := b6+l6;
    b8   := b7+l7;
    b9   := b8+l8;
    b10  := b9+l9;
    b11  := b10+l10;
    b12  := b11+l11;
    Rb0  := b12+l12;
    Rb1  := Rb0+Rl0;
    Rb2  := Rb1+Rl1;
    Rb3  := Rb2+Rl2;
    Rb4  := Rb3+Rl3;
    Rb5  := Rb4+Rl4;
    Rb6  := Rb5+Rl5;
    Rb7  := Rb6+Rl6;
    Rb8  := Rb7+Rl7;
    Rb9  := Rb8+Rl8;
    Rb10 := Rb9+Rl9;
    Rb11 := Rb10+Rl10;
    Rb12 := Rb11+Rl11;
    g1  := 0.62;
    g2  := 0.62;
    g3  := 0.62;
    g4  := 0.62;
    g5  := 0.62;
    g7  := 0.62;
    g9  := 0.62;
    g11 := 0.62;
    f   := Exp( Ln(0.001) / ( slider3*FSampleRate / (l5+l6+l7+l8) ) );
    d   := Exp( -2*Z_PI*18000/FSampleRate );
    dry := Power(10,(slider1/20)); // 10^(slider1/20);
    wet := Power(10,(slider2/20)); // 10^(slider2/20);
    needrecalc := false;
  end;
end;

//----------

procedure myInstance.on_processSample(AInputs,AOutputs:PPZSample);
var
  spl0,spl1 : single;
  _in,_out : single;
  _Rin,_Rout : single;

begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;

  // left
  _in := spl0;
  in0:=_in;  out0:=BUF[b0+p0];        BUF[b0+p0]:=in0;         p0:=(p0+1) mod l0;
  in1:=out0; out1:=BUF[b1+p1]-g1*in1; BUF[b1+p1]:=in1+g1*out1; p1:=(p1+1) mod l1;
  in2:=out1; out2:=BUF[b2+p2]-g2*in2; BUF[b2+p2]:=in2+g2*out2; p2:=(p2+1) mod l2;
  in3:=out2; out3:=BUF[b3+p3]-g3*in3; BUF[b3+p3]:=in3+g3*out3; p3:=(p3+1) mod l3;
  in4:=out3; out4:=BUF[b4+p4]-g4*in4; BUF[b4+p4]:=in4+g4*out4; p4:=(p4+1) mod l4;
  tmp1 := out4 + out12 * f;
  tmp2 := out4 + out8  * f;
  t1   := tmp1 + d * (t1-tmp1);
  tmp1 := t1;
  t2   := tmp2 + d * (t2-tmp2);
  tmp2 := t2;
  in5 :=tmp1;  out5 :=BUF[b5 +p5] -g5*in5;   BUF[b5 +p5] :=in5+g5*out5;    p5 :=(p5+1)  mod l5;
  in6 :=out5;  out6 :=BUF[b6 +p6];           BUF[b6 +p6] :=in6;            p6 :=(p6+1)  mod l6;
  in7 :=out6;  out7 :=BUF[b7 +p7] -g7*in7;   BUF[b7 +p7] :=in7+g7*out7;    p7 :=(p7+1)  mod l7;
  in8 :=out7;  out8 :=BUF[b8 +p8];           BUF[b8 +p8] :=in8;            p8 :=(p8+1)  mod l8;
  in9 :=tmp2;  out9 :=BUF[b9 +p9] -g9*in9;   BUF[b9 +p9] :=in9+g9*out9;    p9 :=(p9+1)  mod l9 ;
  in10:=out9;  out10:=BUF[b10+p10];          BUF[b10+p10]:=in10;           p10:=(p10+1) mod l10;
  in11:=out10; out11:=BUF[b11+p11]-g11*in11; BUF[b11+p11]:=in11+g11*out11; p11:=(p11+1) mod l11;
  in12:=out11; out12:=BUF[b12+p12];          BUF[b12+p12]:=in12;           p12:=(p12+1) mod l12;
  _out := out5 + out7 + out9 + out11;
  spl0 := _out*wet + _in*dry;

  // right
  _Rin := spl1;
  Rin0:=_Rin;  Rout0:=BUF[Rb0+Rp0];         BUF[Rb0+Rp0]:=Rin0;          Rp0:=(Rp0+1) mod Rl0;
  Rin1:=Rout0; Rout1:=BUF[Rb1+Rp1]-g1*Rin1; BUF[Rb1+Rp1]:=Rin1+g1*Rout1; Rp1:=(Rp1+1) mod Rl1;
  Rin2:=Rout1; Rout2:=BUF[Rb2+Rp2]-g2*Rin2; BUF[Rb2+Rp2]:=Rin2+g2*Rout2; Rp2:=(Rp2+1) mod Rl2;
  Rin3:=Rout2; Rout3:=BUF[Rb3+Rp3]-g3*Rin3; BUF[Rb3+Rp3]:=Rin3+g3*Rout3; Rp3:=(Rp3+1) mod Rl3;
  Rin4:=Rout3; Rout4:=BUF[Rb4+Rp4]-g4*Rin4; BUF[Rb4+Rp4]:=Rin4+g4*Rout4; Rp4:=(Rp4+1) mod Rl4;
  Rtmp1 := Rout4 + Rout12 * f;
  Rtmp2 := Rout4 + Rout8  * f;
  Rt1   := Rtmp1 + d * (Rt1-Rtmp1);
  Rtmp1 := Rt1;
  Rt2   := Rtmp2 + d * (Rt2-Rtmp2);
  Rtmp2 := Rt2;
  Rin5 :=Rtmp1;  Rout5 :=BUF[Rb5 +Rp5] -g5*Rin5;   BUF[Rb5 +Rp5] :=Rin5+g5*Rout5;    Rp5 :=(Rp5 +1) mod Rl5;
  Rin6 :=Rout5;  Rout6 :=BUF[Rb6 +Rp6];            BUF[Rb6 +Rp6] :=Rin6;             Rp6 :=(Rp6 +1) mod Rl6;
  Rin7 :=Rout6;  Rout7 :=BUF[Rb7 +Rp7] -g7*Rin7;   BUF[Rb7 +Rp7] :=Rin7+g7*Rout7;    Rp7 :=(Rp7 +1) mod Rl7;
  Rin8 :=Rout7;  Rout8 :=BUF[Rb8 +Rp8];            BUF[Rb8 +Rp8] :=Rin8;             Rp8 :=(Rp8 +1) mod Rl8;
  Rin9 :=Rtmp2;  Rout9 :=BUF[Rb9 +Rp9] -g9*Rin9;   BUF[Rb9 +Rp9 ]:=Rin9+g9*Rout9;    Rp9 :=(Rp9 +1) mod Rl9 ;
  Rin10:=Rout9;  Rout10:=BUF[Rb10+Rp10];           BUF[Rb10+Rp10]:=Rin10;            Rp10:=(Rp10+1) mod Rl10;
  Rin11:=Rout10; Rout11:=BUF[Rb11+Rp11]-g11*Rin11; BUF[Rb11+Rp11]:=Rin11+g11*Rout11; Rp11:=(Rp11+1) mod Rl11;
  Rin12:=Rout11; Rout12:=BUF[Rb12+Rp12];           BUF[Rb12+Rp12]:=Rin12;            Rp12:=(Rp12+1) mod Rl12;
  _Rout := Rout5 + Rout7 + Rout9 + Rout11;
  spl1 := _Rout*wet + _Rin*dry;

  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.

(*

/* Hall Reverb
 *
 * Copyright (c) 2011, Michael Gruhn
 * All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 */
desc:Reverb (Hall)
slider1:0<-120,0,1>Dry (dB)
slider2:-20<-120,0,1>Wet (dB)
slider3:1<0,3,.1>Length (s)
slider4:50<0,100,1>Pre-Delay (ms)

@init
tmp1 = tmp2 = 0;

@slider
l0 = slider4/1000*srate + 1;

l1 = 3;
l2 = 13;
l3 = 31;
l4 = 103;
l5 = 331;
l6 = 853;
l7 = 1709;
l8 = 1117;
l9 = 631;
l10 = 887;
l11 = 1213;
l12 = 1553;

Rl0 = slider4/1000*srate + 1;

Rl1 = 3;
Rl2 = 11;
Rl3 = 37;
Rl4 = 113;
Rl5 = l5+37;
Rl6 = l6-37;
Rl7 = l7+37;
Rl8 = l8-37;
Rl9 = l9+37;
Rl10 = l10-37;
Rl11 = l11+37;
Rl12 = l12-37;

b0 = 0;
b1 = b0+l0;
b2 = b1+l1;
b3 = b2+l2;
b4 = b3+l3;
b5 = b4+l4;
b6 = b5+l5;
b7 = b6+l6;
b8 = b7+l7;
b9 = b8+l8;
b10 = b9+l9;
b11 = b10+l10;
b12 = b11+l11;

Rb0 = b12+l12;
Rb1 = Rb0+Rl0;
Rb2 = Rb1+Rl1;
Rb3 = Rb2+Rl2;
Rb4 = Rb3+Rl3;
Rb5 = Rb4+Rl4;
Rb6 = Rb5+Rl5;
Rb7 = Rb6+Rl6;
Rb8 = Rb7+Rl7;
Rb9 = Rb8+Rl8;
Rb10 = Rb9+Rl9;
Rb11 = Rb10+Rl10;
Rb12 = Rb11+Rl11;

g1 = g2 = g3 = g4 = 0.62;
g5 = 0.62;
g7 = 0.62;
g9 = 0.62;
g11 = 0.62;

f = exp( log(0.001) / ( slider3*srate / (l5+l6+l7+l8) ) );
d = exp( -2*$pi*18000/srate );

dry = 10^(slider1/20);
wet = 10^(slider2/20);

@sample
in = spl0;

in0=in  ;out0=b0[p0]        ;b0[p0]=in0       ;p0=(p0+1)%l0;
in1=out0;out1=b1[p1]-g1*in1;b1[p1]=in1+g1*out1;p1=(p1+1)%l1;
in2=out1;out2=b2[p2]-g2*in2;b2[p2]=in2+g2*out2;p2=(p2+1)%l2;
in3=out2;out3=b3[p3]-g3*in3;b3[p3]=in3+g3*out3;p3=(p3+1)%l3;
in4=out3;out4=b4[p4]-g4*in4;b4[p4]=in4+g4*out4;p4=(p4+1)%l4;

tmp1 = out4 + out12 * f;
tmp2 = out4 + out8  * f;

tmp1 = t1 = tmp1 + d * (t1-tmp1);
tmp2 = t2 = tmp2 + d * (t2-tmp2);

in5=tmp1;out5=b5[p5]-g5*in5;b5[p5]=in5+g5*out5;p5=(p5+1)%l5;
in6=out5;out6=b6[p6]       ;b6[p6]=in6        ;p6=(p6+1)%l6;
in7=out6;out7=b7[p7]-g7*in7;b7[p7]=in7+g7*out7;p7=(p7+1)%l7;
in8=out7;out8=b8[p8]       ;b8[p8]=in8        ;p8=(p8+1)%l8;

in9 =tmp2 ;out9 =b9 [p9 ]-g9 *in9 ;b9 [p9 ]=in9 +g9 *out9 ;p9 =(p9 +1)%l9 ;
in10=out9 ;out10=b10[p10]         ;b10[p10]=in10          ;p10=(p10+1)%l10;
in11=out10;out11=b11[p11]-g11*in11;b11[p11]=in11+g11*out11;p11=(p11+1)%l11;
in12=out11;out12=b12[p12]         ;b12[p12]=in12          ;p12=(p12+1)%l12;

out = out5 + out7 + out9 + out11;

spl0 = out*wet + in*dry;


/* RIGHT CHANNEL */

Rin = spl1;

Rin0=Rin  ;Rout0=Rb0[Rp0]        ;Rb0[Rp0]=Rin0         ;Rp0=(Rp0+1)%Rl0;
Rin1=Rout0;Rout1=Rb1[Rp1]-g1*Rin1;Rb1[Rp1]=Rin1+g1*Rout1;Rp1=(Rp1+1)%Rl1;
Rin2=Rout1;Rout2=Rb2[Rp2]-g2*Rin2;Rb2[Rp2]=Rin2+g2*Rout2;Rp2=(Rp2+1)%Rl2;
Rin3=Rout2;Rout3=Rb3[Rp3]-g3*Rin3;Rb3[Rp3]=Rin3+g3*Rout3;Rp3=(Rp3+1)%Rl3;
Rin4=Rout3;Rout4=Rb4[Rp4]-g4*Rin4;Rb4[Rp4]=Rin4+g4*Rout4;Rp4=(Rp4+1)%Rl4;

Rtmp1 = Rout4 + Rout12 * f;
Rtmp2 = Rout4 + Rout8  * f;

Rtmp1 = Rt1 = Rtmp1 + d * (Rt1-Rtmp1);
Rtmp2 = Rt2 = Rtmp2 + d * (Rt2-Rtmp2);

Rin5=Rtmp1;Rout5=Rb5[Rp5]-g5*Rin5;Rb5[Rp5]=Rin5+g5*Rout5;Rp5=(Rp5+1)%Rl5;
Rin6=Rout5;Rout6=Rb6[Rp6]        ;Rb6[Rp6]=Rin6         ;Rp6=(Rp6+1)%Rl6;
Rin7=Rout6;Rout7=Rb7[Rp7]-g7*Rin7;Rb7[Rp7]=Rin7+g7*Rout7;Rp7=(Rp7+1)%Rl7;
Rin8=Rout7;Rout8=Rb8[Rp8]        ;Rb8[Rp8]=Rin8         ;Rp8=(Rp8+1)%Rl8;

Rin9 =Rtmp2 ;Rout9 =Rb9 [Rp9 ]-g9 *Rin9 ;Rb9 [Rp9 ]=Rin9 +g9 *Rout9 ;Rp9 =(Rp9 +1)%Rl9 ;
Rin10=Rout9 ;Rout10=Rb10[Rp10]          ;Rb10[Rp10]=Rin10           ;Rp10=(Rp10+1)%Rl10;
Rin11=Rout10;Rout11=Rb11[Rp11]-g11*Rin11;Rb11[Rp11]=Rin11+g11*Rout11;Rp11=(Rp11+1)%Rl11;
Rin12=Rout11;Rout12=Rb12[Rp12]          ;Rb12[Rp12]=Rin12           ;Rp12=(Rp12+1)%Rl12;

Rout = Rout5 + Rout7 + Rout9 + Rout11;

spl1 = Rout*wet + Rin*dry;

*)
