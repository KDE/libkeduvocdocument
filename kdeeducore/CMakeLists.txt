
########### next target ###############

set(kdeeducore_LIB_SRCS
   keduvoccsvreader.cpp
   keduvoccsvwriter.cpp
   keduvocdocument.cpp
   keduvocexpression.cpp
   keduvoctranslation.cpp
   keduvocgrade.cpp
   keduvocgrammar.cpp
   keduvockvtmlreader.cpp
   keduvockvtmlwriter.cpp
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

kde4_automoc(${kdeeducore_LIB_SRCS})

kde4_add_ui_files(kdeeducore_LIB_SRCS
   prefleitnerbase.ui
)

kde4_add_library(kdeeducore SHARED ${kdeeducore_LIB_SRCS})

target_link_libraries(kdeeducore  ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY})

set_target_properties(kdeeducore PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS kdeeducore  DESTINATION ${LIB_INSTALL_DIR} )


########### install files ###############

install(FILES
   libkdeedu_core_export.h
   keduvocdocument.h
   keduvocexpression.h
   keduvocgrammar.h
   keduvocmultiplechoice.h
   leitnerbox.h
   leitnersystem.h
   leitnersystemview.h
   sharedkvtmlfiles.h
   prefleitner.h DESTINATION ${INCLUDE_INSTALL_DIR}/libkdeedu
)

