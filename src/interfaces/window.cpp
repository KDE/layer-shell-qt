/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "window.h"
#include <QDebug>
#include <private/qwaylandwindow_p.h>
#include <private/qwaylandshellsurface_p.h>
#include "../qwaylandlayersurface_p.h"

using namespace LayerShellQt;

class LayerShellQt::WindowPrivate
{
public:
    WindowPrivate(QWaylandLayerSurface *surface)
        : surface(surface)
    {
    }

    QWaylandLayerSurface *const surface;
};

Window::~Window() = default;

void Window::setAnchor(Anchor anchor)
{
    d->surface->setAnchor(anchor);
}

void Window::setExclusiveZone(int32_t zone)
{
    d->surface->setExclusiveZone(zone);
}

void Window::setMargins(const QMargins &margins)
{
    d->surface->setMargins(margins);
}

void Window::setKeyboardInteractivity(bool enabled)
{
    d->surface->setKeyboardInteractivity(enabled);
}

Window::Window(WindowPrivate *d)
    : d(d)
{}

Window *Window::get(QWindow *window)
{
    auto ww = dynamic_cast<QtWaylandClient::QWaylandWindow *>(window->handle());
    if (!ww) {
        qDebug() << "window not a wayland window" << window;
        return nullptr;
    }
    QWaylandLayerSurface* s = qobject_cast<QWaylandLayerSurface *>(ww->shellSurface());
    if (!s) {
        qDebug() << "window not using wlr-layer-shell" << window << ww->shellSurface();
        return nullptr;
    }

    return new Window(new WindowPrivate(s));
}
