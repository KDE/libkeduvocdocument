kde4_header()

include_directories( ${CMAKE_SOURCE_DIR}/kdeeducore ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(loader_SRCS loader.cpp )

kde4_automoc(${loader_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(loader ${loader_SRCS})

target_link_libraries(loader  ${QT_AND_KDECORE_LIBS} kdeeducore )

endif(KDE4_BUILD_TESTS)

########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = .
#INCLUDES = -I.  -I$(top_srcdir)/kdeeducore $(all_includes)
#
#check_PROGRAMS = loader
#
##all: check
#
#loader_SOURCES         = loader.cpp
#loader_LDFLAGS = $(all_libraries)
#loader_LDADD = ../libkdeeducore.la
