/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "shell.h"
#include <QByteArray>
#include <qglobal.h>

using namespace LayerShellQt;

void Shell::useLayerShell()
{
    qputenv("QT_WAYLAND_SHELL_INTEGRATION", "layer-shell");
}
