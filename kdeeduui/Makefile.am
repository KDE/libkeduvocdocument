INCLUDES= $(all_includes)
SUBDIRS = .  tests

lib_LTLIBRARIES = libkdeeduui.la

libkdeeduui_la_SOURCES = \
            kedusimpleentrydlg.cpp  kedusimpleentrydlgForm.ui kdeeduglossary.cpp

libkdeeduuiincludedir = $(includedir)/libkdeedu
libkdeeduuiinclude_HEADERS = kdeeduglossary.h

libkdeeduui_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 3:5:0
libkdeeduui_la_LIBADD = $(LIB_KDECORE) $(LIB_KIO) $(LIB_KDEUI) $(LIB_KHTML)

METASOURCES = AUTO

