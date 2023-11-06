/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "window.h"
#include "../qwaylandlayershellintegration_p.h"

#include <layershellqt_logging.h>

#include <QPointer>
#include <optional>

#include <QtWaylandClient/private/qwaylandwindow_p.h>

using namespace LayerShellQt;

class LayerShellQt::WindowPrivate
{
public:
    WindowPrivate(QWindow *window)
        : parentWindow(window)
    {
    }

    QWindow *parentWindow;
    QString scope = QStringLiteral("window");
    Window::Anchors anchors = {Window::AnchorTop | Window::AnchorBottom | Window::AnchorLeft | Window::AnchorRight};
    int32_t exclusionZone = 0;
    Window::KeyboardInteractivity keyboardInteractivity = Window::KeyboardInteractivityOnDemand;
    Window::Layer layer = Window::LayerTop;
    QMargins margins;
    Window::ScreenConfiguration screenConfiguration = Window::ScreenFromQWindow;
    bool closeOnDismissed = true;
};

static QMap<QWindow *, Window *> s_map;

Window::~Window()
{
    s_map.remove(d->parentWindow);
}

void Window::setAnchors(Anchors anchors)
{
    if (d->anchors != anchors) {
        d->anchors = anchors;
        Q_EMIT anchorsChanged();
    }
}

Window::Anchors Window::anchors() const
{
    return d->anchors;
}

void Window::setExclusiveZone(int32_t zone)
{
    if (d->exclusionZone != zone) {
        d->exclusionZone = zone;
        Q_EMIT exclusionZoneChanged();
    }
}

int32_t Window::exclusionZone() const
{
    return d->exclusionZone;
}

void Window::setMargins(const QMargins &margins)
{
    if (d->margins != margins) {
        d->margins = margins;
        Q_EMIT marginsChanged();
    }
}

QMargins Window::margins() const
{
    return d->margins;
}

void Window::setKeyboardInteractivity(KeyboardInteractivity interactivity)
{
    if (d->keyboardInteractivity != interactivity) {
        d->keyboardInteractivity = interactivity;
        Q_EMIT keyboardInteractivityChanged();
    }
}

Window::KeyboardInteractivity Window::keyboardInteractivity() const
{
    return d->keyboardInteractivity;
}

void Window::setLayer(Layer layer)
{
    if (d->layer != layer) {
        d->layer = layer;
        Q_EMIT layerChanged();
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

Window::ScreenConfiguration Window::screenConfiguration() const
{
    return d->screenConfiguration;
}

void Window::setScreenConfiguration(Window::ScreenConfiguration screenConfiguration)
{
    d->screenConfiguration = screenConfiguration;
}

bool Window::closeOnDismissed() const
{
    return d->closeOnDismissed;
}

void Window::setCloseOnDismissed(bool close)
{
    d->closeOnDismissed = close;
}

Window::Window(QWindow *window)
    : QObject(window)
    , d(new WindowPrivate(window))
{
    s_map.insert(d->parentWindow, this);

    window->create();

    auto waylandWindow = dynamic_cast<QtWaylandClient::QWaylandWindow *>(window->handle());
    if (!waylandWindow) {
        qCWarning(LAYERSHELLQT) << window << "is not a wayland window. Not creating zwlr_layer_surface";
        return;
    }

    static QWaylandLayerShellIntegration *shellIntegration = nullptr;
    if (!shellIntegration) {
        shellIntegration = new QWaylandLayerShellIntegration();
        if (!shellIntegration->initialize(waylandWindow->display())) {
            delete shellIntegration;
            shellIntegration = nullptr;
            qCWarning(LAYERSHELLQT) << "Failed to initialize layer-shell integration, possibly because compositor does not support the layer-shell protocol";
            return;
        }
    }

    waylandWindow->setShellIntegration(shellIntegration);
}

Window *Window::get(QWindow *window)
{
    if (!window) {
        return nullptr;
    }

    auto layerShellWindow = s_map.value(window);
    if (layerShellWindow) {
        return layerShellWindow;
    }
    return new Window(window);
}

Window *Window::qmlAttachedProperties(QObject *object)
{
    return get(qobject_cast<QWindow *>(object));
}
