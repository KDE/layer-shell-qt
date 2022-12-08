/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef _LAYERSHELL_H
#define _LAYERSHELL_H

#include "layershellqt_export.h"

#include <wayland-client.h>

#include <QtWaylandClient/private/qwaylandshellintegration_p.h>
#include <qwayland-wlr-layer-shell-unstable-v1.h>

namespace LayerShellQt
{
class LAYERSHELLQT_EXPORT QWaylandLayerShell : public QWaylandClientExtensionTemplate<QWaylandLayerShell>, public QtWayland::zwlr_layer_shell_v1
{
public:
    QWaylandLayerShell();
    ~QWaylandLayerShell() override;
};

}

#endif
