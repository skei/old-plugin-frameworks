README FOR FREEPASCAL PORTAUDIO BINDINGS
by the HuMuS Team
http://humus.sourceforge.net


Roland Schaefer, 2010-02-18
mail[at]rolandschaefer[dot]net


Status:
Complete for PortAudio19. Mac extensions not tested - Please
test, report, contribute!


Purpose:
The enclosed portaudio.pp file provides all you need to
write Freepascal application which use the Portaudio
library written in C. It is written manually for the WiSeSLAp
and HuMuS projects. The pa_*.pp units provide special functions
for PortAudio backends. Only use them if the PA binaries on the
target system were compiled with support for the respective unit.


Note:
ISO/GNU Pascal compatibility will only be implemented if enough people
request it. See above email address.


Compilation & Installation:
To compile and run the main unit and some simple tests, just do

$ fpcmake
$ make clean all
$ ./testpasine

You could also consider moving the files in package/unit to your FPC
unit dir. Notice that the pa_*.pp files are _not_ compiled when using
the fpcmake process. Compile and install them as needed on your
system.
