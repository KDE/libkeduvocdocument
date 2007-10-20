
include_directories(
  ${CMAKE_SOURCE_DIR}/libkdeedu/extdate/
)


########### next target ###############

set(kdeeduwidgets_PART_SRCS
  ${CMAKE_CURRENT_BINARY_DIR}/kdeeduwidgets.cpp
)

KDE4_ADD_WIDGET_FILES(${kdeeduwidgets_PART_SRCS} kdeedu.widgets)

kde4_add_plugin(kdeeduwidgets ${kdeeduwidgets_PART_SRCS})



target_link_libraries(kdeeduwidgets ${KDE4_KDEUI_LIBS} extdate)

install(TARGETS kdeeduwidgets DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/designer )

