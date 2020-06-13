#!/usr/bin/env bash
if test -x "$(command -v gsed)"; then
	SED=gsed;
else
	SED=sed;
fi
$SED -i "s/"$1"\."$2"/"$3"\."$4"/g" $1.$2/{configure.ac,Doxyfile,src/mainWindow.c,manual/*.tex,locale/*/*/*.po} README.*
git mv $1.$2 $3.$4
