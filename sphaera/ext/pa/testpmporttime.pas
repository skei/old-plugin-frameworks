{
  PortMidi bindings for FreePascal - Basic PortTime test program
  Latest version available at: http://sourceforge.net/apps/trac/humus/
  
  Copyright (c) 2010 Roland Schaefer

  PortMidi Portable Real-Time MIDI Library
  Latest version available at: http://sourceforge.net/apps/trac/humus/

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

  The text above constitutes the entire PortMidi license; however, 
  the PortMusic community also makes the following non-binding requests:

  Any person wishing to distribute modifications to the Software is
  requested to send the modifications to the original developer so that
  they can be incorporated into the canonical version. It is also
  requested that these non-binding requests be included along with the 
  license above.
}

program TestPMPortTime;

{$MODE FPC}
{$H+}

uses
  PortMidi,
  PortTime;

procedure PtTestCallback(timestamp : PtTimestamp; userData : Pointer); cdecl;
begin 
  Writeln('timestamp is: ', timestamp);
end;

var
  TheError : PtError;

begin
  Writeln('Pt_Start');
  TheError := Pt_Start(10, @PtTestCallback, nil);
  Writeln('error : ',TheError);
  Pt_Sleep(5000);
end.
