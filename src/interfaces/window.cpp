/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "window.h"
#include "../qwaylandlayershell_p.h"
#include "../qwaylandlayersurface_p.h"
#include <layershellqt_logging.h>
#include <private/qwaylandwindowshellintegration_p.h>
#include <private/qwaylandshellsurface_p.h>
#include <private/qwaylandwindow_p.h>

using namespace LayerShellQt;


class LayerShellQt::WindowPrivate : public QtWaylandClient::QWaylandWindowShellIntegration
{
public:
    WindowPrivate(Window *_q, QWindow *window)
        : QtWaylandClient::QWaylandWindowShellIntegration(window)
        , q(_q)
    {
    }

    Window *q;
    QString scope = QStringLiteral("window");
    Window::Anchors anchors = {Window::AnchorTop | Window::AnchorBottom | Window::AnchorLeft | Window::AnchorRight};
    int32_t exclusionZone = 0;
    Window::KeyboardInteractivity keyboardInteractivity = Window::KeyboardInteractivityExclusive;
    Window::Layer layer = Window::LayerTop;
    QMargins margins;
    QPointer<QWaylandLayerSurface> surface;

    QtWaylandClient::QWaylandShellSurface *createShellSurface(QtWaylandClient::QWaylandWindow *window) override;
};

Window::~Window()
{
}

void Window::setAnchors(Anchors anchors)
{
    d->anchors = anchors;
    if (auto surface = d->surface) {
        surface->setAnchor(anchors);
    }
}

Window::Anchors Window::anchors() const
{
    return d->anchors;
}

void Window::setExclusiveZone(int32_t zone)
{
    d->exclusionZone = zone;
    if (auto surface = d->surface) {
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
    if (auto surface = d->surface) {
        surface->setMargins(margins);
    }
}

QMargins Window::margins() const
{
    return d->margins;
}

void Window::setKeyboardInteractivity(KeyboardInteractivity interactivity)
{
    d->keyboardInteractivity = interactivity;
    if (auto surface = d->surface) {
        surface->setKeyboardInteractivity(interactivity);
    }
}

Window::KeyboardInteractivity Window::keyboardInteractivity() const
{
    return d->keyboardInteractivity;
}

void Window::setLayer(Layer layer)
{
    d->layer = layer;
    if (auto surface = d->surface) {
        surface->setLayer(layer);
    }
}

void Window::setScope(const QString &scope)
{
    d->scope = scope;
    // this is static and must be set before the platform window is created
}

QString Window::scope() const
{
    return d->scope;
}

Window::Layer Window::layer() const
{
    return d->layer;
}

Window::Window(QWindow *window)
    : QObject(window)
    , d(new WindowPrivate(this, window))
{
}

static LayerShellQt::QWaylandLayerShell *s_shell = nullptr;

QtWaylandClient::QWaylandShellSurface *WindowPrivate::createShellSurface(QtWaylandClient::QWaylandWindow *window)
{
    if (!s_shell) {
        s_shell = new LayerShellQt::QWaylandLayerShell;
    }
    return new QWaylandLayerSurface(s_shell, window, q);
}
