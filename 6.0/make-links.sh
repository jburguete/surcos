#!/usr/bin/env bash
if [ `uname -s` != 'Msys' ]; then
  if [ `uname -s` != 'Darwin' ]; then
    bin=bin/
  fi
  ln -sf ../../../jb/jb src
  ln -sf src/jb/${bin}libjb-3.so
  ln -sf src/jb/${bin}libjbm-3.so
  ln -sf src/jb/${bin}libjbjson-3.so
  ln -sf src/jb/${bin}libjbbin-3.so
  ln -sf src/jb/${bin}libjbwin-3.so
else
  mkdir -p win64/bin
  ln -sf src/jb/bin/libjb-3.dll
  ln -sf src/jb/bin/libjbm-3.dll
  ln -sf src/jb/bin/libjbjson-3.dll
  ln -sf src/jb/bin/libjbbin-3.dll
  ln -sf src/jb/bin/libjbwin-3.dll
fi
