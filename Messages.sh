#! /bin/sh
find keduvocdocument -maxdepth 1 -name \*.cpp -o -name \*.h >>list_libkeduvocdocument

$XGETTEXT --files-from=list_libkeduvocdocument --from-code=UTF-8 -o $podir/libkeduvocdocument.pot

rm -f list_libkeduvocdocument
