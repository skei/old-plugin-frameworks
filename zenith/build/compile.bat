@echo off

rem %1 = plugin name (file name, without '.pas')

echo --------- %1.pas ----

del compile.pas
echo {$include zenith.inc} >> compile.pas
echo {$ifdef Z_LIB} library {$else} program {$endif} build_win; >> compile.pas
echo uses {heaptrc,} zenith, %1; >> compile.pas
echo {$include zenith_main.inc} >> compile.pas
rem del compile.pas
                                          
C:\lazarus\fpc\2.4.4\bin\i386-win32\ppc386 compile.pas -v0 -Rintel -MObjFPC -Scgim -O1 -OpPENTIUM4 -Twin32 -Pi386 -Xs -l -Fi..\src -Fi.\units -Fu..\src -Fu..\src\audio -Fu..\src\core -Fu..\src\ext -Fu..\src\gui -Fu..\src\lib -Fu..\src\plugin -Fu. -Fu..\plugins -FUunits\ -FE..\bin\ -dZ_VST -o%1.dll
