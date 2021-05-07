if (BUILD_TESTING)
   add_subdirectory(autotests)
   add_subdirectory(tests)
endif()

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
    keduvockvtml2writer.cpp
    keduvoccsvwriter.cpp
    readerwriters/dummyreader.cpp
    readerwriters/failedreader.cpp
    readerwriters/keduvockvtmlreader.cpp
    readerwriters/keduvockvtml2reader.cpp
    readerwriters/keduvoccsvreader.cpp
    readerwriters/keduvocpaukerreader.cpp
    readerwriters/keduvocvokabelnreader.cpp
    readerwriters/keduvocwqlreader.cpp
    readerwriters/keduvocxdxfreader.cpp
    readerwriters/readermanager.cpp
    sharedkvtmlfiles.cpp
)

include(ECMGenerateHeaders)
ecm_generate_headers( KdeEdu_HEADERS
  HEADER_NAMES
    KEduVocDocument
    KEduVocIdentifier
    KEduVocExpression
    KEduVocText
    KEduVocArticle
    KEduVocConjugation
    KEduVocLesson
    KEduVocLeitnerBox
    KEduVocContainer
    KEduVocWordFlags
    KEduVocMultipleChoice
    KEduVocTranslation
    KEduVocWordtype
    KEduVocPersonalPronoun
    SharedKVTMLFiles
    KEduVocDeclension
    KEduVocKVTML2Writer

    REQUIRED_HEADERS KdeEdu_HEADERS
)

add_library(KEduVocDocument SHARED ${keduvocdocument_LIB_SRCS})
generate_export_header(KEduVocDocument BASE_NAME KEduVocDocument)
# vHanda: Add library alias?

target_link_libraries(KEduVocDocument
  PUBLIC
    Qt5::Xml
  PRIVATE
    KF5::I18n
    KF5::KIOCore
    KF5::Archive
)


# vHanda: Add Export Name?
set_target_properties(KEduVocDocument
    PROPERTIES VERSION   ${KDEEDU_VERSION}
               SOVERSION ${KDEEDU_SOVERSION}
)
target_include_directories(KEduVocDocument INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/libkeduvocdocument>")


#Library for non-api unittests
add_library(keduvocdocument_static STATIC ${keduvocdocument_LIB_SRCS})
set_target_properties(keduvocdocument_static PROPERTIES COMPILE_FLAGS -DKEDUVOCDOCUMENT_STATIC_DEFINE)

target_link_libraries(keduvocdocument_static
  PUBLIC
    KF5::KIOCore
    Qt5::Xml
  PRIVATE
    KF5::I18n
    KF5::Archive
)



# if we want to set our own version instead of following kde generic
#set(LIB_KEDUVOCDOCUMENT_VERSION "5.0.0")
#set(LIB_KEDUVOCDOCUMENT_SOVERSION "5")
#set_target_properties(keduvocdocument
#    PROPERTIES VERSION ${LIB_KEDUVOCDOCUMENT_VERSION}
#    SOVERSION ${LIB_KEDUVOCDOCUMENT_SOVERSION}
#)

install(TARGETS KEduVocDocument EXPORT LibKEduVocDocumentTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/keduvocdocument_export.h
  ${KdeEdu_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR}/libkeduvocdocument COMPONENT Devel
)
