
include_directories(
  ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduplot/
  ${CMAKE_SOURCE_DIR}/libkdeedu/extdate/
)


########### next target ###############

set(kdeeduwidgets_PART_SRCS
  kdeeduwidgets.cpp
)

KDE4_ADD_WIDGET_FILES(${kdeeduwidgets_PART_SRCS} kdeedu.widgets)

kde4_add_plugin(kdeeduwidgets ${kdeeduwidgets_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdeeduwidgets )

target_link_libraries(kdeeduwidgets ${KDE4_KIO_LIBS} kdeeduplot extdate)

install(TARGETS kdeeduwidgets DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/designer )


########### install files ###############



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
