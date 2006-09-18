
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

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR}/plugins/designer kdeeduwidgets )

target_link_libraries(kdeeduwidgets ${KDE4_KIO_LIBS} kdeeduplot extdate)

install(TARGETS kdeeduwidgets DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/designer )

