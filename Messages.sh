#! /bin/sh
find kdeeduui -maxdepth 1 -name \*.cpp >>list_libkdeedu
find keduvocdocument -maxdepth 1 -name \*.cpp -o -name \*.h >>list_libkdeedu
find libscience -maxdepth 1 -name \*.cpp >>list_libkdeedu

# extracting a reduced version of the elements.xml, with only the data we want
echo "<?xml version=\"1.0\" ?><tmp>" >element_tiny.xml
cat libscience/data/elements.xml | grep 'bo:name' >>element_tiny.xml
echo "</tmp>" >>element_tiny.xml
$EXTRACTATTR --attr=label,value element_tiny.xml >>rc.cpp
$EXTRACTRC --tag-group=none --tag=scalar element_tiny.xml >>rc.cpp
echo 'rc.cpp' >>list_libkdeedu

$XGETTEXT --files-from=list_libkdeedu --from-code=UTF-8 -o $podir/libkdeedu.pot
rm -f element_tiny.xml
rm -f list_libkdeedu
rm -f rc.cpp
