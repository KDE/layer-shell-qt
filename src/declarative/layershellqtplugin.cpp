/*
 *   SPDX-FileCopyrightText: 2023 Aleix Pol Gonzalez <aleix.pol_gonzalez@mercedes-benz.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include <QQmlExtensionPlugin>
#include "../interfaces/window.h"
#include <qqml.h>

QML_DECLARE_TYPEINFO(LayerShellQt::Window, QML_HAS_ATTACHED_PROPERTIES)

class Plugin : public QQmlExtensionPlugin
{
    Q_PLUGIN_METADATA(IID "org.kde.layershellqt")
    Q_OBJECT
public:
    void registerTypes(const char *uri) override {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("org.kde.layershell"));
        qmlRegisterType<LayerShellQt::Window>(uri, 1, 0, "Window");
    }
};

#include "layershellqtplugin.moc"
