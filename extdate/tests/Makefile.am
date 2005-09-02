INCLUDES = -I$(top_srcdir)/libkdeedu/extdate $(all_includes)

LDADD = ../libextdate.la

AM_LDFLAGS = $(QT_LDFLAGS) $(X_LDFLAGS) $(KDE_RPATH)

check_PROGRAMS = test_extdate test_extdatepicker

test_extdate_SOURCES = test_extdate.cc
test_extdatepicker_SOURCES = testwidget.cpp main.cpp

METASOURCES = AUTO
