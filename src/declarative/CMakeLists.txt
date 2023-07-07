# SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleix.pol_gonzalez@mercedes-benz.com>
# SPDX-License-Identifier: BSD-3-Clause

qt_add_qml_module(LayerShellQtQml
                  URI "org.kde.layershell"
                  VERSION 1.0
                  SOURCES layershellqtplugin.cpp)
target_link_libraries(LayerShellQtQml PRIVATE Qt::Qml LayerShellQtInterface)

