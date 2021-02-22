#!/bin/bash

FULLPATH=../plugins/fx_pitch.h
FILENAME=${FULLPATH##*/}
BASENAME=${FILENAME%%.*}
FILEEXTENSION=${FILENAME##*.}
BASEDIRECTORY=${FULLPATH%$FILENAME}

echo ----------
echo "path = $FULLPATH"
echo "file name = $FILENAME"
echo "base name = $BASENAME"
echo "file extension = $FILEEXTENSION"
echo "base directory = $BASEDIRECTORY"
echo ----------

# output
#
#----------
# path = $FULLPATH"
# file name = fx_pitch.h
# base name = fx_pitch
# file extension = h
# base directory = ../plugins/
#----------

