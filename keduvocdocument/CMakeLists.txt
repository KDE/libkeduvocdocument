add_subdirectory(tests)

########### next target ###############

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
# vHanda: ?
# generate_export_header(KEduVocDocument BASE_NAME KEduVocDocument)
# vHanda: Add library alias?

target_link_libraries(KEduVocDocument
  PUBLIC
    KF5::KIOCore
    Qt5::Xml
  PRIVATE
    KF5::KDE4Support
    Qt5::Widgets
)

# vHanda: Add Export Name?
set_target_properties(KEduVocDocument
    PROPERTIES VERSION ${GENERIC_LIB_VERSION}
               SOVERSION ${GENERIC_LIB_SOVERSION}
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
    libkeduvocdocument_export.h
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
    DESTINATION ${INCLUDE_INSTALL_DIR}/libkdeedu  COMPONENT Devel
)
