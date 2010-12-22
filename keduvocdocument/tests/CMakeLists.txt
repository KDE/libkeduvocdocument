set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )


kde4_add_unit_test(keduvocdocumentvalidatortest keduvocdocumentvalidatortest.cpp)
target_link_libraries(keduvocdocumentvalidatortest keduvocdocument
    ${KDE4_KDECORE_LIBS} ${QT_QTGUI_LIBRARY} ${QT_QTTEST_LIBRARY} ${QT_QTXML_LIBRARY})


# CONVERTER:


include_directories( .. )


########### next target ###############

set(converter_SRCS
   converter.cpp )


kde4_add_executable(converter ${converter_SRCS})

target_link_libraries(converter keduvocdocument ${KDE4_KDECORE_LIBS})





