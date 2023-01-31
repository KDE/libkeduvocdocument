include(ECMMarkAsTest)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

macro(KEDUVOCDOCUMENT_EXECUTABLE_TESTS)
   foreach(_testname ${ARGN})
      add_executable(${_testname} ${_testname}.cpp) # TODO NOGUI
      target_link_libraries(${_testname} KEduVocDocument KF${KF_MAJOR_VERSION}::Archive KF${KF_MAJOR_VERSION}::CoreAddons KF${KF_MAJOR_VERSION}::I18n)
      ecm_mark_as_test(${_testname})
   endforeach(_testname)
endmacro()

keduvocdocument_executable_tests(
     sharedkvtmlfilestest
     converter
)
