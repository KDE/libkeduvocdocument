set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include(ECMMarkAsTest)

# vHanda: is this required?
find_package(Qt5 5.2.0 CONFIG REQUIRED Test)

macro(KDEEDU_UNIT_TESTS)
   foreach(_testname ${ARGN})
      add_executable(${_testname} ${_testname}.cpp)
      add_test(kdeedu-${_testname} ${_testname})
      target_link_libraries(${_testname} KF5::CoreAddons Qt5::Test Qt5::Xml)
      ecm_mark_as_test(${_testname})
   endforeach()
endmacro()

kdeedu_unit_tests(
    keduvocdocumentvalidatortest
)

include_directories( .. )


########### next target ###############

#set(converter_SRCS
#   converter.cpp )

#add_executable(converter ${converter_SRCS})
#target_link_libraries(converter KEduVocDocument KF5::CoreAddons KF5::KDE4Support Qt5::Widgets)





