/** Copyright (C) 2020 Aleix Pol Gonzalez <aleixpol@kde.org>
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef QWAYLANDXDGACTIVATIONV1_P_H
#define QWAYLANDXDGACTIVATIONV1_P_H

#include "qwayland-xdg-activation-v1.h"
#include <QObject>

#include <QtWaylandClient/QWaylandClientExtension>

namespace QtWaylandClient
{
class QWaylandDisplay;
class QWaylandSurface;
}

class QWaylandXdgActivationTokenV1 : public QObject, public QtWayland::xdg_activation_token_v1
{
    Q_OBJECT
public:
    ~QWaylandXdgActivationTokenV1() override
    {
        destroy();
    }

protected:
    void xdg_activation_token_v1_done(const QString &token) override
    {
        Q_EMIT done(token);
    }

Q_SIGNALS:
    void done(const QString &token);
};

class QWaylandXdgActivationV1 : public QWaylandClientExtensionTemplate<QWaylandXdgActivationV1>, public QtWayland::xdg_activation_v1
{
public:
    QWaylandXdgActivationV1();
    ~QWaylandXdgActivationV1() override;

    QWaylandXdgActivationTokenV1 *
    requestXdgActivationToken(QtWaylandClient::QWaylandDisplay *display, struct ::wl_surface *surface, std::optional<uint32_t> serial, const QString &app_id);
};

#endif // QWAYLANDXDGACTIVATIONV1_P_H
