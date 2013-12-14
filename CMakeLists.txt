project(LibKdeEdu)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(Qt5 5.2 REQUIRED NO_MODULE COMPONENTS Widgets Qml Quick Xml Svg OpenGL PrintSupport Test Xml)
find_package(KF5 5.0.0 REQUIRED NO_MODULE COMPONENTS GuiAddons KDE4Support I18n ConfigWidgets DBusAddons Service Archive KIO)

#add_definitions( -Wall -fprofile-arcs -ftest-coverage )

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
    ${CMAKE_INSTALL_PREFIX}/include/KDE #FIXME
)

remove_definitions(-DQT_NO_CAST_TO_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)
remove_definitions(-DQT_NO_KEYWORDS)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

include(KDEInstallDirs)

add_subdirectory(keduvocdocument)
add_subdirectory(data)
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
