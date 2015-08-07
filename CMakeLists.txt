cmake_minimum_required(VERSION 3.16)
project(LibKEduVocDocument)

set(QT_MIN_VERSION "5.15.0")
set(KF5_MIN_VERSION "5.90.0")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")
find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(GenerateExportHeader)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Core DBus Gui Multimedia Widgets Xml)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS I18n Archive KIO Completion Sonnet Notifications XmlGui Kross NewStuff KCMUtils KHtml)

find_package(LibXml2)
set_package_properties(LibXml2 PROPERTIES
                       URL "http://xmlsoft.org"
                       TYPE OPTIONAL
                       PURPOSE "Required to build HTML export."
                      )
include_directories(${LIBXML2_INCLUDE_DIR})

find_package(LibXslt)
set_package_properties(LibXslt PROPERTIES
                       URL "http://xmlsoft.org/XSLT"
                       TYPE OPTIONAL
                       PURPOSE "Required to build HTML export for Parley."
                      )
include_directories(${LIBXSLT_INCLUDE_DIR})

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

add_subdirectory(keduvocdocument)

configure_file( "${LibKEduVocDocument_SOURCE_DIR}/LibKEduVocDocumentConfig.cmake.in" "${LibKEduVocDocument_BINARY_DIR}/LibKEduVocDocumentConfig.cmake" @ONLY )

set(_LibKEduVocDocument_CONFIG_DEST "${KDE_INSTALL_CMAKEPACKAGEDIR}/libkeduvocdocument")

ki18n_install(po)
install( FILES
        "${LibKEduVocDocument_BINARY_DIR}/LibKEduVocDocumentConfig.cmake"
        DESTINATION "${_LibKEduVocDocument_CONFIG_DEST}" )
install( EXPORT LibKEduVocDocumentTargets
         DESTINATION "${_LibKEduVocDocument_CONFIG_DEST}"
         NAMESPACE LibKEduVocDocumentImport__
         FILE LibKEduVocDocumentTargets.cmake )
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
