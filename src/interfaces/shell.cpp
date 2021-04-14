/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "shell.h"
#include <QByteArray>
#include <layershellqt_logging.h>
#include <qglobal.h>

using namespace LayerShellQt;

void Shell::useLayerShell()
{
    const bool ret = qputenv("QT_WAYLAND_SHELL_INTEGRATION", "layer-shell");
    if (!ret) {
        qCDebug(LAYERSHELLQT) << "Unable to set QT_WAYLAND_SHELL_INTEGRATION=layer-shell";
    }
}

struct Defaults {
    QString scope = QStringLiteral("normal");
    Window::Anchors anchors = {Window::AnchorTop | Window::AnchorBottom | Window::AnchorLeft | Window::AnchorRight};
    Window::Layer layer = Window::LayerTop;
    QMargins margins;
};
Q_GLOBAL_STATIC(Defaults, s_defaults);

void Shell::setDefaultScope(const QString &scope)
{
    s_defaults->scope = scope;
}

QString Shell::defaultScope()
{
    return s_defaults->scope;
}

void Shell::setDefaultLayer(Window::Layer defaultLayer)
{
    s_defaults->layer = defaultLayer;
}

Window::Layer Shell::defaultLayer()
{
    return s_defaults->layer;
}

void Shell::setDefaultAnchors(Window::Anchors anchors)
{
    s_defaults->anchors = anchors;
}

Window::Anchors Shell::defaultAnchors()
{
    return s_defaults->anchors;
}

void Shell::setDefaultMargins(const QMargins &margins)
{
    s_defaults->margins = margins;
}

QMargins Shell::defaultMargins()
{
    return s_defaults->margins;
}
