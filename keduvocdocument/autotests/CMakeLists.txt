# To add a new unittest
# 1. Add its name to kvocdoc_unittests or kvocdoc_api_unittests

include(ECMMarkAsTest)
include(ECMAddTests)
include(CTest)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ../readerwriters
)

find_package(Qt5 5.2.0 CONFIG REQUIRED Test)

#API interface unit tests linked to exported shared library
set(kvocdoc_api_unittests
  # keduvocdocumentvalidatortest
  # keduvocdocumentfilelockingtest
  # openAPItest
  keduvocdocumenttest.cpp
  emptydocumenttest.cpp
  keduvoctexttest.cpp
)

#Unit Tests linked to objects behind the API wall
set(kvocdoc_unittests
  readerkvtml2test.cpp
  readerwordquiztest.cpp
  readerpaukertest.cpp
  readervokabelntest.cpp
  readerxdxftest.cpp
)

#Helper Clases for unit tests
set(kvocdoc_unittests_helpers
  readerTestHelpers.h
)


macro(KEDUVOCDOCUMENT_NON_GUI_UNIT_TESTS staticOrAPILib)
   foreach(_testname ${ARGN})
     ecm_add_test( ${_testname} ${kvocdoc_unittests_helpers}
       LINK_LIBRARIES ${staticOrAPILib} KF5::CoreAddons KF5::KIOCore Qt5::Test Qt5::Xml
       TEST_NAME ${_testname}
       )
   endforeach()
endmacro()

macro(KEDUVOCDOCUMENT_GUI_UNIT_TESTS staticOrAPILib)
   foreach(_testname ${ARGN})
     ecm_add_test( ${_testname} ${kvocdoc_unittests_helpers}
       LINK_LIBRARIES ${staticOrAPILib} KF5::CoreAddons KF5::KIOCore Qt5::Test Qt5::Xml
       TEST_NAME ${_testname}
       GUI)
   endforeach()
endmacro()

macro(KEDUVOCDOCUMENT_GUI_UNIT_TESTS_STATIC staticOrAPILib)
   foreach(_testname ${ARGN})
     ecm_add_test( ${_testname} ${kvocdoc_unittests_helpers}
       LINK_LIBRARIES ${staticOrAPILib} KF5::CoreAddons KF5::KIOCore Qt5::Test Qt5::Xml
       TEST_NAME ${_testname}
       GUI)
       set_target_properties(${_testname} PROPERTIES COMPILE_FLAGS -DKEDUVOCDOCUMENT_STATIC_DEFINE)
   endforeach()
endmacro()


keduvocdocument_non_gui_unit_tests(KEduVocDocument ${kvocdoc_api_unittests})
keduvocdocument_gui_unit_tests_static(keduvocdocument_static ${kvocdoc_unittests})
