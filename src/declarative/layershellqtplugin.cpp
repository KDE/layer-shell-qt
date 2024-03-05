/*
 *   SPDX-FileCopyrightText: 2023 Aleix Pol Gonzalez <aleix.pol_gonzalez@mercedes-benz.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "../interfaces/window.h"
#include <QQmlExtensionPlugin>
#include <qqml.h>

QML_DECLARE_TYPEINFO(LayerShellQt::Window, QML_HAS_ATTACHED_PROPERTIES)

class ExtQMargins
{
    QMargins m_margins;
    Q_GADGET
    Q_PROPERTY(int left READ left WRITE setLeft FINAL)
    Q_PROPERTY(int right READ right WRITE setRight FINAL)
    Q_PROPERTY(int top READ top WRITE setTop FINAL)
    Q_PROPERTY(int bottom READ bottom WRITE setBottom FINAL)
    QML_FOREIGN(QMargins)
    QML_EXTENDED(ExtQMargins)
public:
    ExtQMargins(const QMargins &margins);
    int left() const { return m_margins.left(); }
    void setLeft(int left) { m_margins.setLeft(left); }

    int right() const { return m_margins.right(); }
    void setRight(int right) { m_margins.setRight(right); }

    int top() const { return m_margins.top(); }
    void setTop(int top) { m_margins.setTop(top); }

    int bottom() const { return m_margins.bottom(); }
    void setBottom(int bottom) { m_margins.setBottom(bottom); }
};

class Plugin : public QQmlExtensionPlugin
{
    Q_PLUGIN_METADATA(IID "org.kde.layershellqt")
    Q_OBJECT
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("org.kde.layershell"));
        qmlRegisterType<LayerShellQt::Window>(uri, 1, 0, "Window");
        qmlRegisterExtendedUncreatableType<QMargins, ExtQMargins>(uri, 1, 0, "ExtQMargins", QStringLiteral("Only created from C++"));
    }
};

#include "layershellqtplugin.moc"
