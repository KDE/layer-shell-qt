remove_definitions(-DQT_NO_SIGNALS_SLOTS_KEYWORDS)

ecm_add_qtwayland_client_protocol(LAYER_SHELL_SOURCES PROTOCOL ${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml BASENAME xdg-shell)
ecm_add_qtwayland_client_protocol(LAYER_SHELL_SOURCES PROTOCOL wlr-layer-shell-unstable-v1.xml BASENAME wlr-layer-shell-unstable-v1)

add_library(layer-shell SHARED qwaylandlayersurface.cpp qwaylandlayershellintegration.cpp qwaylandlayershell.cpp qwaylandlayershellintegrationplugin.cpp ${LAYER_SHELL_SOURCES})
target_link_libraries(layer-shell Qt5::WaylandClient Wayland::Client Qt5::XkbCommonSupportPrivate Qt::WaylandClientPrivate)
target_include_directories(layer-shell PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/LayerShellQt")

add_library(LayerShellQtInterface SHARED interfaces/window.cpp interfaces/shell.cpp)
target_link_libraries(LayerShellQtInterface PRIVATE layer-shell PUBLIC Qt5::Gui)
target_include_directories(LayerShellQtInterface PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/LayerShellQt"
                                              INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/LayerShellQt>"
)

generate_export_header(LayerShellQtInterface
    BASE_NAME LayerShellQtInterface
    EXPORT_MACRO_NAME LAYERSHELLQT_EXPORT
    EXPORT_FILE_NAME LayerShellQt/layershellqt_export.h
)

install(TARGETS layer-shell
        LIBRARY DESTINATION ${QT_PLUGIN_INSTALL_DIR}/wayland-shell-integration)
