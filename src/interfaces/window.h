/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef LAYERSHELLQTWINDOW_H
#define LAYERSHELLQTWINDOW_H

#include <QObject>
#include <QWindow>

#include "layershellqt_export.h"

namespace LayerShellQt
{
class WindowPrivate;

class LAYERSHELLQT_EXPORT Window : public QObject
{
    Q_OBJECT
public:
    ~Window() override;

    enum Anchor {
        AnchorTop = 1, // the top edge of the anchor rectangle
        AnchorBottom = 2, // the bottom edge of the anchor rectangle
        AnchorLeft = 4, // the left edge of the anchor rectangle
        AnchorRight = 8, // the right edge of the anchor rectangle
    };
    Q_ENUM(Anchor);
    Q_DECLARE_FLAGS(Anchors, Anchor)

    /**
     * This enum type is used to specify the layer where a surface can be put in.
     */
    enum Layer {
        LayerBackground = 0,
        LayerBottom = 1,
        LayerTop = 2,
        LayerOverlay = 3,
    };
    Q_ENUM(Layer)

    void setAnchor(Anchor anchor);
    void setExclusiveZone(int32_t zone);
    void setMargins(const QMargins &margins);
    void setKeyboardInteractivity(bool enabled);
    void setLayer(Layer layer);

    static Window *get(QWindow *window);

private:
    Window(WindowPrivate *d);
    QScopedPointer<WindowPrivate> d;
};

}

#endif
