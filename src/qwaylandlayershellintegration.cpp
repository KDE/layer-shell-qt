/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "qwaylandlayershell_p.h"
#include "qwaylandlayershellintegration_p.h"

#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <qwayland-wlr-layer-shell-unstable-v1.h>

namespace LayerShellQt
{
QWaylandLayerShellIntegration::QWaylandLayerShellIntegration()
{
}

QWaylandLayerShellIntegration::~QWaylandLayerShellIntegration()
{
}

bool QWaylandLayerShellIntegration::initialize()
{
    if (m_layerShell) {
        return true;
    }
    wl_registry *registry;
    uint32_t id;
    uint32_t version;
    const bool found = findGlobal(QLatin1String("zwlr_layer_shell_v1"), &registry, &id, &version);
    if (!found) {
        qWarning() << "Could not find the zwlr_layer_shell_v1 global. Compositor does not support the layer shell protocol?";
        return false;
    }

    m_layerShell.reset(new QWaylandLayerShell(registry, id, std::min(version, 4u)));
    return true;
}

QtWaylandClient::QWaylandShellSurface *QWaylandLayerShellIntegration::createShellSurface(QtWaylandClient::QWaylandWindow *window)
{
    if (m_layerShell) {
        return m_layerShell->createLayerSurface(window);
    }
    return nullptr;
}

}
