INCLUDES= $(all_includes)
SUBDIRS = .  tests

lib_LTLIBRARIES = libkdeeduui.la

libkdeeduui_la_SOURCES = \
            kedusimpleentrydlg.cpp  kedusimpleentrydlgForm.ui

libkdeeduui_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 1:0
libkdeeduui_la_LIBADD = $(LIB_KDECORE) $(LIB_QT)

include_HEADERS =

METASOURCES = AUTO

