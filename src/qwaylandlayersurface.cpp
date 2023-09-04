/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "interfaces/window.h"
#include "layershellqt_logging.h"
#include "qwaylandlayersurface_p.h"

#include <QtWaylandClient/private/qwaylandscreen_p.h>
#include <QtWaylandClient/private/qwaylandsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

namespace LayerShellQt
{
QWaylandLayerSurface::QWaylandLayerSurface(QtWayland::zwlr_layer_shell_v1 *shell, QtWaylandClient::QWaylandWindow *window)
    : QtWaylandClient::QWaylandShellSurface(window)
    , QtWayland::zwlr_layer_surface_v1()
    , m_interface(Window::get(window->window()))
{
    wl_output *output = nullptr;
    if (m_interface->screenConfiguration() == Window::ScreenFromQWindow) {
        auto waylandScreen = dynamic_cast<QtWaylandClient::QWaylandScreen *>(window->window()->screen()->handle());
        // Qt will always assign a screen to a window, but if the compositor has no screens available a dummy QScreen object is created
        // this will not cast to a QWaylandScreen
        if (!waylandScreen) {
            qCWarning(LAYERSHELLQT) << "Creating a layer shell for placeholder screen. This will be positioned incorrectly";
        } else {
            output = waylandScreen->output();
        }
    }
    init(shell->get_layer_surface(window->waylandSurface()->object(), output, m_interface->layer(), m_interface->scope()));
    connect(m_interface, &Window::layerChanged, this, [this]() {
        setLayer(m_interface->layer());
    });

    set_anchor(m_interface->anchors());
    connect(m_interface, &Window::anchorsChanged, this, [this]() {
        set_anchor(m_interface->anchors());
    });
    setExclusiveZone(m_interface->exclusionZone());
    connect(m_interface, &Window::exclusionZoneChanged, this, [this]() {
        setExclusiveZone(m_interface->exclusionZone());
    });

    setMargins(m_interface->margins());
    connect(m_interface, &Window::marginsChanged, this, [this]() {
        setMargins(m_interface->margins());
    });

    setKeyboardInteractivity(m_interface->keyboardInteractivity());
    connect(m_interface, &Window::keyboardInteractivityChanged, this, [this]() {
        setKeyboardInteractivity(m_interface->keyboardInteractivity());
    });

    setDesiredSize(m_interface->desiredSize());
    connect(m_interface, &Window::desiredSizeChanged, this, [this]() {
        setDesiredSize(m_interface->desiredSize());
    });
}

QWaylandLayerSurface::~QWaylandLayerSurface()
{
    destroy();
}

void QWaylandLayerSurface::zwlr_layer_surface_v1_closed()
{
    if (m_interface->closeOnDismissed()) {
        window()->window()->close();
    }
}

void QWaylandLayerSurface::zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height)
{
    ack_configure(serial);
    m_pendingSize = QSize(width, height);

    if (!m_configured) {
        m_configured = true;
        window()->resizeFromApplyConfigure(m_pendingSize);
        window()->handleExpose(QRect(QPoint(), m_pendingSize));
    } else {
        // Later configures are resizes, so we have to queue them up for a time when we
        // are not painting to the window.
        window()->applyConfigureWhenPossible();
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
void QWaylandLayerSurface::attachPopup(QtWaylandClient::QWaylandShellSurface *popup)
{
    std::any anyRole = popup->surfaceRole();

    if (auto role = std::any_cast<::xdg_popup *>(&anyRole)) {
        get_popup(*role);
    } else {
        qCWarning(LAYERSHELLQT) << "Cannot attach popup of unknown type";
    }
}
#endif

void QWaylandLayerSurface::applyConfigure()
{
    window()->resizeFromApplyConfigure(m_pendingSize);
}

void QWaylandLayerSurface::setAnchor(uint anchor)
{
    set_anchor(anchor);
}

void QWaylandLayerSurface::setExclusiveZone(int32_t zone)
{
    set_exclusive_zone(zone);
}

void QWaylandLayerSurface::setMargins(const QMargins &margins)
{
    set_margin(margins.top(), margins.right(), margins.bottom(), margins.left());
}

void QWaylandLayerSurface::setKeyboardInteractivity(uint32_t interactivity)
{
    set_keyboard_interactivity(interactivity);
}

void QWaylandLayerSurface::setLayer(uint32_t layer)
{
    qDebug() << "wtfffffff" << layer << zwlr_layer_surface_v1_get_version(object()) << ZWLR_LAYER_SURFACE_V1_SET_LAYER_SINCE_VERSION;
    if (zwlr_layer_surface_v1_get_version(object()) >= ZWLR_LAYER_SURFACE_V1_SET_LAYER_SINCE_VERSION)
        set_layer(layer);
}

void QWaylandLayerSurface::setDesiredSize(const QSize &size)
{
    set_size(size.width(), size.height());
}

}
