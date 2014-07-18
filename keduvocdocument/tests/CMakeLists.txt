# To add a new unittest
# 1. Add its name to kvocdoc_unittests or kvocdoc_api_unittests

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( unittest_INCLUDE_DIRS
  ..
  ../readerwriters
)

set(unittest_LINK_LIBS
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTTEST_LIBRARY}
  ${QT_QTXML_LIBRARY}
)

#API interface unit tests linked to exported shared library
set(kvocdoc_api_unittests
  keduvocdocumentvalidatortest
  keduvocdocumentfilelockingtest
  openAPItest
)

#Unit Tests linked to object behind the API wall
set(kvocdoc_unittests
  readerkvtml2test
  readerwordquiztest
  readerpaukertest
  readervokabelntest
  readerxdxftest
)

#Helper Clases for unit tests
set(kvocdoc_unittests_helpers

)

macro(ADD_KVOCDOC_UNITTESTS staticOrAPILib)
  foreach(_unittestname ${ARGN} )

    kde4_add_unit_test(${_unittestname} ${_unittestname}.cpp ${kvocdoc_unittests_helpers} )

    target_link_libraries( ${_unittestname} PRIVATE ${staticOrAPILib} ${unittest_LINK_LIBS} ${kvocdoc_LINK_LIBS} )

  endforeach(_unittestname ${argn} )
endmacro(ADD_KVOCDOC_UNITTESTS)


include_directories( ${unittest_INCLUDE_DIRS} )

ADD_KVOCDOC_UNITTESTS( keduvocdocument_static ${kvocdoc_unittests} )
ADD_KVOCDOC_UNITTESTS( keduvocdocument ${kvocdoc_api_unittests} )


# CONVERTER:


include_directories( .. )


########### next target ###############

set(converter_SRCS
   converter.cpp )


kde4_add_executable(converter ${converter_SRCS})

target_link_libraries(converter keduvocdocument ${KDE4_KDECORE_LIBS})
