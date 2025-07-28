/*
 *   SPDX-FileCopyrightText: 2023 Aleix Pol Gonzalez <aleix.pol_gonzalez@mercedes-benz.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "../interfaces/window.h"
#include <qqml.h>

QML_DECLARE_TYPEINFO(LayerShellQt::Window, QML_HAS_ATTACHED_PROPERTIES)

class WindowForeign
{
    Q_GADGET
    QML_NAMED_ELEMENT(Window)
    QML_FOREIGN(LayerShellQt::Window)
    QML_UNCREATABLE("")
    QML_ATTACHED(LayerShellQt::Window)
};

// available upstream since https://invent.kde.org/qt/qt/qtdeclarative/-/commit/a398101f715bfc447aa889fc9c58b13bfe75ab47
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 1)
struct Q_QML_EXPORT QQmlMarginsValueType {
    QMargins m;
    Q_PROPERTY(int left READ left WRITE setLeft FINAL)
    Q_PROPERTY(int right READ right WRITE setRight FINAL)
    Q_PROPERTY(int top READ top WRITE setTop FINAL)
    Q_PROPERTY(int bottom READ bottom WRITE setBottom FINAL)
    Q_GADGET
    QML_ANONYMOUS
    QML_FOREIGN(QMargins)
    QML_EXTENDED(QQmlMarginsValueType)
    QML_STRUCTURED_VALUE

public:
    QQmlMarginsValueType() = default;
    Q_INVOKABLE QQmlMarginsValueType(const QMarginsF &margins)
        : m(margins.toMargins())
    {
    }
    int left() const;
    int right() const;
    int top() const;
    int bottom() const;
    void setLeft(int);
    void setRight(int);
    void setTop(int);
    void setBottom(int);

    operator QMargins() const
    {
        return m;
    }
};
#endif
