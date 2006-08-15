
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
install(TARGETS extdate  DESTINATION ${LIB_INSTALL_DIR} )


