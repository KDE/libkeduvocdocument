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

kde4_add_library(keduvocdocument SHARED ${keduvocdocument_LIB_SRCS})

target_link_libraries(keduvocdocument  ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY})

set_target_properties(keduvocdocument
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

install(TARGETS keduvocdocument EXPORT LibKdeEduTargets ${INSTALL_TARGETS_DEFAULT_ARGS})


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
