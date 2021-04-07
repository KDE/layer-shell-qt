/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "qwaylandlayershell_p.h"
#include "qwaylandlayersurface_p.h"

namespace LayerShellQt
{
QWaylandLayerShell::QWaylandLayerShell(QtWayland::zwlr_layer_shell_v1 *shell)
    : QtWayland::zwlr_layer_shell_v1(shell->object())
{
}

QWaylandLayerShell::~QWaylandLayerShell()
{
    if (zwlr_layer_shell_v1_get_version(object()) >= ZWLR_LAYER_SHELL_V1_DESTROY_SINCE_VERSION)
        zwlr_layer_shell_v1_destroy(object());
}

QWaylandLayerSurface *QWaylandLayerShell::createLayerSurface(QtWaylandClient::QWaylandWindow *window)
{
    return new QWaylandLayerSurface(this, window);
}

}
