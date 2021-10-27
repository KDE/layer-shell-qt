/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef _LAYERSURFACE_H
#define _LAYERSURFACE_H

#include <wayland-client.h>

#include "layershellqt_export.h"
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <qwayland-wlr-layer-shell-unstable-v1.h>

namespace LayerShellQt
{
class QWaylandLayerShell;

class LAYERSHELLQT_EXPORT QWaylandLayerSurface : public QtWaylandClient::QWaylandShellSurface, public QtWayland::zwlr_layer_surface_v1
{
    Q_OBJECT
public:
    QWaylandLayerSurface(QWaylandLayerShell *shell, QtWaylandClient::QWaylandWindow *window);
    ~QWaylandLayerSurface() override;

    bool isExposed() const override
    {
        return m_configured;
    }

    void setAnchor(uint32_t anchor);
    void setExclusiveZone(int32_t zone);
    void setMargins(const QMargins &margins);
    void setKeyboardInteractivity(uint32_t interactivity);
    void setLayer(uint32_t layer);

    void applyConfigure() override;

private:
    void zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height) override;
    void zwlr_layer_surface_v1_closed() override;

    QSize m_pendingSize;
    bool m_configured = false;
};

}

#endif
