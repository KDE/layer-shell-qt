# SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(layershellqt)
set(PROJECT_VERSION "6.0.0")
set(PROJECT_VERSION_MAJOR 6)

set(CMAKE_C_STANDARD 99)

set(QT_MIN_VERSION "6.2.0")
set(KF5_MIN_VERSION "5.86")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS WaylandClient Qml)
find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(CMakePackageConfigHelpers)
include(FeatureSummary)
include(GenerateExportHeader)
include(KDEClangFormat)
include(ECMQtDeclareLoggingCategory)

find_package(WaylandScanner)
find_package(QtWaylandScanner)
find_package(Wayland 1.3 COMPONENTS Client Server)
find_package(WaylandProtocols)
find_package(PkgConfig REQUIRED)

set_package_properties(Wayland PROPERTIES
                       TYPE REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu90")

ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX LAYERSHELLQT
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/layershellqt_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LayerShellQtConfigVersion.cmake"
                        SOVERSION 5)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
add_subdirectory(src)
add_subdirectory(tests)

set(CMAKECONFIG_INSTALL_DIR ${KDE_INSTALL_LIBDIR}/cmake/LayerShellQt)
install(EXPORT LayerShellQtTargets
        NAMESPACE LayerShellQt::
        DESTINATION ${CMAKECONFIG_INSTALL_DIR}
        FILE LayerShellQtTargets.cmake)

configure_package_config_file(LayerShellQtConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/LayerShellQtConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LayerShellQtConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/LayerShellQtConfigVersion.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
