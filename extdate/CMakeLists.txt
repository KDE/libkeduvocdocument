
add_subdirectory( tests ) 

add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)



########### next target ###############

set(extdate_LIB_SRCS 
   extdatetime.cpp 
   extcalendarsystem.cpp 
   extcalendarsystemgregorian.cpp 
   extdatetbl.cpp 
   extdatepicker.cpp 
   extdatetimeedit.cpp 
   extdatewidget.cpp )


kde4_add_library(extdate SHARED ${extdate_LIB_SRCS})

target_link_libraries(extdate  ${KDE4_KDEUI_LIBS} ${QT_QT3SUPPORT_LIBRARY})

set_target_properties(extdate PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS extdate  DESTINATION ${LIB_INSTALL_DIR})


