#!/usr/bin/env bash
if [ `uname -s` != 'Darwin' ]; then
    bin=bin/
fi
if [[ `uname -s` = 'NetBSD' || `uname -o` != 'Msys' ]]; then
  ln -sf ../../../jb/jb src
  ln -sf src/jb/${bin}libjb-3.so
  ln -sf src/jb/${bin}libjbm-3.so
  ln -sf src/jb/${bin}libjbjson-3.so
  ln -sf src/jb/${bin}libjbbin-3.so
  ln -sf src/jb/${bin}libjbwin-3.so
else
  bin=win64/bin
  mkdir -p $bin
  ln -sf ../../src/jb/bin/libjb-3.dll $bin
  for i in m json bin win; do
    ln -sf ../../src/jb/bin/libjb${i}-3.dll $bin
  done
fi
