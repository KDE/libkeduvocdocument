project(LibKdeEdu)

#add_definitions( -Wall -fprofile-arcs -ftest-coverage )

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

add_subdirectory(keduvocdocument)
#add_subdirectory(widgets)
add_subdirectory(qtmmlwidget)
add_subdirectory(icons)


configure_file( "${LibKdeEdu_SOURCE_DIR}/LibKdeEduConfig.cmake.in" "${LibKdeEdu_BINARY_DIR}/LibKdeEduConfig.cmake" @ONLY )

if(KDE4_USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR)
    set(_LibKdeEdu_CONFIG_DEST "${LIB_INSTALL_DIR}/cmake/libkdeedu")
else(KDE4_USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR)
    set(_LibKdeEdu_CONFIG_DEST "${LIB_INSTALL_DIR}/libkdeedu")
endif(KDE4_USE_COMMON_CMAKE_PACKAGE_CONFIG_DIR)

install( FILES
        "${LibKdeEdu_BINARY_DIR}/LibKdeEduConfig.cmake"
        DESTINATION "${_LibKdeEdu_CONFIG_DEST}" )
install( EXPORT LibKdeEduTargets
         DESTINATION "${_LibKdeEdu_CONFIG_DEST}"
         NAMESPACE LibKdeEduImport__
         FILE LibKdeEduTargets.cmake )
