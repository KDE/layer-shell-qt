/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef LAYERSHELLQTWINDOW_H
#define LAYERSHELLQTWINDOW_H

#include <QObject>
#include <QScreen>
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
        AnchorTop = 1, ///< The top edge of the anchor rectangle
        AnchorBottom = 2, ///< The bottom edge of the anchor rectangle
        AnchorLeft = 4, ///< The left edge of the anchor rectangle
        AnchorRight = 8, ///< The right edge of the anchor rectangle
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

    /**
     * This enum type is used to specify how the layer surface handles keyboard focus.
     */
    enum KeyboardInteractivity {
        KeyboardInteractivityNone = 0,
        KeyboardInteractivityExclusive = 1,
        KeyboardInteractivityOnDemand = 2,
    };
    Q_ENUM(KeyboardInteractivity)

    void setAnchors(Anchors anchor);
    Anchors anchors() const;

    void setExclusiveZone(int32_t zone);
    int32_t exclusionZone() const;

    void setMargins(const QMargins &margins);
    QMargins margins() const;

    void setKeyboardInteractivity(KeyboardInteractivity interactivity);
    KeyboardInteractivity keyboardInteractivity() const;

    void setLayer(Layer layer);
    Layer layer() const;

    /**
     * If set, the compositor will try to put the window on the given screen.
     * If its not set, then the compositor will decide where to put the window.
     * Under normal circumstances, this should be the active output.
     */
    void setDesiredOutput(QScreen *output);
    QScreen *desiredOutput() const;

    /**
     * Sets a string based identifier for this window.
     * This may be used by a compositor to determine stacking
     * order within a given layer.
     *
     * May also be referred to as a role
     */
    void setScope(const QString &scope);
    QString scope() const;

    /**
     * Gets the LayerShell Window for a given Qt Window
     * Ownership is not transferred
     */
    static Window *get(QWindow *window);

private:
    Window(QWindow *window);
    QScopedPointer<WindowPrivate> d;
};

}

#endif
