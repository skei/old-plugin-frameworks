{$include sphaera.inc}
unit s_lock;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{$ifdef SPH_WIN32}
uses Windows;
{$endif}

type

  SLock = class
  private
    {$ifdef SPH_WIN32}
    FHandle : TRTLCriticalSection;
    {$endif}
  public
    constructor create;
    destructor  destroy; override;
    procedure   lock;
    procedure   unlock;
  end;


//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor SLock.create;
begin
  inherited;
  {$ifdef SPH_WIN32}
  InitCriticalSection(FHandle);
  {$endif}
end;

//----------

destructor SLock.destroy;
begin
  {$ifdef SPH_WIN32}
  DoneCriticalsection(FHandle);
  {$endif}
  inherited;
end;

//----------

procedure SLock.lock;
begin
  {$ifdef SPH_WIN32}
  EnterCriticalSection(FHandle);
  {$endif}
end;

//----------

procedure SLock.unlock;
begin
  {$ifdef SPH_WIN32}
  LeaveCriticalSection(FHandle);
  {$endif}
end;

//----------------------------------------------------------------------
end.

