include(ECMMarkAsTest)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

find_package(Qt5 5.2.0 CONFIG REQUIRED Test)

macro(KEDUVOCDOCUMENT_UNIT_TESTS)
   foreach(_testname ${ARGN})
      add_executable(${_testname} ${_testname}.cpp)
      add_test(kdeedu-${_testname} ${_testname})
      target_link_libraries(${_testname} KEduVocDocument KF5::CoreAddons KF5::KDE4Support Qt5::Test Qt5::Xml)
      ecm_mark_as_test(${_testname})
   endforeach()
endmacro()

keduvocdocument_unit_tests(
#    keduvocdocumentvalidatortest
)
