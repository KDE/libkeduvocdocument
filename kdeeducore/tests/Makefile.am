SUBDIRS = .
INCLUDES = -I.  -I$(top_srcdir)/kdeeducore $(all_includes)

check_PROGRAMS = loader

#all: check

loader_SOURCES         = loader.cpp
loader_LDFLAGS = $(all_libraries)
loader_LDADD = ../libkdeeducore.la
