if (BUILD_TESTING)
   add_subdirectory(autotests)
   add_subdirectory(tests)
endif()

########### next target ###############

include(ECMSetupVersion)

ecm_setup_version(PROJECT
    VARIABLE_PREFIX KEDUVOCDOCUMENT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/keduvocdocument_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_BINARY_DIR}/KEduVocDocumentVersion.cmake"
)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(keduvocdocument_LIB_SRCS
    keduvocarticle.cpp
    keduvocarticle.h
    keduvoccommon_p.h
    keduvocconjugation.cpp
    keduvocconjugation.h
    keduvoccontainer.cpp
    keduvoccontainer.h
    keduvoccsvwriter.cpp
    keduvoccsvwriter.h
    keduvocdeclension.cpp
    keduvocdeclension.h
    keduvocdocument.cpp
    keduvocdocument.h
    keduvocexpression.cpp
    keduvocexpression.h
    keduvocidentifier.cpp
    keduvocidentifier.h
    keduvockvtml2writer.cpp
    keduvockvtml2writer.h
    keduvockvtmlcompability.cpp
    keduvockvtmlcompability.h
    keduvocleitnerbox.cpp
    keduvocleitnerbox.h
    keduvoclesson.cpp
    keduvoclesson.h
    keduvocmultiplechoice.cpp
    keduvocmultiplechoice.h
    keduvocpersonalpronoun.cpp
    keduvocpersonalpronoun.h
    keduvoctext.cpp
    keduvoctext.h
    keduvoctranslation.cpp
    keduvoctranslation.h
    keduvocwordflags.h
    keduvocwordtype.cpp
    keduvocwordtype.h
    kvtml2defs.h
    kvtmldefs.h
    readerwriters/dummyreader.cpp
    readerwriters/dummyreader.h
    readerwriters/failedreader.cpp
    readerwriters/failedreader.h
    readerwriters/keduvoccsvreader.cpp
    readerwriters/keduvoccsvreader.h
    readerwriters/keduvockvtml2reader.cpp
    readerwriters/keduvockvtml2reader.h
    readerwriters/keduvockvtmlreader.cpp
    readerwriters/keduvockvtmlreader.h
    readerwriters/keduvocpaukerreader.cpp
    readerwriters/keduvocpaukerreader.h
    readerwriters/keduvocvokabelnreader.cpp
    readerwriters/keduvocvokabelnreader.h
    readerwriters/keduvocwqlreader.cpp
    readerwriters/keduvocwqlreader.h
    readerwriters/keduvocxdxfreader.cpp
    readerwriters/keduvocxdxfreader.h
    readerwriters/readerbase.h
    readerwriters/readermanager.cpp
    readerwriters/readermanager.h
    sharedkvtmlfiles.cpp
    sharedkvtmlfiles.h
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
set_target_properties(KEduVocDocument PROPERTIES
    VERSION   ${KEDUVOCDOCUMENT_VERSION}
    SOVERSION ${KEDUVOCDOCUMENT_SOVERSION}
)

# vHanda: Add library alias?
ecm_generate_export_header(KEduVocDocument
    BASE_NAME KEduVocDocument
    VERSION ${KEDUVOCDOCUMENT_VERSION}
    DEPRECATED_BASE_VERSION 0
    USE_VERSION_HEADER
)

target_link_libraries(KEduVocDocument
  PUBLIC
    Qt6::Xml
  PRIVATE
    KF6::I18n
    KF6::KIOCore
    KF6::Archive
)


target_include_directories(KEduVocDocument INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/libkeduvocdocument>")


#Library for non-api unittests
add_library(keduvocdocument_static STATIC ${keduvocdocument_LIB_SRCS})
set_target_properties(keduvocdocument_static PROPERTIES COMPILE_FLAGS -DKEDUVOCDOCUMENT_STATIC_DEFINE)

target_link_libraries(keduvocdocument_static
  PUBLIC
    KF6::KIOCore
    Qt6::Xml
  PRIVATE
    KF6::I18n
    KF6::Archive
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
  ${CMAKE_CURRENT_BINARY_DIR}/keduvocdocument_version.h
  ${KdeEdu_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR}/libkeduvocdocument COMPONENT Devel
)
