kde4_header()

include_directories(${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduplot/  ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/kdeeduwidgets.cpp
  COMMAND ${CMAKE_INSTALL_PREFIX}/bin/makekdewidgets
  ARGS -o ${CMAKE_CURRENT_BINARY_DIR}/kdeeduwidgets.cpp ${CMAKE_CURRENT_SOURCE_DIR}/kdeedu.widgets
  DEPENDS kdeedu.widgets)

########### next target ###############

set(kdeeduwidgets_PART_SRCS kdeeduwidgets.cpp )

qt4_generate_moc(${CMAKE_CURRENT_BINARY_DIR}/kdeeduwidgets.cpp ${CMAKE_CURRENT_BINARY_DIR}/kdeeduwidgets.moc)

kde4_add_plugin(kdeeduwidgets ${kdeeduwidgets_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdeeduwidgets )

target_link_libraries(kdeeduwidgets  ${LIB_KIO} kdeeduplot )

install_targets(${LIB_INSTALL_DIR}/kde4 kdeeduwidgets )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(srcdir)/../kdeeduplot $(all_includes)
#
#AM_CXXFLAGS = -DQT_PLUGIN
##KDE_CXXFLAGS = -UQT3_SUPPORT
#
#METASOURCES = AUTO
#
#kde_widget_LTLIBRARIES = kdeeduwidgets.la
#kdeeduwidgets_la_LDFLAGS = $(KDE_PLUGIN) -module $(all_libraries)
#kdeeduwidgets_la_LIBADD = $(LIB_KIO) ../kdeeduplot/libkdeeduplot.la
#kdeeduwidgets_la_SOURCES = kdeeduwidgets.cpp
#
#kdeeduwidgets.cpp: $(srcdir)/kdeedu.widgets
#	$(MAKEKDEWIDGETS) -o kdeeduwidgets.cpp $(srcdir)/kdeedu.widgets
#
#kdeeduwidgets.lo: kdeeduwidgets.moc
#kdeeduwidgets.moc: kdeeduwidgets.cpp
#	$(MOC) -o kdeeduwidgets.moc kdeeduwidgets.cpp
#
#CLEANFILES = kdeeduwidgets.cpp kdeeduwidgets.moc
