# SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(layershellqt-test main.cpp)
target_link_libraries(layershellqt-test PRIVATE LayerShellQtInterface Qt::Qml Qt::Gui)
