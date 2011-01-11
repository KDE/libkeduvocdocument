#! /bin/sh
find keduvocdocument -maxdepth 1 -name \*.cpp -o -name \*.h >>list_libkdeedu

$XGETTEXT --files-from=list_libkdeedu --from-code=UTF-8 -o $podir/libkdeedu.pot

rm -f list_libkdeedu
