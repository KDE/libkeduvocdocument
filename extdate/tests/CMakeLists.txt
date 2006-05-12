
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/extdate  )


########### next target ###############

set(test_extdate_SRCS test_extdate.cc )

kde4_automoc(${test_extdate_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(test_extdate ${test_extdate_SRCS})

target_link_libraries(test_extdate  ${KDE4_KDECORE_LIBS} extdate )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(test_extdatepicker_SRCS edpicker_widget.cpp edpicker_main.cpp )

kde4_automoc(${test_extdatepicker_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(test_extdatepicker ${test_extdatepicker_SRCS})

target_link_libraries(test_extdatepicker  ${KDE4_KDECORE_LIBS} extdate )

endif(KDE4_BUILD_TESTS)

########### next target ###############

set(test_extdatetimeedit_SRCS edtedit_widget.cpp edtedit_main.cpp )

kde4_automoc(${test_extdatetimeedit_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(test_extdatetimeedit ${test_extdatetimeedit_SRCS})

target_link_libraries(test_extdatetimeedit  ${KDE4_KDECORE_LIBS} extdate )

endif(KDE4_BUILD_TESTS)

########### install files ###############





#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/libkdeedu/extdate $(all_includes)
#
#AM_LDFLAGS = $(QT_LDFLAGS) $(X_LDFLAGS) $(KDE_RPATH)
#
#check_PROGRAMS = test_extdate test_extdatepicker test_extdatetimeedit
#
#test_extdate_SOURCES = test_extdate.cc
#test_extdate_LDFLAGS = $(all_libraries)
#test_extdate_LDADD = ../libextdate.la
#
#test_extdatepicker_SOURCES = edpicker_widget.cpp edpicker_main.cpp
#test_extdatepicker_LDFLAGS = $(all_libraries)
#test_extdatepicker_LDADD = ../libextdate.la
#
#test_extdatetimeedit_SOURCES = edtedit_widget.cpp edtedit_main.cpp
#test_extdatetimeedit_LDFLAGS = $(all_libraries)
#test_extdatetimeedit_LDADD = ../libextdate.la
#
#METASOURCES = AUTO
