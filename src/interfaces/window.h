/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    Q_PROPERTY(Anchors anchors READ anchors WRITE setAnchors NOTIFY anchorsChanged)
    Q_PROPERTY(QString scope READ scope WRITE setScope)
    Q_PROPERTY(QMargins margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(qint32 exclusionZone READ exclusionZone WRITE setExclusiveZone NOTIFY exclusionZoneChanged)
    Q_PROPERTY(Layer layer READ layer WRITE setLayer NOTIFY layerChanged)
    Q_PROPERTY(KeyboardInteractivity keyboardInteractivity READ keyboardInteractivity WRITE setKeyboardInteractivity NOTIFY keyboardInteractivityChanged)
    Q_PROPERTY(bool activateOnShow READ activateOnShow WRITE setActivateOnShow)
    Q_PROPERTY(bool wantsToBeOnActiveScreen READ wantsToBeOnActiveScreen WRITE setWantsToBeOnActiveScreen NOTIFY wantsToBeOnActiveScreenChanged)
    Q_PROPERTY(QScreen *screen READ screen WRITE setScreen NOTIFY screenChanged)
#if LAYERSHELLQTINTERFACE_ENABLE_DEPRECATED_SINCE(6, 6)
    Q_PROPERTY(ScreenConfiguration screenConfiguration READ screenConfiguration WRITE setScreenConfiguration)
#endif

public:
    ~Window() override;

    enum Anchor {
        AnchorNone = 0,
        AnchorTop = 1, ///< The top edge of the anchor rectangle
        AnchorBottom = 2, ///< The bottom edge of the anchor rectangle
        AnchorLeft = 4, ///< The left edge of the anchor rectangle
        AnchorRight = 8, ///< The right edge of the anchor rectangle
    };
    Q_DECLARE_FLAGS(Anchors, Anchor)
    Q_FLAG(Anchors)

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

    void setExclusiveEdge(Window::Anchor edge);
    Window::Anchor exclusiveEdge() const;

    void setMargins(const QMargins &margins);
    QMargins margins() const;

    void setDesiredSize(const QSize &size);
    QSize desiredSize() const;

    void setKeyboardInteractivity(KeyboardInteractivity interactivity);
    KeyboardInteractivity keyboardInteractivity() const;

    void setLayer(Layer layer);
    Layer layer() const;

#if LAYERSHELLQTINTERFACE_ENABLE_DEPRECATED_SINCE(6, 6)
    /**
     * This enum type is used to specify which screen to place the surface on.
     * ScreenFromQWindow (the default) reads QWindow::screen() while ScreenFromCompositor
     * passes nil and lets the compositor decide.
     */
    LAYERSHELLQTINTERFACE_DEPRECATED_VERSION(6, 6, "Use wantsToBeOnActiveScreen and screen instead")
    enum ScreenConfiguration {
        ScreenFromQWindow = 0,
        ScreenFromCompositor = 1,
    };
    Q_ENUM(ScreenConfiguration)

    LAYERSHELLQTINTERFACE_DEPRECATED_VERSION(6, 6, "Use wantsToBeOnActiveScreen and screen instead")
    void setScreenConfiguration(ScreenConfiguration screenConfiguration);
    LAYERSHELLQTINTERFACE_DEPRECATED_VERSION(6, 6, "Use wantsToBeOnActiveScreen and screen instead")
    ScreenConfiguration screenConfiguration() const;
#endif

    /**
     * Indicates whether the layer shell surface should be placed on the active screen based on @a set.
     *
     * The active screen depends on the compositor policies.
     *
     * If the screen() is @c null and the wantsToBeOnActiveScreen() is @c false, then the
     * QWindow::screen() will be used to decide what screen the layer shell surface should be placed on.
     *
     * The screen() will be reset if @a set is @c true.
     */
    void setWantsToBeOnActiveScreen(bool set);
    bool wantsToBeOnActiveScreen() const;

    /**
     * Indicates that the layer shell surface should be placed on the specified @a screen.
     *
     * If the screen() is @c null and the wantsToBeOnActiveScreen() is @c false, then the
     * QWindow::screen() will be used to decide what screen the layer shell surface should be placed on.
     *
     * The wantsToBeOnActiveScreen() will be reset to @c false after calling this function.
     */
    void setScreen(QScreen *screen);
    QScreen *screen() const;

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
     * Whether the window should requestActivate on show.
     *
     * Normally, you want this enabled but in case of e.g. a desktop window, this can be disabled.
     *
     * It does nothing when KeyboardInteractivity is KeyboardInteractivityNone.
     *
     * The default is true.
     */
    void setActivateOnShow(bool activateOnShow);
    bool activateOnShow() const;

    /**
     * Gets the LayerShell Window for a given Qt Window
     * Ownership is not transferred
     */
    static Window *get(QWindow *window);

    static Window *qmlAttachedProperties(QObject *object);

    bool eventFilter(QObject *watched, QEvent *event) override;

Q_SIGNALS:
    void anchorsChanged();
    void exclusionZoneChanged();
    void exclusiveEdgeChanged();
    void marginsChanged();
    void desiredSizeChanged();
    void keyboardInteractivityChanged();
    void layerChanged();
    void wantsToBeOnActiveScreenChanged();
    void screenChanged();

private:
    void initializeShell();

    Window(QWindow *window);
    QScopedPointer<WindowPrivate> d;
};

}

#endif
