INCLUDES= $(all_includes)
SUBDIRS = . tests 

lib_LTLIBRARIES = libkdeeducore.la

libkdeeducore_la_SOURCES = keduvocdata.cpp

libkdeeducore_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 1:0

include_HEADERS = keduvocdata.h
# noinst_HEADERS = 

libkdeeducore_la_LIBADD = $(LIB_KDECORE) $(LIB_QT)

METASOURCES = AUTO

