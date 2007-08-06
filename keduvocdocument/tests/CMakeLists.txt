
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/   )


########### next target ###############

set(converter_SRCS
   converter.cpp )


kde4_add_executable(converter ${converter_SRCS})

target_link_libraries(converter keduvocdocument )

