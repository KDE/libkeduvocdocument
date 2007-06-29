project(plasma-rypton)

set(rypton_SRCS
    rypton.cpp)

kde4_automoc(${rypton_SRCS})

kde4_add_plugin(plasma_applet_rypton ${rypton_SRCS})
target_link_libraries(plasma_applet_rypton plasma ${KDE4_KIO_LIBS} ${KDE4_KDEUI_LIBS})
set_target_properties(plasma_applet_rypton PROPERTIES VERSION 1.0.0 SOVERSION 1)

install(TARGETS plasma_applet_rypton DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-rypton-default.desktop DESTINATION ${SERVICES_INSTALL_DIR})
