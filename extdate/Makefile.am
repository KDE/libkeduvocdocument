bin_PROGRAMS = test_extdate test_extdatepicker

INCLUDES= $(all_includes)
SUBDIRS = . 

lib_LTLIBRARIES = libextdate.la

libextdate_la_SOURCES = extdatetime.cpp extcalendarsystem.cpp extcalendarsystemgregorian.cpp extdatetbl.cpp extdatepicker.cpp extdatetimeedit.cpp extdatewidget.cpp

libextdate_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 3:0:2
libextdate_la_LIBADD = $(LIB_KDECORE) $(LIB_QT) $(LIB_KDEUI)

test_extdate_SOURCES = test_extdate.cc
test_extdate_LDADD = libextdate.la

test_extdatepicker_SOURCES = testwidget.cpp main.cpp
test_extdatepicker_LDADD = libextdate.la

METASOURCES = AUTO

