@echo off

rem %1 = plugin name (file name, without '.pas')

echo --------- %1.pas ----

del compile.pas
echo {$include sphaera.inc} >> compile.pas
echo library build_win32; >> compile.pas
echo uses %1; >> compile.pas
echo {$include sphaera_vst.inc} >> compile.pas
echo end. >> compile.pas
                                          
C:\lazarus\fpc\2.6.0\bin\i386-win32\ppc386 compile.pas -v0 -Rintel -MObjFPC -Scgim -O1 -OpPENTIUM4 -Twin32 -Pi386 -Xs -l -Fi..\src -Fi.\units -Fu. -Fu..\src -Fu..\ext -Fu..\ext\vst -Fu..\plugins -FUunits\ -FE..\bin\ -o%1.dll

rem del compile.pas
