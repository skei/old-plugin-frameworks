program bin2inc;

uses
  SysUtils; // Format

var
  in_file : file of byte;
  in_size : int64;
  in_buffer : array of byte;
  out_file : text;
  num_read : longint;
  buf_pos,line_pos : longint;
  b : byte;

begin

  if paramcount < 3 then
  begin
    writeln('bin2inc <bin_file> <inc_file> <array_name>');
  end
  else
  begin

    WriteLn('converting: ',paramstr(1));

    { read input }

    Assign(in_file,paramstr(1));
    Reset(in_file,1);
    in_size := FileSize(in_file);
    WriteLn('in_file size: ',in_size);
    SetLength(in_buffer,in_size);
    BlockRead(in_file,in_buffer[0],in_size,num_read);
    WriteLn('num read: ',num_read);
    Close(in_file);

    { write output }

    Assign(out_file,paramstr(2));
    Rewrite(out_file);

    write(out_file,'// ');
    writeln(out_file,paramstr(1));
    writeln(out_file,'var');
    writeln(out_file,paramstr(3),'_size : longint = ',in_size,';');
    write(out_file,paramstr(3),' : array[0..',in_size-1,'] of byte = (',#13#10#32#32);
  
    buf_pos := 0;
    line_pos := 0;
    while buf_pos < in_size do
    begin
      b := in_buffer[buf_pos];
      Write(out_file,Format('$%.2x',[b]));
      buf_pos += 1;
      if line_pos < (16-1) then
      begin
        if buf_pos < in_size then Write(out_file,',');
        line_pos := line_pos + 1;
      end
      else
      begin
        if buf_pos < in_size then Write(out_file,',',#13#10#32#32);
        line_pos := 0;
      end;
    end;
  
    { cleanup }
  
    writeln(out_file,#13#10,');');
    SetLength(in_buffer,0);
    Close(out_file);
    
  end; // paramcount
    
end.
