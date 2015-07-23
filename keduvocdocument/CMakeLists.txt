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
    keduvoccontainermodel.cpp
    keduvoclessonmodel.cpp
    keduvocreadonlycontainermodel.cpp
    keduvocvocabularymodel.cpp
    keduvocwordclassmodel.cpp
    keduvoccontainermimedata.cpp
    keduvocvocabularymimedata.cpp
    keduvocvocabularyfilter.cpp
    keduvoctranslator.cpp
    keduvocvocabularydelegate.cpp
    keduvocvocabularyheaderview.cpp
    keduvocvocabularycolumnsdialog.cpp
    keduvoccontainerview.cpp
    keduvocvocabularyview.cpp
    keduvocwordtypeview.cpp
    keduvoclessonview.cpp
    keduvocaudiowidget.cpp
    keduvocmultiplechoicewidget.cpp
    keduvoccomparisonwidget.cpp
    keduvocdeclensionwidget.cpp
    keduvocconjugationwidget.cpp
    keduvocinflectionwidget.cpp
    keduvocimagewidget.cpp
    keduvocimagechooserwidget.cpp
    keduvocsynonymwidget.cpp
    keduvocsummarywordwidget.cpp
    keduvoclatexrenderer.cpp
    keduvoclatexwidget.cpp
    keduvocscript.cpp
    keduvocdocumentproperties.cpp
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

ki18n_wrap_ui(keduvocdocument_LIB_SRCS
    keduvocaudiowidget.ui
    keduvocmultiplechoicewidget.ui
    keduvoccomparisonwidget.ui
    keduvocdeclensionwidget.ui
    keduvocconjugationwidget.ui
    keduvocimagechooserwidget.ui
    keduvocsynonymwidget.ui
    keduvocsummarywordwidget.ui
    keduvoclatexwidget.ui
    keduvocdocumentproperties.ui
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
    KEduVocContainerModel
    KEduVocLessonModel
    KEduVocReadOnlyContainerModel
    KEduVocVocabularyModel
    KEduVocWordClassModel
    KEduVocContainerMimeData
    KEduVocVocabularyMimeData
    KEduVocVocabularyFilter
    KEduVocTranslator
    KEduVocVocabularyDelegate
    KEduVocVocabularyHeaderView
    KEduVocVocabularyColumnsDialog
    KEduVocContainerView
    KEduVocVocabularyView
    KEduVocWordTypeView
    KEduVocLessonView
    KEduVocAudioWidget
    KEduVocMultipleChoiceWidget
    KEduVocComparisonWidget
    KEduVocDeclensionWidget
    KEduVocConjugationWidget
    KEduVocInflectionWidget
    KEduVocImageWidget
    KEduVocImageChooserWidget
    KEduVocSynonymWidget
    KEduVocSummaryWordWidget
    KEduVocLatexRenderer
    KEduVocLatexWidget
    KEduVocScript
    KEduVocDocumentProperties

    REQUIRED_HEADERS KdeEdu_HEADERS
)

add_library(KEduVocDocument SHARED ${keduvocdocument_LIB_SRCS})
generate_export_header(KEduVocDocument BASE_NAME KEduVocDocument)
# vHanda: Add library alias?

target_link_libraries(KEduVocDocument
  PUBLIC
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::DBus
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::Multimedia
    Qt${QT_MAJOR_VERSION}::Widgets
    Qt${QT_MAJOR_VERSION}::Xml
  PRIVATE
    KF5::I18n
    KF5::KIOCore
    KF5::KIOFileWidgets
    KF5::KIOWidgets
    KF5::KIONTLM
    KF5::Archive
    KF5::Completion
    KF5::WidgetsAddons
    KF5::SonnetCore
    KF5::SonnetUi
    KF5::Notifications
    KF5::XmlGui
    KF5::KrossCore
    KF5::KrossUi
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
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::DBus
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::Multimedia
    Qt${QT_MAJOR_VERSION}::Widgets
    Qt${QT_MAJOR_VERSION}::Xml
  PRIVATE
    KF5::I18n
    KF5::KIOCore
    KF5::KIOFileWidgets
    KF5::KIOWidgets
    KF5::KIONTLM
    KF5::Archive
    KF5::Completion
    KF5::WidgetsAddons
    KF5::SonnetCore
    KF5::SonnetUi
    KF5::Notifications
    KF5::XmlGui
    KF5::KrossCore
    KF5::KrossUi
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
