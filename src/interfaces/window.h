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

#if QT_VERSION < QT_VERSION_CHECK(6, 6, 0)
struct xdg_popup;
#endif

namespace LayerShellQt
{
class WindowPrivate;

class LAYERSHELLQT_EXPORT Window : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Anchors anchors READ anchors WRITE setAnchors NOTIFY anchorsChanged)
    Q_PROPERTY(QString scope READ scope WRITE setScope)
    Q_PROPERTY(QMargins margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(qint32 exclusionZone READ exclusionZone WRITE setExclusiveZone NOTIFY exclusionZoneChanged)
    Q_PROPERTY(Layer layer READ layer WRITE setLayer NOTIFY layerChanged)
    Q_PROPERTY(KeyboardInteractivity keyboardInteractivity READ keyboardInteractivity WRITE setKeyboardInteractivity NOTIFY keyboardInteractivityChanged)
    Q_PROPERTY(ScreenConfiguration screenConfiguration READ screenConfiguration WRITE setScreenConfiguration)

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

    /**
     * This enum type is used to specify which screen to place the surface on.
     * ScreenFromQWindow (the default) reads QWindow::screen() while ScreenFromCompositor
     * passes nil and lets the compositor decide.
     */
    enum ScreenConfiguration {
        ScreenFromQWindow = 0,
        ScreenFromCompositor = 1,
    };
    Q_ENUM(ScreenConfiguration)

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

    void setScreenConfiguration(ScreenConfiguration screenConfiguration);
    ScreenConfiguration screenConfiguration() const;

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
     * Whether the QWindow should be closed when the layer surface is dismissed by the compositor.
     * For example, if the associated screen has been removed.
     *
     * This can be used to map the window on another screen.
     */
    void setCloseOnDismissed(bool close);
    bool closeOnDismissed() const;

    /**
     * Sets the desired window size. This size indicates how much space the compositor is going
     * to reserve for the window.
     */
    void setDesiredSize(const QSize &size);
    QSize desiredSize() const;

    /**
     * Gets the LayerShell Window for a given Qt Window
     * Ownership is not transferred
     */
    static Window *get(QWindow *window);

#if QT_VERSION < QT_VERSION_CHECK(6, 6, 0)
    static void attachPopup(QWindow *window, xdg_popup *popup);
#endif

    static Window *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void anchorsChanged();
    void exclusionZoneChanged();
    void marginsChanged();
    void keyboardInteractivityChanged();
    void layerChanged();
    void desiredSizeChanged();

private:
    Window(QWindow *window);
    QScopedPointer<WindowPrivate> d;
};

}

#endif
