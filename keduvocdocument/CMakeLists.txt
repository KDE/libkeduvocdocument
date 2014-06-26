project(libkeduvodocument)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Qt5 5.2 REQUIRED NO_MODULE COMPONENTS Widgets Xml)
find_package(KF5 4.96.0 REQUIRED NO_MODULE COMPONENTS KDE4Support I18n Archive KIO)

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

add_subdirectory(autotests)
add_subdirectory(tests)

########### next target ###############

include(ECMSetupVersion)

ecm_setup_version(5.0.0 VARIABLE_PREFIX KDEEDU
                        VERSION_HEADER "${CMAKE_BINARY_DIR}/keduvocdocument/keduvocdocument_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_BINARY_DIR}/KEduVocDocumentVersion.cmake")

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(keduvocdocument_LIB_SRCS
    keduvocdocument.cpp
    keduvocidentifier.cpp
    keduvocexpression.cpp
    keduvoctranslation.cpp
    keduvoccontainer.cpp
    keduvoclesson.cpp
    keduvocleitnerbox.cpp
    keduvoctext.cpp
    keduvocarticle.cpp
    keduvocconjugation.cpp
    keduvocpersonalpronoun.cpp
    keduvocdeclension.cpp
    keduvocwordtype.cpp
    keduvockvtmlcompability.cpp
    keduvockvtmlreader.cpp
    keduvockvtml2reader.cpp
    keduvockvtml2writer.cpp
    keduvoccsvreader.cpp
    keduvoccsvwriter.cpp
    keduvocpaukerreader.cpp
    keduvocvokabelnreader.cpp
    keduvocwqlreader.cpp
    keduvocxdxfreader.cpp
    sharedkvtmlfiles.cpp
)

add_library(KEduVocDocument SHARED ${keduvocdocument_LIB_SRCS})
generate_export_header(KEduVocDocument BASE_NAME KEduVocDocument)
# vHanda: Add library alias?

target_link_libraries(KEduVocDocument
  PUBLIC
    KF5::KIOCore
    Qt5::Xml
  PRIVATE
    KF5::KDE4Support
    KF5::Archive
    Qt5::Widgets
)

# vHanda: Add Export Name?
set_target_properties(KEduVocDocument
    PROPERTIES VERSION   ${KDEEDU_VERSION_STRING}
               SOVERSION ${KDEEDU_SOVERSION}
)

# if we want to set our own version instead of following kde generic
#set(LIB_KEDUVOCDOCUMENT_VERSION "5.0.0")
#set(LIB_KEDUVOCDOCUMENT_SOVERSION "5")
#set_target_properties(keduvocdocument
#    PROPERTIES VERSION ${LIB_KEDUVOCDOCUMENT_VERSION}
#    SOVERSION ${LIB_KEDUVOCDOCUMENT_SOVERSION}
#)

install(TARGETS KEduVocDocument EXPORT LibKdeEduTargets ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES
    keduvocdocument.h
    keduvocidentifier.h
    keduvocexpression.h
    keduvoctext.h
    keduvocarticle.h
    keduvocconjugation.h
    keduvoclesson.h
    keduvocleitnerbox.h
    keduvoccontainer.h
    keduvocwordflags.h
    keduvocmultiplechoice.h
    keduvoctranslation.h
    keduvocwordtype.h
    keduvocpersonalpronoun.h
    sharedkvtmlfiles.h
    keduvocdeclension.h
    keduvockvtml2writer.h

    ${CMAKE_CURRENT_BINARY_DIR}/keduvocdocument_export.h
    DESTINATION ${INCLUDE_INSTALL_DIR}/libkdeedu  COMPONENT Devel
)
