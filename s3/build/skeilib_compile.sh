#!/bin/bash

# based on the skeilib compile script
#
# todo:
# - better handling of xlib, xrender, xft, xinput, etc..

# --------------------------------------------------
#
# --------------------------------------------------

# FULLPATH=../plugins/fx_pitch.h
# FILENAME=${FULLPATH##*/}
# BASENAME=${FILENAME%%.*}
# FILEEXTENSION=${FILENAME##*.}
# BASEDIRECTORY=${FULLPATH%$FILENAME}
# echo ----------
# echo "path = $FULLPATH"
# echo "file name = $FILENAME"
# echo "base name = $BASENAME"
# echo "file extension = $FILEEXTENSION"
# echo "base directory = $BASEDIRECTORY"
# echo ----------

# --------------------------------------------------
# default values
# --------------------------------------------------

DEBUG="off"
EXTRA="off"
GPL="off"
TYPE="vst"
GUI="nogui"

# ----------

FLAGS="-Wall "
DEF=""
INC="-I../src "

# INC+="-I../src/extern "

# INC+="-I../src/common "
# INC+="-I../src/common/debug "
# INC+="-I../src/common/types "
# INC+="-I../src/common/containers "
# INC+="-I../src/common/utilities "
# INC+="-I../src/plugin "
# INC+="-I../src/plugin/parameters "
# INC+="-I../src/gui "
# INC+="-I../src/gui/widgets "
# INC+="-I../src/audio "
# INC+="-I../src/audio/filters "
# INC+="-I../src/audio/oscillators "
# INC+="-I../src/gfx "
# INC+="-I../src/gfx/render "

INC+="-I../plugins "
INC+="-I../test "
LIB="-L../lib "
POST="-s -lstdc++ -lm "

# --------------------------------------------------
# help
# --------------------------------------------------

function print_help {
  echo "usage: ./compile <flags>"
  echo "  -i input file name (without path/ext)"
  echo "  -o output file name (without path/ext)"
  echo "  -t type (VST,exe)"
  echo "  -g gui (NOGUI,x11,xrender,cairo,opengl)"
  echo "  -d debug"
  echo "  -h this help"
  echo "  -E use extra features"
  echo "  -F additional gcc flags"
  echo "  -G use gpl"
  echo "  -D additional defines"
  echo "  -I additional include directories"
  echo "  -L additional linked libraries"
  exit 1
}

# --------------------------------------------------
# parse command line
# --------------------------------------------------

if [ "$#" -eq 0 ]; then
  print_help
fi

while getopts i:o:t:g:dheF:GD:I:L: opt
do
  case "$opt" in
    i) INPUT="$OPTARG";;
    o) OUTPUT="$OPTARG";;
    t) TYPE="$OPTARG";;
    g) GUI="$OPTARG";;
    d) DEBUG="on";;
    h) print_help;;
    e) EXTRA="on";;
    F) FLAGS+="$OPTARG ";;
    G) GPL="on";;
    D) DEF+="-D$OPTARG ";;
    I) INC+="-I$OPTARG ";;
    L) LIB+="-l$OPTARG ";;
  esac
done

# shift `expr $OPTIND - 1`
# INC+="-I../extra -I../plugins_extra "

# --------------------------------------------------
#
# --------------------------------------------------

if [ "$OUTPUT" = "" ]; then
  OUTPUT=$INPUT
fi

# ----------

if [ "$DEBUG" = "on" ]; then
  DEF+="-DSKEI_DEBUG "
else
  FLAGS="-O2 "
fi

# ----------

if [ "$EXTRA" = "on" ]; then
  INC+="-I../extra -I../plugins_extra "
fi

# ----------

if [ "$GPL" = "on" ]; then
  INC+="-I../plugins/gpl "
fi

# ----------

if [ "$TYPE" = "exe" ]; then
  DEF+="-DSKEI_EXE "
fi

if [ "$TYPE" = "vst" ]; then
  FLAGS+="-fPIC -shared "
  DEF+="-DSKEI_VST "
fi

# ----------

if [ "$GUI" = "nogui" ]; then
  DEF+="-DSKEI_NOGUI "
fi

if [ "$GUI" = "xlib" ]; then
#  FLAGS+="-lx11 "
  DEF+="-DSKEI_XLIB "
#  LIB+="-lx11 -lxft "
  LIB+="-lx11 -lxrender -lxft "
fi

if [ "$GUI" = "xrender" ]; then
#  FLAGS+="-lx11 -lxrender "
  DEF+="-DSKEI_XLIB -DSKEI_XRENDER "
  LIB+="-lx11 -lxrender -lxft "
fi

# --------------------------------------------------
# printout
# --------------------------------------------------

# echo "input:" $INPUT
# echo "output:" $OUTPUT

# echo "debug:" $DEBUG
# echo "extra:" $EXTRA
# echo "type:" $TYPE
# echo "gui:" $GUI

# echo "flags:" $FLAGS
# echo "def:" $DEF
# echo "inc:" $INC
# echo "lib:" $LIB

# --------------------------------------------------
# create compile.cpp
# --------------------------------------------------

# echo 'compile.cpp:'
echo '#include "skei.h"' >> compile.cpp
echo '#include "'$INPUT'.h"' >> compile.cpp
# echo 'SKEI_MAIN(myPlugin)' >> compile.cpp

# --------------------------------------------------
# finalize command line
# --------------------------------------------------

CMDLINE=""
CMDLINE+=$FLAGS
CMDLINE+=$DEF
CMDLINE+=$INC
CMDLINE+=$LIB

echo "compiling $INPUT"
# echo "gcc" $CMDLINE compile.cpp -o ../bin/$OUTPUT.so $POST
gcc $CMDLINE compile.cpp -o ../bin/$OUTPUT.so $POST
rm compile.cpp

# --------------------------------------------------
#
# --------------------------------------------------

# -fPIC
# Generate position-independent code (PIC) suitable for use in a shared library,
# if supported for the target machine. Such code accesses all constant addresses
# through a global offset table (GOT). The dynamic loader resolves the GOT
# entries when the program starts (the dynamic loader is not part of GCC;
# it is part of the operating system).
#
# -rdynamic
# Pass the flag -export-dynamic to the ELF linker, on targets that support it.
# This instructs the linker to add all symbols, not only used ones, to the
# dynamic symbol table. This option is needed for some uses of dlopen or to
# allow obtaining backtraces from within a program.
#
# -Wl,-Bsymbolic
# When creating a shared library, bind references to global symbols to the
# definition within the shared library, if any. Normally, it is possible
# for a program linked against a shared library to override the definition
# within the shared library.
#
# ----------
#
# linker settings/link libraries:
# ../lib/libluajit.a
# dl
#
# ----------
#
# code::blocks
#
# compiler settings/other options
#   -fPIC
#   -rdynamic
#  -Wl,-Bsynbolic
#
# linker settings/other linker options
#   -Wl,-Bsymbolic
#
# removed from linker options, and it looks like it works anyway:
#   -fPIC
#   -export-dynamic
#


