add_subdirectory(tests)

########### next target ###############

set(keduvocdocument_LIB_SRCS
   keduvoccsvreader.cpp
   keduvoccsvwriter.cpp
   keduvocdocument.cpp
   keduvocexpression.cpp
   keduvoctranslation.cpp
   keduvocgrade.cpp
   keduvocgrammar.cpp
   keduvockvtmlreader.cpp
   keduvockvtml2reader.cpp
   keduvockvtmlwriter.cpp
   keduvockvtml2writer.cpp
   keduvocmultiplechoice.cpp
   keduvocpaukerreader.cpp
   keduvocvokabelnreader.cpp
   keduvocwqlreader.cpp
   keduvocxdxfreader.cpp
   leitnerbox.cpp
   leitnersystem.cpp
   leitnersystemview.cpp
   sharedkvtmlfiles.cpp
   prefleitner.cpp )


kde4_add_ui_files(keduvocdocument_LIB_SRCS
   prefleitnerbase.ui
)

kde4_add_library(keduvocdocument SHARED ${keduvocdocument_LIB_SRCS})

target_link_libraries(keduvocdocument  ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY})

set_target_properties(keduvocdocument PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS keduvocdocument  DESTINATION ${LIB_INSTALL_DIR} )


########### install files ###############

install(FILES
   libkeduvocdocument_export.h
   keduvocdocument.h
   keduvocexpression.h
   keduvocgrade.h
   keduvocgrammar.h
   keduvocmultiplechoice.h
   keduvoctranslation.h
   leitnerbox.h
   leitnersystem.h
   leitnersystemview.h
   sharedkvtmlfiles.h
   prefleitner.h DESTINATION ${INCLUDE_INSTALL_DIR}/libkdeedu
)
