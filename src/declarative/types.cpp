/*
 *   SPDX-FileCopyrightText: 2023 Aleix Pol Gonzalez <aleix.pol_gonzalez@mercedes-benz.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "types.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 8, 1)
int QQmlMarginsValueType::left() const
{
    return m.left();
}

int QQmlMarginsValueType::top() const
{
    return m.top();
}

int QQmlMarginsValueType::right() const
{
    return m.right();
}

int QQmlMarginsValueType::bottom() const
{
    return m.bottom();
}

void QQmlMarginsValueType::setLeft(int left)
{
    m.setLeft(left);
}

void QQmlMarginsValueType::setTop(int top)
{
    m.setTop(top);
}

void QQmlMarginsValueType::setRight(int right)
{
    m.setRight(right);
}

void QQmlMarginsValueType::setBottom(int bottom)
{
    m.setBottom(bottom);
}
#endif
