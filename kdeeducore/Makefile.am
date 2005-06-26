INCLUDES= $(all_includes)
# there's nothing in tests for the moment, so I'm removing it from the compilation
#SUBDIRS = . tests
SUBDIRS = .

lib_LTLIBRARIES = libkdeeducore.la

libkdeeducore_la_SOURCES = keduvocdata.cpp
libkdeeducore_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 3:0:2

libkdeeducode_includedir = $(includedir)/libkdeedu
libkdeeducode_include_HEADERS = keduvocdata.h

libkdeeducore_la_LIBADD = $(LIB_KDECORE)

METASOURCES = AUTO

