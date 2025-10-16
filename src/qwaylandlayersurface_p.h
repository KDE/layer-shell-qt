/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef _LAYERSURFACE_H
#define _LAYERSURFACE_H

#include <wayland-client.h>

#include "qwaylandlayershellintegration_p.h"

#include "layershellqt_export.h"
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <qwayland-wlr-layer-shell-unstable-v1.h>

namespace LayerShellQt
{

class Window;

class LAYERSHELLQT_EXPORT QWaylandLayerSurface : public QtWaylandClient::QWaylandShellSurface, public QtWayland::zwlr_layer_surface_v1
{
    Q_OBJECT
public:
    QWaylandLayerSurface(QWaylandLayerShellIntegration *shell, QtWaylandClient::QWaylandWindow *window);
    ~QWaylandLayerSurface() override;

    bool isExposed() const override
    {
        return m_configured;
    }
    void attachPopup(QtWaylandClient::QWaylandShellSurface *popup) override;

    void setDesiredSize(const QSize &size);
    void setAnchor(uint32_t anchor);
    void setExclusiveZone(int32_t zone);
    void setExclusiveEdge(uint32_t edge);
    void setMargins(const QMargins &margins);
    void setKeyboardInteractivity(uint32_t interactivity);
    void setLayer(uint32_t layer);

    void applyConfigure() override;
    void setWindowSize(const QSize &size) override;

    bool requestActivate() override;
    bool requestActivateOnShow() override;
    void setXdgActivationToken(const QString &token) override;
    void requestXdgActivationToken(quint32 serial) override;

private:
    void sendExpose();
    void zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height) override;
    void zwlr_layer_surface_v1_closed() override;

    QWaylandLayerShellIntegration *m_shell;
    LayerShellQt::Window *m_interface;
    QtWaylandClient::QWaylandWindow *m_window;
    QSize m_pendingSize;
    QString m_activationToken;

    bool m_configured = false;
};

}

#endif
