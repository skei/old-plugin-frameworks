unit z_logfile;
{$include zenith.inc}

// TODO

//{$ifdef Z_DEBUG}
//  {$define Z_LOG}
//{$endif}


//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{$ifndef Z_LOG}

  type
    ZLogfile = class
    public
      constructor create;
      destructor  destroy; override;
      procedure   write(AText:PChar);
    end;

{$else}

  //uses
  //  Windows;

  type

    ZLogfile = class
    //private
    //  FStdout     : HANDLE;
    public
      constructor create;
      destructor  destroy; override;
      procedure   write(AText:PChar);
    end;

{$endif}

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{$ifndef Z_DEBUG}

constructor ZLogfile.create;
begin
end;

//----------

destructor ZLogfile.destroy;
begin
end;

//----------

procedure ZLogfile.write(AText:PChar);
begin
end;

//------------------------------

{$else}

//uses
//  z_string;

//----------

constructor ZLogfile.create;
begin
end;

//----------

destructor ZLogfile.destroy;
begin
end;

//----------

procedure ZLogfile.write(AText:PChar);
begin
end;

{$endif}

//----------------------------------------------------------------------
end.

