# SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleix.pol_gonzalez@mercedes-benz.com>
# SPDX-License-Identifier: BSD-3-Clause

ecm_add_qml_module(LayerShellQtQml
                  URI "org.kde.layershell"
                  VERSION 1.0
                  SOURCES types.h
                  GENERATE_PLUGIN_SOURCE)
target_link_libraries(LayerShellQtQml PRIVATE Qt::Qml LayerShellQtInterface)

ecm_finalize_qml_module(LayerShellQtQml DESTINATION ${KDE_INSTALL_QMLDIR})
