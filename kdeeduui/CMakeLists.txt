kde4_header()

#add_subdirectory(tests)

include_directories(
   ${KDE4_INCLUDE_DIR}
   ${QT_INCLUDES}
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}
)


########### next target ###############

set(kdeeduui_LIB_SRCS
   kdeeduglossary.cpp
)

kde4_automoc(${kdeeduui_LIB_SRCS})

kde4_add_library(kdeeduui SHARED ${kdeeduui_LIB_SRCS})

target_link_libraries(kdeeduui  ${KDE4_KDE3SUPPORT_LIBS} khtml ${QT_QTXML_LIBRARY})

set_target_properties(kdeeduui PROPERTIES VERSION 3.0.5 SOVERSION 3 )
install_targets(${LIB_INSTALL_DIR} kdeeduui )


########### install files ###############

install_files(/include/libkdeedu FILES
   libkdeedu_ui_export.h
   kdeeduglossary.h
)

kde4_footer()

#original Makefile.am contents follow:

#INCLUDES= $(all_includes)
#SUBDIRS = .  tests
#
#lib_LTLIBRARIES = libkdeeduui.la
#
#libkdeeduui_la_SOURCES = \
#            kdeeduglossary.cpp
#
#libkdeeduuiincludedir = $(includedir)/libkdeedu
#libkdeeduuiinclude_HEADERS = kdeeduglossary.h
#
#libkdeeduui_la_LDFLAGS = $(all_libraries) -no-undefined -version-info 3:5:0
#libkdeeduui_la_LIBADD = $(LIB_KDECORE) $(LIB_KIO) $(LIB_KDEUI) $(LIB_KHTML)
#
#METASOURCES = AUTO
#
