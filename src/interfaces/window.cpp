/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "window.h"
#include "../qwaylandlayersurface_p.h"
#include <layershellqt_logging.h>
#include <private/qwaylandshellsurface_p.h>
#include <private/qwaylandwindow_p.h>

using namespace LayerShellQt;

class LayerShellQt::WindowPrivate
{
public:
    WindowPrivate(QWindow *window)
        : parentWindow(window)
    {
    }

    QWindow *parentWindow;
    QString scope = QStringLiteral("qt");
    Window::Anchors anchor = {Window::AnchorTop | Window::AnchorBottom | Window::AnchorLeft | Window::AnchorRight};
    int32_t exclusionZone = 0;
    bool keyboardInteractivity = false;
    Window::Layer layer = Window::LayerTop;
    QMargins margins;
    QWaylandLayerSurface* getSurface() const;
};

static QMap<QWindow*, Window*> s_map;

Window::~Window()
{
        s_map.remove(d->parentWindow);
}

void Window::setAnchor(Anchors anchor)
{
    d->anchor = anchor;
    if (auto surface = d->getSurface()) {
        surface->setAnchor(anchor);
    }
}

Window::Anchors Window::anchor() const
{
    return d->anchor;
}

void Window::setExclusiveZone(int32_t zone)
{
    d->exclusionZone = zone;
    if (auto surface= d->getSurface()) {
        surface->setExclusiveZone(zone);
    }
}

int32_t Window::exclusionZone() const
{
    return d->exclusionZone;
}

void Window::setMargins(const QMargins &margins)
{
    d->margins = margins;
    if (auto surface= d->getSurface()) {
        surface->setMargins(margins);
    }
}

QMargins Window::margins() const
{
    return d->margins;
}

void Window::setKeyboardInteractivity(bool enabled)
{
    d->keyboardInteractivity = enabled;
    if (auto surface= d->getSurface()) {
        surface->setKeyboardInteractivity(enabled);
    }
}

bool Window::keyboardInteractivity() const
{
    return d->keyboardInteractivity;
}

void Window::setLayer(Layer layer)
{
    d->layer = layer;
    if (auto surface= d->getSurface()) {
        surface->setLayer(layer);
    }
}

void Window::setScope(const QString &scope)
{
    d->scope = scope;
    //this is static and must be set before the platform window is created
}

QString Window::scope() const
{
    return d->scope;
}

Window::Layer Window::layer() const
{
    return d->layer;
}

Window::Window(WindowPrivate *d)
    : QObject(d->parentWindow)
    , d(d)
{
    s_map.insert(d->parentWindow, this);
}

QWaylandLayerSurface *WindowPrivate::getSurface() const
{
    if (!parentWindow) {
        return nullptr;
    }
    auto ww = dynamic_cast<QtWaylandClient::QWaylandWindow *>(parentWindow->handle());
    if (!ww) {
        qCDebug(LAYERSHELLQT) << "window not a wayland window" << parentWindow;
        return nullptr;
    }
    QWaylandLayerSurface *s = qobject_cast<QWaylandLayerSurface *>(ww->shellSurface());
    if (!s) {
        qCDebug(LAYERSHELLQT) << "window not using wlr-layer-shell" << parentWindow << ww->shellSurface();
        return nullptr;
    }
    return s;
}

Window *Window::get(QWindow *window)
{
    if (s_map.contains(window)) {
        return s_map[window];
    }
    return new Window(new WindowPrivate(window));
}
