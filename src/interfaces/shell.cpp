/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "shell.h"

#if LAYERSHELLQTINTERFACE_BUILD_DEPRECATED_SINCE(6, 6)

#include <QByteArray>
#include <layershellqt_logging.h>
#include <qglobal.h>

using namespace LayerShellQt;

void Shell::useLayerShell()
{
    const bool ret = qputenv("QT_WAYLAND_SHELL_INTEGRATION", "layer-shell");
    if (!ret) {
        qCDebug(LAYERSHELLQT) << "Unable to set QT_WAYLAND_SHELL_INTEGRATION=layer-shell";
    }
}
#endif
