include(ECMMarkAsTest)
include(ECMAddTests)
include(CTest)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

find_package(Qt5 5.2.0 CONFIG REQUIRED Test)

macro(KEDUVOCDOCUMENT_NON_GUI_UNIT_TESTS)
   foreach(_testname ${ARGN})
     ecm_add_test( ${_testname}
       LINK_LIBRARIES KEduVocDocument KF5::CoreAddons Qt5::Test Qt5::Xml
       TEST_NAME ${_testname}
       )
   endforeach()
endmacro()

macro(KEDUVOCDOCUMENT_GUI_UNIT_TESTS)
   foreach(_testname ${ARGN})
     ecm_add_test( ${_testname}
       LINK_LIBRARIES KEduVocDocument KF5::CoreAddons Qt5::Test Qt5::Xml
       TEST_NAME ${_testname}
       GUI)
   endforeach()
endmacro()

keduvocdocument_gui_unit_tests(
  # keduvocdocumentvalidatortest
  # keduvocdocumentfilelockingtest
)
