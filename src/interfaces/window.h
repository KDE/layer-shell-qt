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

/*!
 * \class LayerShellQt::Window
 * \inmodule LayerShellQt
 * \inheaderfile LayerShellQt/Window
 *
 * \brief A window.
 */
class LAYERSHELLQT_EXPORT Window : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Anchors anchors READ anchors WRITE setAnchors NOTIFY anchorsChanged)
    /*! \property LayerShellQt::Window::scope */
    Q_PROPERTY(QString scope READ scope WRITE setScope)
    Q_PROPERTY(QMargins margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(qint32 exclusionZone READ exclusionZone WRITE setExclusiveZone NOTIFY exclusionZoneChanged)
    Q_PROPERTY(Layer layer READ layer WRITE setLayer NOTIFY layerChanged)
    Q_PROPERTY(KeyboardInteractivity keyboardInteractivity READ keyboardInteractivity WRITE setKeyboardInteractivity NOTIFY keyboardInteractivityChanged)
    Q_PROPERTY(ScreenConfiguration screenConfiguration READ screenConfiguration WRITE setScreenConfiguration)

public:
    ~Window() override;

    /*!
     * \value AnchorNone
     * \value AnchorTop The top edge of the anchor rectangle
     * \value AnchorBottom The bottom edge of the anchor rectangle
     * \value AnchorLeft The left edge of the anchor rectangle
     * \value AnchorRight The right edge of the anchor rectangle
     */
    enum Anchor {
        AnchorNone = 0,
        AnchorTop = 1,
        AnchorBottom = 2,
        AnchorLeft = 4,
        AnchorRight = 8,
    };
    Q_ENUM(Anchor);
    Q_DECLARE_FLAGS(Anchors, Anchor)

    /*!
     * This enum type is used to specify the layer where a surface can be put in.
     */
    enum Layer {
        LayerBackground = 0,
        LayerBottom = 1,
        LayerTop = 2,
        LayerOverlay = 3,
    };
    Q_ENUM(Layer)

    /*!
     * This enum type is used to specify how the layer surface handles keyboard focus.
     */
    enum KeyboardInteractivity {
        KeyboardInteractivityNone = 0,
        KeyboardInteractivityExclusive = 1,
        KeyboardInteractivityOnDemand = 2,
    };
    Q_ENUM(KeyboardInteractivity)

    /*!
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

    void setExclusiveEdge(Window::Anchor edge);
    Window::Anchor exclusiveEdge() const;

    void setMargins(const QMargins &margins);
    QMargins margins() const;

    void setKeyboardInteractivity(KeyboardInteractivity interactivity);
    KeyboardInteractivity keyboardInteractivity() const;

    void setLayer(Layer layer);
    Layer layer() const;

    void setScreenConfiguration(ScreenConfiguration screenConfiguration);
    ScreenConfiguration screenConfiguration() const;

    /*!
     * Sets a string based identifier for this window.
     * This may be used by a compositor to determine stacking
     * order within a given layer.
     *
     * May also be referred to as a role
     */
    void setScope(const QString &scope);

    /*!
     * A string based identifier for this window.
     * This may be used by a compositor to determine stacking
     * order within a given layer.
     *
     * May also be referred to as a role
     */
    QString scope() const;

    /*!
     * Whether the QWindow should be closed when the layer surface is dismissed by the compositor.
     * For example, if the associated screen has been removed.
     *
     * This can be used to map the window on another screen.
     */
    void setCloseOnDismissed(bool close);
    bool closeOnDismissed() const;

    /*!
     * Gets the LayerShell Window for a given Qt Window
     * Ownership is not transferred
     */
    static Window *get(QWindow *window);

    static Window *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void anchorsChanged();
    void exclusionZoneChanged();
    void exclusiveEdgeChanged();
    void marginsChanged();
    void keyboardInteractivityChanged();
    void layerChanged();

private:
    Window(QWindow *window);
    QScopedPointer<WindowPrivate> d;
};

}

#endif
