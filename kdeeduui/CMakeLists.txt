
#add_subdirectory(tests)

########### next target ###############

set(kdeeduui_LIB_SRCS
   kdeeduglossary.cpp
   kedufontutils.cpp
)


kde4_add_library(kdeeduui SHARED ${kdeeduui_LIB_SRCS})

target_link_libraries(kdeeduui ${KDE4_KHTML_LIBS} ${QT_QTXML_LIBRARY})

set_target_properties(kdeeduui PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS kdeeduui EXPORT LibKdeEduTargets ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES
   libkdeedu_ui_export.h
   kdeeduglossary.h kedufontutils.h DESTINATION ${INCLUDE_INSTALL_DIR}/libkdeedu  COMPONENT Devel
)
