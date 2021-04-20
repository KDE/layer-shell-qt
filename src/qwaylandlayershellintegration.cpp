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

bool QWaylandLayerShellIntegration::initialize(QtWaylandClient::QWaylandDisplay *display)
{
    QWaylandShellIntegration::initialize(display);
    display->addRegistryListener(registryLayer, this);
    return m_layerShell != nullptr;
}

QtWaylandClient::QWaylandShellSurface *QWaylandLayerShellIntegration::createShellSurface(QtWaylandClient::QWaylandWindow *window)
{
    return m_layerShell->createLayerSurface(window);
}

void QWaylandLayerShellIntegration::registryLayer(void *data, struct wl_registry *registry, uint32_t id, const QString &interface, uint32_t version)
{
    QWaylandLayerShellIntegration *shell = static_cast<QWaylandLayerShellIntegration *>(data);

    if (interface == zwlr_layer_shell_v1_interface.name)
        shell->m_layerShell = new QWaylandLayerShell(registry, id, std::min(version, 4u));
}

}

//#include "qwaylandlayershellintegration.moc"
