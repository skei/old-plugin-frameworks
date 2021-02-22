#!/bin/bash

if [ "$#" -eq 0 ]; then
  echo "usage: ./disasm <file>"
  echo "  <file> input file name (with path/ext)"
  exit 1
fi

objdump -S -d -M intel "$1" > s3.asm

