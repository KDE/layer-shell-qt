/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LAYERSHELLQTSHELL_H
#define LAYERSHELLQTSHELL_H

#include "layershellqt_export.h"

#if LAYERSHELLQTINTERFACE_ENABLE_DEPRECATED_SINCE(6, 6)

#include <QString>

namespace LayerShellQt
{
/**
 * Sets the right environment so the shells created from now on use wlr-layer-shell.
 */
class LAYERSHELLQT_EXPORT Shell
{
public:
    LAYERSHELLQTINTERFACE_DEPRECATED_VERSION(6, 6, "Calling useLayerShell is not needed since Qt 6.5")
    static void useLayerShell();
};

}
#endif

#endif
