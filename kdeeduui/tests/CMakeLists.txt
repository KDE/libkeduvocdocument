kde4_header()

include_directories( ${CMAKE_SOURCE_DIR}/kdeeduui ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = .
#INCLUDES = -I.  -I$(top_srcdir)/kdeeduui $(all_includes)
#LDADD = ../libkdeeduui.la
#LDFLAGS = $(all_libraries)
#
##check_PROGRAMS = entrydialogs
#check_PROGRAMS = 
#
##all: check
#
##entrydialogs_SOURCES         = entrydialogs.cpp
