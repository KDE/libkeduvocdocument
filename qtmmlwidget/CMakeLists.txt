set(qtmmlwidget_LIB_SRCS
    qtmmlwidget.cpp
)

add_library(qtmmlwidget STATIC ${qtmmlwidget_LIB_SRCS})

target_link_libraries(qtmmlwidget
    PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Xml
)

install(TARGETS qtmmlwidget ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES QtMmlDocument QtMmlWidget qtmmlwidget.h DESTINATION ${INCLUDE_INSTALL_DIR}/libkdeedu/qtmml/ COMPONENT Devel)

