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
