bin_PROGRAMS = test_extdate test_extdatepicker

INCLUDES= $(all_includes)

lib_LTLIBRARIES = libextdate.la

libextdate_la_SOURCES = extdatetime.cpp extcalendarsystem.cpp extcalendarsystemgregorian.cpp extdatetbl.cpp extdatepicker.cpp extdatetimeedit.cpp extdatewidget.cpp

libextdate_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 3:0:2
libextdate_la_LIBADD = $(LIB_KDEUI)

test_extdate_SOURCES = test_extdate.cc
test_extdate_LDADD = libextdate.la
test_extdate_LDFLAGS = $(all_libraries) $(KDE_RPATH)

test_extdatepicker_SOURCES = testwidget.cpp main.cpp
test_extdatepicker_LDADD = libextdate.la
test_extdatepicker_LDFLAGS = $(all_libraries) $(KDE_RPATH)

METASOURCES = AUTO

