/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "qwaylandlayershellintegration_p.h"
#include <QtWaylandClient/private/qwaylandshellintegrationplugin_p.h>

using namespace LayerShellQt;

class QWaylandLayerShellIntegrationPlugin : public QtWaylandClient::QWaylandShellIntegrationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QWaylandShellIntegrationFactoryInterface_iid FILE "layer-shell.json")

public:
    QWaylandLayerShellIntegrationPlugin()
    {
    }

    QtWaylandClient::QWaylandShellIntegration *create(const QString &key, const QStringList &paramList) override
    {
        Q_UNUSED(key);
        Q_UNUSED(paramList);
        return new QWaylandLayerShellIntegration();
    }
};

// Q_IMPORT_PLUGIN(QWaylandLayerShellIntegrationPlugin);

#include "qwaylandlayershellintegrationplugin.moc"
