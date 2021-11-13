/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef _LAYERSHELLINTEGRATION_P_H
#define _LAYERSHELLINTEGRATION_P_H

#include "layershellqt_export.h"

#include <QtWaylandClient/private/qwaylandclientshellapi_p.h>

namespace LayerShellQt
{
class QWaylandLayerShell;

class LAYERSHELLQT_EXPORT QWaylandLayerShellIntegration : public QtWaylandClient::QWaylandShellIntegration
{
public:
    QWaylandLayerShellIntegration();
    ~QWaylandLayerShellIntegration() override;

    bool initialize() override;
    QtWaylandClient::QWaylandShellSurface *createShellSurface(QtWaylandClient::QWaylandWindow *window) override;

private:
    QScopedPointer<QWaylandLayerShell> m_layerShell;
};

}

#endif
