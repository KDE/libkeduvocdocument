INCLUDES = -I$(top_srcdir)/libkdeedu/extdate $(all_includes)

AM_LDFLAGS = $(QT_LDFLAGS) $(X_LDFLAGS) $(KDE_RPATH)

check_PROGRAMS = test_extdate test_extdatepicker test_extdatetimeedit

test_extdate_SOURCES = test_extdate.cc
test_extdate_LDFLAGS = $(all_libraries)
test_extdate_LDADD = ../libextdate.la

test_extdatepicker_SOURCES = edpicker_widget.cpp edpicker_main.cpp
test_extdatepicker_LDFLAGS = $(all_libraries)
test_extdatepicker_LDADD = ../libextdate.la

test_extdatetimeedit_SOURCES = edtedit_widget.cpp edtedit_main.cpp
test_extdatetimeedit_LDFLAGS = $(all_libraries)
test_extdatetimeedit_LDADD = ../libextdate.la

METASOURCES = AUTO
