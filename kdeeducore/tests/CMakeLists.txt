
include_directories( ${CMAKE_SOURCE_DIR}/kdeeducore  )


########### next target ###############
if(KDE4_BUILD_TESTS)
set(loader_SRCS loader.cpp )

kde4_automoc(loader ${loader_SRCS})

kde4_add_executable(loader ${loader_SRCS})

target_link_libraries(loader  ${KDE4_KDECORE_LIBS} kdeeducore )

endif(KDE4_BUILD_TESTS)

