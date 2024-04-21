/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef _LAYERSHELLINTEGRATION_P_H
#define _LAYERSHELLINTEGRATION_P_H

#include "layershellqt_export.h"

#include <QtWaylandClient/private/qwaylandshellintegration_p.h>
#include <qwayland-wlr-layer-shell-unstable-v1.h>

class QWaylandXdgActivationV1;

namespace LayerShellQt
{

class LAYERSHELLQT_EXPORT QWaylandLayerShellIntegration : public QtWaylandClient::QWaylandShellIntegrationTemplate<QWaylandLayerShellIntegration>,
                                                          public QtWayland::zwlr_layer_shell_v1
{
public:
    QWaylandLayerShellIntegration();
    ~QWaylandLayerShellIntegration() override;

    QWaylandXdgActivationV1 *activation() const
    {
        return m_xdgActivation.data();
    }
    QtWaylandClient::QWaylandShellSurface *createShellSurface(QtWaylandClient::QWaylandWindow *window) override;

private:
    QScopedPointer<QWaylandXdgActivationV1> m_xdgActivation;
};

}

#endif
