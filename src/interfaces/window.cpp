/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "window.h"
#include <layershellqt_logging.h>

#include <QPointer>
#include <optional>

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
    Window::KeyboardInteractivity keyboardInteractivity = Window::KeyboardInteractivityExclusive;
    Window::Layer layer = Window::LayerTop;
    QMargins margins;
    std::optional<QPointer<QScreen>> desiredOutput;
};

static QMap<QWindow *, Window *> s_map;

Window::~Window()
{
    s_map.remove(d->parentWindow);
}

void Window::setAnchors(Anchors anchors)
{
    d->anchors = anchors;
    Q_EMIT anchorsChanged();
}

Window::Anchors Window::anchors() const
{
    return d->anchors;
}

void Window::setExclusiveZone(int32_t zone)
{
    d->exclusionZone = zone;
    Q_EMIT exclusionZoneChanged();
}

int32_t Window::exclusionZone() const
{
    return d->exclusionZone;
}

void Window::setMargins(const QMargins &margins)
{
    d->margins = margins;
    Q_EMIT marginsChanged();
}

QMargins Window::margins() const
{
    return d->margins;
}

void Window::setKeyboardInteractivity(KeyboardInteractivity interactivity)
{
    d->keyboardInteractivity = interactivity;
    Q_EMIT keyboardInteractivityChanged();
}

Window::KeyboardInteractivity Window::keyboardInteractivity() const
{
    return d->keyboardInteractivity;
}

void Window::setLayer(Layer layer)
{
    d->layer = layer;
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

QScreen *Window::desiredOutput() const
{
    // Don't use .value_or here to avoid a temporary QPointer
    if (d->desiredOutput.has_value()) {
        return d->desiredOutput.value();
    }

    return d->parentWindow->screen();
}

void Window::setDesiredOutput(QScreen *output)
{
    d->desiredOutput = output;
}

Window::Window(QWindow *window)
    : QObject(window)
    , d(new WindowPrivate(window))
{
    s_map.insert(d->parentWindow, this);
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
