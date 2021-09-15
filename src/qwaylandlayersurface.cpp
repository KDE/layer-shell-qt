/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "interfaces/shell.h"
#include "qwaylandlayershell_p.h"
#include "qwaylandlayersurface_p.h"
#include "layershellqt_logging.h"

#include <QtWaylandClient/private/qwaylandscreen_p.h>
#include <QtWaylandClient/private/qwaylandsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

namespace LayerShellQt
{
QWaylandLayerSurface::QWaylandLayerSurface(QWaylandLayerShell *shell, QtWaylandClient::QWaylandWindow *window)
    : QtWaylandClient::QWaylandShellSurface(window)
    , QtWayland::zwlr_layer_surface_v1()
{
    LayerShellQt::Window *interface = Window::get(window->window());
    Q_ASSERT(interface);

    // Qt will always assign a screen to a window, but if the compositor has no screens available a dummy QScreen object is created
    // this will not cast to a QWaylandScreen
    QtWaylandClient::QWaylandScreen *screen = window->waylandScreen();
    if (screen->isPlaceholder()) {
        qCWarning(LAYERSHELLQT) << "Creating a layer shell for placeholder screen. This will be positioned incorrectly";
    }

    init(shell->get_layer_surface(window->waylandSurface()->object(), screen->isPlaceholder() ? nullptr : screen->output(), interface->layer(), interface->scope()));

    Window::Anchors anchors = interface->anchors();

    set_anchor(interface->anchors());
    setMargins(interface->margins());
    setKeyboardInteractivity(interface->keyboardInteractivity());
    setExclusiveZone(interface->exclusionZone());

    QSize size = window->surfaceSize();
    if ((anchors & Window::AnchorLeft) && (anchors & Window::AnchorRight)) {
        size.setWidth(0);
    }
    if ((anchors & Window::AnchorTop) && (anchors & Window::AnchorBottom)) {
        size.setHeight(0);
    }
    if (size.isValid() && size != QSize(0, 0)) {
        set_size(size.width(), size.height());
    }
}

QWaylandLayerSurface::~QWaylandLayerSurface()
{
    destroy();
}

void QWaylandLayerSurface::zwlr_layer_surface_v1_closed()
{
    window()->window()->close();
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
    if (zwlr_layer_surface_v1_get_version(object()) >= ZWLR_LAYER_SURFACE_V1_SET_LAYER_SINCE_VERSION)
        set_layer(layer);
}

}
