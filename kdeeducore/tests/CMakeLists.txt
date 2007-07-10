
include_directories( ${CMAKE_SOURCE_DIR}/kdeeducore  )


########### next target ###############
set(loader_SRCS loader.cpp )


kde4_add_test_executable(loader ${loader_SRCS})

target_link_libraries(loader  ${KDE4_KDECORE_LIBS} kdeeducore )

