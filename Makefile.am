INCLUDES= $(all_includes)
SUBDIRS = kdeeduui kdeeducore

lib_LTLIBRARIES = libkdeedu.la

libkdeedu_la_SOURCES = kedudata.cpp

libkdeedu_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 1:0

include_HEADERS = kedudata.h
# noinst_HEADERS = 

libkdeedu_la_LIBADD = $(LIB_KDECORE)

METASOURCES = AUTO

# updatedir =
# update_DATA = 

# messages: 
#	$(XGETTEXT) *.cpp *.h -o $(podir)/libkdeedu.pot
