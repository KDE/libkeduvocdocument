SUBDIRS = . tests

INCLUDES= $(all_includes)

lib_LTLIBRARIES = libextdate.la

libextdate_la_SOURCES = extdatetime.cpp extcalendarsystem.cpp extcalendarsystemgregorian.cpp extdatetbl.cpp extdatepicker.cpp extdatetimeedit.cpp extdatewidget.cpp

libextdate_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 3:0:2
libextdate_la_LIBADD = $(LIB_KDEUI)

METASOURCES = AUTO
