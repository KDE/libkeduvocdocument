#! /bin/sh
find extdate -maxdepth 1 -name \*.cpp >list_libkdeedu
find kdeeduui -maxdepth 1 -name \*.cpp >>list_libkdeedu
find keduvocdocument -maxdepth 1 -name \*.cpp  >>list_libkdeedu
find libscience -maxdepth 1 -name \*.cpp >>list_libkdeedu
find extdate -maxdepth 1 -name \*.cpp >>list_libkdeedu
$XGETTEXT --files-from=list_libkdeedu -o $podir/libkdeedu.pot
rm -f list_libkdeedu
