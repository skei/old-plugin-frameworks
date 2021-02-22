
var
  app  : SPHAERA_APPLICATION;

{$R *.res}

begin
  app := SPHAERA_APPLICATION.create;
  app.initialize(nil);
  //..
  app.cleanup;
  app.destroy;
end.

