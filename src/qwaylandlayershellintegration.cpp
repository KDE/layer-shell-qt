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

#include <QPlatformSurfaceEvent>

namespace LayerShellQt
{
QWaylandLayerShellIntegration::QWaylandLayerShellIntegration()
    : QWaylandShellIntegrationTemplate<QWaylandLayerShellIntegration>(5)
    , m_xdgActivation(new QWaylandXdgActivationV1)
{
    qGuiApp->installEventFilter(this);
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

bool QWaylandLayerShellIntegration::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::PlatformSurface && static_cast<QPlatformSurfaceEvent *>(event)->surfaceEventType() == QPlatformSurfaceEvent::SurfaceCreated) {
        QWindow *window = qobject_cast<QWindow *>(watched);
        if (!window) {
            return false;
        }
        auto child = static_cast<QtWaylandClient::QWaylandWindow *>(window->handle());
        auto maybeSetParent = [child]() {
            auto transientParent = child->window()->transientParent();
            if (!transientParent) {
                return;
            }
            if (auto layerParent = qobject_cast<QWaylandLayerSurface *>(static_cast<QtWaylandClient::QWaylandWindow *>(transientParent->handle())->shellSurface())) {
                if (auto topevel = child->surfaceRole<xdg_toplevel>()) {
                    layerParent->set_parent_of(topevel);
                }
            }
        };
        maybeSetParent();
        connect(window, &QWindow::transientParentChanged, child, maybeSetParent);
        connect(child, &QtWaylandClient::QWaylandWindow::surfaceRoleCreated, this, maybeSetParent);
    }
    return false;
}
}
