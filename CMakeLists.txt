project(LibKEduVocDocument)
cmake_minimum_required(VERSION 3.5)

find_package(ECM 1.3.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(Qt5 5.2 REQUIRED NO_MODULE COMPONENTS Xml)
find_package(KF5 5.3.0 REQUIRED COMPONENTS I18n Archive KIO)

#add_definitions( -Wall -fprofile-arcs -ftest-coverage )

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
)


remove_definitions(-DQT_NO_CAST_FROM_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)
remove_definitions(-DQT_NO_KEYWORDS)

add_definitions(-DQT_NO_CAST_TO_ASCII)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DTRANSLATION_DOMAIN=\"libkeduvocdocument\")
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(GenerateExportHeader)

add_subdirectory(keduvocdocument)

configure_file( "${LibKEduVocDocument_SOURCE_DIR}/LibKEduVocDocumentConfig.cmake.in" "${LibKEduVocDocument_BINARY_DIR}/LibKEduVocDocumentConfig.cmake" @ONLY )

set(_LibKEduVocDocument_CONFIG_DEST "${KDE_INSTALL_LIBDIR}/cmake/libkeduvocdocument")

ki18n_install(po)
install( FILES
        "${LibKEduVocDocument_BINARY_DIR}/LibKEduVocDocumentConfig.cmake"
        DESTINATION "${_LibKEduVocDocument_CONFIG_DEST}" )
install( EXPORT LibKEduVocDocumentTargets
         DESTINATION "${_LibKEduVocDocument_CONFIG_DEST}"
         NAMESPACE LibKEduVocDocumentImport__
         FILE LibKEduVocDocumentTargets.cmake )
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
