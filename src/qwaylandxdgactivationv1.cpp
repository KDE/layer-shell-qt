/** Copyright (C) 2020 Aleix Pol Gonzalez <aleixpol@kde.org>
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "qwaylandxdgactivationv1_p.h"

QWaylandXdgActivationV1::QWaylandXdgActivationV1()
    : QWaylandClientExtensionTemplate<QWaylandXdgActivationV1>(1)
{
    initialize();
}

QWaylandXdgActivationV1::~QWaylandXdgActivationV1()
{
    if (isActive()) {
        destroy();
    }
}

QWaylandXdgActivationTokenV1 *
QWaylandXdgActivationV1::requestXdgActivationToken(::wl_seat *seat, struct ::wl_surface *surface, std::optional<uint32_t> serial, const QString &app_id)
{
    auto wl = get_activation_token();
    auto provider = new QWaylandXdgActivationTokenV1;
    provider->init(wl);

    if (surface) {
        provider->set_surface(surface);
    }
    if (!app_id.isEmpty()) {
        provider->set_app_id(app_id);
    }
    if (serial && seat) {
        provider->set_serial(*serial, seat);
    }
    provider->commit();
    return provider;
}

#include "moc_qwaylandxdgactivationv1_p.cpp"
