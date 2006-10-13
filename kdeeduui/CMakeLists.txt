
#add_subdirectory(tests)


########### next target ###############

set(kdeeduui_LIB_SRCS
   kdeeduglossary.cpp
)

kde4_automoc(kdeeduui ${kdeeduui_LIB_SRCS})

kde4_add_library(kdeeduui SHARED ${kdeeduui_LIB_SRCS})

target_link_libraries(kdeeduui  ${KDE4_KDE3SUPPORT_LIBS} ${KDE4_KHTML_LIBS} ${QT_QTXML_LIBRARY})

set_target_properties(kdeeduui PROPERTIES VERSION 3.0.5 SOVERSION 3 )
install(TARGETS kdeeduui  DESTINATION ${LIB_INSTALL_DIR} )


########### install files ###############

install(FILES
   libkdeedu_ui_export.h
   kdeeduglossary.h DESTINATION ${INCLUDE_INSTALL_DIR}/libkdeedu
)
