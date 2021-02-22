unit s_quaternion;
interface
implementation
end.

(*

http://rosettacode.org/wiki/Quaternion_type#Delphi

unit Quaternions;

interface

type

  TQuaternion = record
    A, B, C, D: double;

    function  Init          (aA, aB, aC, aD : double): TQuaternion;
    function  Norm          : double;
    function  Conjugate     : TQuaternion;
    function  ToString      : string;

    class operator Negative (Left : TQuaternion): TQuaternion;
    class operator Positive (Left : TQuaternion): TQuaternion;
    class operator Add      (Left, Right : TQuaternion): TQuaternion;
    class operator Add      (Left : TQuaternion; Right : double): TQuaternion; overload;
    class operator Add      (Left : double; Right : TQuaternion): TQuaternion; overload;
    class operator Subtract (Left, Right : TQuaternion): TQuaternion;
    class operator Multiply (Left, Right : TQuaternion): TQuaternion;
    class operator Multiply (Left : TQuaternion; Right : double): TQuaternion; overload;
    class operator Multiply (Left : double; Right : TQuaternion): TQuaternion; overload;
  end;

implementation

uses
  SysUtils;

{ TQuaternion }

function TQuaternion.Init(aA, aB, aC, aD: double): TQuaternion;
begin
  A := aA;
  B := aB;
  C := aC;
  D := aD;

  result := Self;
end;

function TQuaternion.Norm: double;
begin
  result := sqrt(sqr(A) + sqr(B) + sqr(C) + sqr(D));
end;

function TQuaternion.Conjugate: TQuaternion;
begin
  result.B := -B;
  result.C := -C;
  result.D := -D;
end;

class operator TQuaternion.Negative(Left: TQuaternion): TQuaternion;
begin
  result.A := -Left.A;
  result.B := -Left.B;
  result.C := -Left.C;
  result.D := -Left.D;
end;

class operator TQuaternion.Positive(Left: TQuaternion): TQuaternion;
begin
  result := Left;
end;

class operator TQuaternion.Add(Left, Right: TQuaternion): TQuaternion;
begin
  result.A := Left.A + Right.A;
  result.B := Left.B + Right.B;
  result.C := Left.C + Right.C;
  result.D := Left.D + Right.D;
end;

class operator TQuaternion.Add(Left: TQuaternion; Right: double): TQuaternion;
begin
  result.A := Left.A + Right;
  result.B := Left.B;
  result.C := Left.C;
  result.D := Left.D;
end;

class operator TQuaternion.Add(Left: double; Right: TQuaternion): TQuaternion;
begin
  result.A := Left + Right.A;
  result.B := Right.B;
  result.C := Right.C;
  result.D := Right.D;
end;

class operator TQuaternion.Subtract(Left, Right: TQuaternion): TQuaternion;
begin
  result.A := Left.A - Right.A;
  result.B := Left.B - Right.B;
  result.C := Left.C - Right.C;
  result.D := Left.D - Right.D;
end;

class operator TQuaternion.Multiply(Left, Right: TQuaternion): TQuaternion;
begin
  result.A := Left.A * Right.A - Left.B * Right.B - Left.C * Right.C - Left.D * Right.D;
  result.B := Left.A * Right.B + Left.B * Right.A + Left.C * Right.D - Left.D * Right.C;
  result.C := Left.A * Right.C - Left.B * Right.D + Left.C * Right.A + Left.D * Right.B;
  result.D := Left.A * Right.D + Left.B * Right.C - Left.C * Right.B + Left.D * Right.A;
end;

class operator TQuaternion.Multiply(Left: double; Right: TQuaternion): TQuaternion;
begin
  result.A := Left * Right.A;
  result.B := Left * Right.B;
  result.C := Left * Right.C;
  result.D := Left * Right.D;
end;

class operator TQuaternion.Multiply(Left: TQuaternion; Right: double): TQuaternion;
begin
  result.A := Left.A * Right;
  result.B := Left.B * Right;
  result.C := Left.C * Right;
  result.D := Left.D * Right;
end;

function TQuaternion.ToString: string;
begin
  result := Format('%f + %fi + %fj + %fk', [A, B, C, D]);
end;

end.

Test program

program QuaternionTest;

{$APPTYPE CONSOLE}

uses
  Quaternions in 'Quaternions.pas';

var
  r : double;
  q, q1, q2 : TQuaternion;
begin
  r := 7;
  q  := q .Init(1, 2, 3, 4);
  q1 := q1.Init(2, 3, 4, 5);
  q2 := q2.Init(3, 4, 5, 6);

  writeln('q  = ', q.ToString);
  writeln('q1 = ', q1.ToString);
  writeln('q2 = ', q2.ToString);
  writeln('r  = ', r);
  writeln('Norm(q ) = ', q.Norm);
  writeln('Norm(q1) = ', q1.Norm);
  writeln('Norm(q2) = ', q2.Norm);
  writeln('-q = ', (-q).ToString);
  writeln('Conjugate q = ', q.Conjugate.ToString);
  writeln('q1 + q2 = ', (q1 + q2).ToString);
  writeln('q2 + q1 = ', (q2 + q1).ToString);
  writeln('q * r   = ', (q * r).ToString);
  writeln('r * q   = ', (r * q).ToString);
  writeln('q1 * q2 = ', (q1 * q2).ToString);
  writeln('q2 * q1 = ', (q2 * q1).ToString);
end.

Output:

q  = 1.00 + 2.00i + 3.00j + 4.00k
q1 = 2.00 + 3.00i + 4.00j + 5.00k
q2 = 3.00 + 4.00i + 5.00j + 6.00k
r  =  7.00000000000000E+0000
Norm(q ) =  5.47722557505166E+0000
Norm(q1) =  7.34846922834953E+0000
Norm(q2) =  9.27361849549570E+0000
-q = -1.00 + -2.00i + -3.00j + -4.00k
Conjugate q = -1.00 + -2.00i + -3.00j + -4.00k
q1 + q2 = 5.00 + 7.00i + 9.00j + 11.00k
q2 + q1 = 5.00 + 7.00i + 9.00j + 11.00k
q * r   = 7.00 + 14.00i + 21.00j + 28.00k
r * q   = 7.00 + 14.00i + 21.00j + 28.00k
q1 * q2 = -56.00 + 16.00i + 24.00j + 26.00k
q2 * q1 = -56.00 + 18.00i + 20.00j + 28.00k


*)

