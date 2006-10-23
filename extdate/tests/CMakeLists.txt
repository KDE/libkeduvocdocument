if(KDE4_BUILD_TESTS)
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/extdate  )


########### next target ###############

set(test_extdate_SRCS test_extdate.cc )

kde4_automoc(${test_extdate_SRCS})

kde4_add_executable(test_extdate ${test_extdate_SRCS})

target_link_libraries(test_extdate  ${KDE4_KDECORE_LIBS} extdate )

########### next target ###############

set(test_extdatepicker_SRCS edpicker_widget.cpp edpicker_main.cpp )

kde4_automoc(${test_extdatepicker_SRCS})

kde4_add_executable(test_extdatepicker ${test_extdatepicker_SRCS})

target_link_libraries(test_extdatepicker  ${KDE4_KDECORE_LIBS} extdate )

########### next target ###############

set(test_extdatetimeedit_SRCS edtedit_widget.cpp edtedit_main.cpp )

kde4_automoc(${test_extdatetimeedit_SRCS})

kde4_add_executable(test_extdatetimeedit ${test_extdatetimeedit_SRCS})

target_link_libraries(test_extdatetimeedit  ${KDE4_KDECORE_LIBS} extdate )

endif(KDE4_BUILD_TESTS)

