# SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

remove_definitions(-DQT_NO_SIGNALS_SLOTS_KEYWORDS)

add_library(LayerShellQtInterface)

qt6_extract_metatypes(LayerShellQtInterface)

if (Qt6_VERSION VERSION_GREATER_EQUAL "6.8.0")
    set(private_code_option  "PRIVATE_CODE")
endif()
qt6_generate_wayland_protocol_client_sources(LayerShellQtInterface
    ${private_code_option}
    FILES
        ${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml
        ${WaylandProtocols_DATADIR}/staging/xdg-activation/xdg-activation-v1.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/wlr-layer-shell-unstable-v1.xml
)

ecm_qt_declare_logging_category(LayerShellQtInterface
    HEADER
        layershellqt_logging.h
    IDENTIFIER
        LAYERSHELLQT
    CATEGORY_NAME
        layershellqt
)

target_sources(LayerShellQtInterface PRIVATE
    qwaylandxdgactivationv1.cpp
    qwaylandlayersurface.cpp
    qwaylandlayershellintegration.cpp
    interfaces/window.cpp
    interfaces/shell.cpp
)

target_link_libraries(LayerShellQtInterface PUBLIC Qt::Gui)
target_link_libraries(LayerShellQtInterface PRIVATE Qt::WaylandClientPrivate Wayland::Client PkgConfig::XKBCOMMON)
if (TARGET Qt::XkbCommonSupportPrivate)
    target_link_libraries(LayerShellQtInterface PRIVATE Qt::XkbCommonSupportPrivate)
endif()
target_include_directories(LayerShellQtInterface PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/LayerShellQt>"
                                              INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/>"
)

set_target_properties(LayerShellQtInterface PROPERTIES VERSION   ${LAYERSHELLQT_VERSION}
                                                       SOVERSION ${LAYERSHELLQT_SOVERSION}
                                                       EXPORT_NAME Interface
)

add_library(layer-shell SHARED qwaylandlayershellintegrationplugin.cpp)
target_link_libraries(layer-shell LayerShellQtInterface Qt::WaylandClient Qt::WaylandClientPrivate Wayland::Client PkgConfig::XKBCOMMON)
if (TARGET Qt::XkbCommonSupportPrivate)
    target_link_libraries(layer-shell Qt::XkbCommonSupportPrivate)
endif()

ecm_generate_headers(LayerShellQt_HEADERS
    HEADER_NAMES
        Shell
        Window
    RELATIVE interfaces/
    REQUIRED_HEADERS LayerShellQt_HEADERS
)


ecm_generate_export_header(LayerShellQtInterface
    BASE_NAME LayerShellQtInterface
    EXPORT_MACRO_NAME LAYERSHELLQT_EXPORT
    EXPORT_FILE_NAME LayerShellQt/layershellqt_export.h
    VERSION ${LAYERSHELLQT_VERSION}
    DEPRECATION_VERSIONS 6.6
)

install(TARGETS layer-shell
        LIBRARY DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/wayland-shell-integration)

install(TARGETS LayerShellQtInterface EXPORT LayerShellQtTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${LayerShellQt_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/LayerShellQt/layershellqt_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/LayerShellQt COMPONENT Devel
)

add_subdirectory(declarative)
