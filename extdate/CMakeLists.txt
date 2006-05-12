
add_subdirectory( tests ) 




########### next target ###############

set(extdate_LIB_SRCS 
   extdatetime.cpp 
   extcalendarsystem.cpp 
   extcalendarsystemgregorian.cpp 
   extdatetbl.cpp 
   extdatepicker.cpp 
   extdatetimeedit.cpp 
   extdatewidget.cpp )

kde4_automoc(${extdate_LIB_SRCS})

kde4_add_library(extdate SHARED ${extdate_LIB_SRCS})

target_link_libraries(extdate  ${KDE4_KDE3SUPPORT_LIBS} )

set_target_properties(extdate PROPERTIES VERSION 1.2.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} extdate )


########### install files ###############





#original Makefile.am contents follow:

#SUBDIRS = . tests
#
#INCLUDES= $(all_includes)
#
#lib_LTLIBRARIES = libextdate.la
#
#libextdate_la_SOURCES = extdatetime.cpp extcalendarsystem.cpp extcalendarsystemgregorian.cpp extdatetbl.cpp extdatepicker.cpp extdatetimeedit.cpp extdatewidget.cpp
#
#libextdate_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 3:0:2
#libextdate_la_LIBADD = $(LIB_KDEUI) -lkde3support
#
#METASOURCES = AUTO
