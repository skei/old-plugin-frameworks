unit z_file;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

//function ZStripLastDir(ADirectory:string): string;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{
function ZStripLastDir(ADirectory:string): string;
var
  i: Integer;
  lDirectory: string;
begin
  lDirectory:= ADirectory;
  for i := Length(lDirectory) - 1 downto 1 do
  begin
    if lDirectory[i] = '/' then
    begin
      SetLength(lDirectory, Length(ADirectory) - i);
      Result:= lDirectory;
      break;
    end;
  end;
end;
}

//----------------------------------------------------------------------
end.
