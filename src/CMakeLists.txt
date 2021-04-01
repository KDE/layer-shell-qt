remove_definitions(-DQT_NO_SIGNALS_SLOTS_KEYWORDS)

ecm_add_qtwayland_client_protocol(LAYER_SHELL_SOURCES PROTOCOL ${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml BASENAME xdg-shell)
ecm_add_qtwayland_client_protocol(LAYER_SHELL_SOURCES PROTOCOL wlr-layer-shell-unstable-v1.xml BASENAME wlr-layer-shell-unstable-v1)

add_library(layer-shell SHARED qwaylandlayershellintegrationplugin.cpp)
target_link_libraries(layer-shell LayerShellQtInterface Qt5::WaylandClient Qt::WaylandClientPrivate Qt5::XkbCommonSupportPrivate)

add_library(LayerShellQtInterface SHARED qwaylandlayersurface.cpp interfaces/window.cpp interfaces/shell.cpp qwaylandlayershellintegration.cpp qwaylandlayershell.cpp ${LAYER_SHELL_SOURCES})
target_link_libraries(LayerShellQtInterface PRIVATE Qt5::Gui Qt::WaylandClientPrivate Qt5::XkbCommonSupportPrivate Wayland::Client)
target_include_directories(LayerShellQtInterface PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/LayerShellQt>"
                                              INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/>"
)

set_target_properties(LayerShellQtInterface PROPERTIES VERSION   ${LAYERSHELLQT_VERSION_STRING}
                                                       SOVERSION ${LAYERSHELLQT_SOVERSION}
                                                       EXPORT_NAME Interface
)

ecm_generate_headers(LayerShellQt_HEADERS
    HEADER_NAMES
        Shell
        Window
    RELATIVE interfaces/
    REQUIRED_HEADERS LayerShellQt_HEADERS
)


generate_export_header(LayerShellQtInterface
    BASE_NAME LayerShellQtInterface
    EXPORT_MACRO_NAME LAYERSHELLQT_EXPORT
    EXPORT_FILE_NAME LayerShellQt/layershellqt_export.h
)

install(TARGETS layer-shell
        LIBRARY DESTINATION ${QT_PLUGIN_INSTALL_DIR}/wayland-shell-integration)

install(TARGETS LayerShellQtInterface EXPORT LayerShellQtTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${LayerShellQt_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/LayerShellQt/layershellqt_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/LayerShellQt COMPONENT Devel
)
