include_directories(
   ${CMAKE_SOURCE_DIR}/libkdeedu/libscience/
   ${CMAKE_CURRENT_BINARY_DIR}/..
)

set(chemicaldata_engine_SRCS
	chemicaldata.cpp
	)

kde4_automoc(${chemicaldata_engine_SRCS})
kde4_add_plugin(plasma_engine_chemicaldata ${chemicaldata_engine_SRCS})
target_link_libraries(plasma_engine_chemicaldata ${KDE4_KDECORE_LIBS} ${PLASMA_LIBRARIES}  ${QT_QTXML_LIBRARY} science )

install(TARGETS plasma_engine_chemicaldata DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-chemicaldata.desktop DESTINATION ${SERVICES_INSTALL_DIR} )

