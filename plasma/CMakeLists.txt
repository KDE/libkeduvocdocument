find_package(Plasma)

if(PLASMA_FOUND)
	include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})
	add_subdirectory(engines)
	add_subdirectory(applets)
endif(PLASMA_FOUND)
