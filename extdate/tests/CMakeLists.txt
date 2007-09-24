set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/extdate  )


########### next target ###############

set(test_extdate_SRCS test_extdate.cc )


kde4_add_executable(test_extdate TEST ${test_extdate_SRCS})

target_link_libraries(test_extdate  ${KDE4_KDECORE_LIBS} extdate )

########### next target ###############

set(test_extdatepicker_SRCS edpicker_widget.cpp edpicker_main.cpp )


kde4_add_executable(test_extdatepicker TEST ${test_extdatepicker_SRCS})

target_link_libraries(test_extdatepicker  ${KDE4_KDECORE_LIBS} extdate )

########### next target ###############

set(test_extdatetimeedit_SRCS edtedit_widget.cpp edtedit_main.cpp )


kde4_add_executable(test_extdatetimeedit TEST ${test_extdatetimeedit_SRCS})

target_link_libraries(test_extdatetimeedit  ${KDE4_KDECORE_LIBS} extdate )


