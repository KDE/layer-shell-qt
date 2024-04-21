/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "qwaylandlayershellintegration_p.h"
#include "qwaylandlayersurface_p.h"
#include "qwaylandxdgactivationv1_p.h"

#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

namespace LayerShellQt
{
QWaylandLayerShellIntegration::QWaylandLayerShellIntegration()
    : QWaylandShellIntegrationTemplate<QWaylandLayerShellIntegration>(5)
    , m_xdgActivation(new QWaylandXdgActivationV1)
{
}

QWaylandLayerShellIntegration::~QWaylandLayerShellIntegration()
{
    if (object() && zwlr_layer_shell_v1_get_version(object()) >= ZWLR_LAYER_SHELL_V1_DESTROY_SINCE_VERSION) {
        zwlr_layer_shell_v1_destroy(object());
    }
}

QtWaylandClient::QWaylandShellSurface *QWaylandLayerShellIntegration::createShellSurface(QtWaylandClient::QWaylandWindow *window)
{
    return new QWaylandLayerSurface(this, window);
}

}
