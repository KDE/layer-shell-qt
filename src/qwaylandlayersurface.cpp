/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *   SPDX-FileCopyrightText: 2018 Drew DeVault <sir@cmpwn.com>
 *
 *   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "interfaces/window.h"
#include "layershellqt_logging.h"
#include "qwaylandlayersurface_p.h"
#include "qwaylandxdgactivationv1_p.h"

#include <QGuiApplication>
#include <QScreen>
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformwindow_p.h> // private native interface

namespace LayerShellQt
{
QWaylandLayerSurface::QWaylandLayerSurface(QWaylandLayerShellIntegration *shell, QtWaylandClient::QWaylandWindow *window)
    : QtWaylandClient::QWaylandShellSurface(window)
    , QtWayland::zwlr_layer_surface_v1()
    , m_shell(shell)
    // FIXME reverse lookup would a QWindow getter make sense, also below a bunch
    // QWindow * QWaylandShellSurface::window
    , m_interface(Window::get(platformWindow()->window()))
    , m_window(window)
{
    wl_output *output = nullptr;
    if (!m_interface->wantsToBeOnActiveScreen()) {
        QScreen *desiredScreen = m_interface->screen();
        if (!desiredScreen) {
            desiredScreen = platformWindow()->window()->screen();
        }

        auto waylandScreen = platformWindow()->window()->screen()->nativeInterface<QNativeInterface::QWaylandScreen>();
        // Qt will always assign a screen to a window, but if the compositor has no screens available a dummy QScreen object is created
        // this will not cast to a QWaylandScreen
        if (!waylandScreen) {
            qCWarning(LAYERSHELLQT) << "Creating a layer shell for placeholder screen. This will be positioned incorrectly";
        } else {
            output = waylandScreen->output();
        }
    }
    init(shell->get_layer_surface(wlSurface(), output, m_interface->layer(), m_interface->scope()));
    connect(m_interface, &Window::layerChanged, this, [this]() {
        setLayer(m_interface->layer());
    });

    setAnchor(m_interface->anchors());
    connect(m_interface, &Window::anchorsChanged, this, [this]() {
        setAnchor(m_interface->anchors());
        if (m_interface->desiredSize().isNull()) {
            setDesiredSize(m_lastContentGeometry.size());
        } else {
            setDesiredSize(m_interface->desiredSize());
        }
    });

    setExclusiveZone(m_interface->exclusionZone());
    connect(m_interface, &Window::exclusionZoneChanged, this, [this]() {
        setExclusiveZone(m_interface->exclusionZone());
    });
    setExclusiveEdge(m_interface->exclusiveEdge());
    connect(m_interface, &Window::exclusiveEdgeChanged, this, [this]() {
        setExclusiveEdge(m_interface->exclusiveEdge());
    });

    setMargins(m_interface->margins());
    connect(m_interface, &Window::marginsChanged, this, [this]() {
        setMargins(m_interface->margins());
    });

    connect(m_interface, &Window::desiredSizeChanged, this, [this]() {
        if (!m_interface->desiredSize().isNull()) {
            setDesiredSize(m_interface->desiredSize());
        }
    });

    setKeyboardInteractivity(m_interface->keyboardInteractivity());
    connect(m_interface, &Window::keyboardInteractivityChanged, this, [this]() {
        setKeyboardInteractivity(m_interface->keyboardInteractivity());
    });

    if (m_interface->desiredSize().isNull()) {
        // FIXME QWaylandWindow should call setWindowContentGeometry when creating the shell surface
        // / Does a setWindowContentGeometry call arrive early enough?
        // setDesiredSize(window->windowContentGeometry().size());
    } else {
        setDesiredSize(m_interface->desiredSize());
    }
}

void QWaylandLayerSurface::setContentGeometry(const QRect &rect)
{
    m_lastContentGeometry = rect;
    if (m_interface->desiredSize().isNull()) {
        setDesiredSize(rect.size());
    }
}

QWaylandLayerSurface::~QWaylandLayerSurface()
{
    destroy();
}

void QWaylandLayerSurface::zwlr_layer_surface_v1_closed()
{
    if (m_interface->closeOnDismissed()) {
        platformWindow()->close();
    }
}

void QWaylandLayerSurface::zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height)
{
    ack_configure(serial);
    m_pendingSize = QSize(width, height);

    if (!m_configured) {
        m_configured = true;
        applyConfigure();
        sendExpose();
    } else {
        // Later configures are resizes, so we have to queue them up for a time when we
        // are not painting to the window.
        applyConfigureWhenPossible();
    }
}

void QWaylandLayerSurface::attachPopup(QtWaylandClient::QWaylandShellSurface *popup)
{
    std::any anyRole = popup->surfaceRole();

    if (auto role = std::any_cast<::xdg_popup *>(&anyRole)) {
        get_popup(*role);
    } else {
        qCWarning(LAYERSHELLQT) << "Cannot attach popup of unknown type";
    }
}

void QWaylandLayerSurface::applyConfigure()
{
    resizeFromApplyConfigure(m_pendingSize);
}

void QWaylandLayerSurface::setDesiredSize(const QSize &size)
{
    const bool horizontallyConstrained = m_interface->anchors().testFlags({Window::AnchorLeft, Window::AnchorRight});
    const bool verticallyConstrained = m_interface->anchors().testFlags({Window::AnchorTop, Window::AnchorBottom});

    QSize effectiveSize = size;
    if (horizontallyConstrained) {
        effectiveSize.setWidth(0);
    }
    if (verticallyConstrained) {
        effectiveSize.setHeight(0);
    }
    set_size(effectiveSize.width(), effectiveSize.height());
}

void QWaylandLayerSurface::setAnchor(uint anchor)
{
    set_anchor(anchor);
}

void QWaylandLayerSurface::setExclusiveZone(int32_t zone)
{
    set_exclusive_zone(zone);
}

void QWaylandLayerSurface::setExclusiveEdge(uint32_t edge)
{
    if (zwlr_layer_surface_v1_get_version(object()) >= ZWLR_LAYER_SURFACE_V1_SET_EXCLUSIVE_EDGE_SINCE_VERSION) {
        set_exclusive_edge(edge);
    }
}

void QWaylandLayerSurface::setMargins(const QMargins &margins)
{
    set_margin(margins.top(), margins.right(), margins.bottom(), margins.left());
}

void QWaylandLayerSurface::setKeyboardInteractivity(uint32_t interactivity)
{
    set_keyboard_interactivity(interactivity);
}

void QWaylandLayerSurface::setLayer(uint32_t layer)
{
    if (zwlr_layer_surface_v1_get_version(object()) >= ZWLR_LAYER_SURFACE_V1_SET_LAYER_SINCE_VERSION)
        set_layer(layer);
}

void QWaylandLayerSurface::setWindowSize(const QSize &size)
{
    if (m_interface->desiredSize().isNull()) {
        setDesiredSize(size);
    }
}

bool QWaylandLayerSurface::requestActivate()
{
    QWaylandXdgActivationV1 *activation = m_shell->activation();
    if (!activation->isActive()) {
        return false;
    }
    if (!m_activationToken.isEmpty()) {
        activation->activate(m_activationToken, wlSurface());
        m_activationToken = {};
        return true;
    } else if (const auto token = qEnvironmentVariable("XDG_ACTIVATION_TOKEN"); !token.isEmpty()) {
        activation->activate(token, wlSurface());
        qunsetenv("XDG_ACTIVATION_TOKEN");
        return true;
    } else {
        const auto focusWindow = QGuiApplication::focusWindow();
        const auto waylandApp = qGuiApp->nativeInterface<QNativeInterface::QWaylandApplication>();
        const auto surface = focusWindow ? focusWindow->nativeInterface<QNativeInterface::Private::QWaylandWindow>()->surface() : wlSurface();
        if (const auto seat = waylandApp->lastInputSeat()) {
            const auto tokenProvider = activation->requestXdgActivationToken(seat, surface, waylandApp->lastInputSerial(), QString());
            connect(tokenProvider, &QWaylandXdgActivationTokenV1::done, this, [this](const QString &token) {
                m_shell->activation()->activate(token, wlSurface());
            });
            connect(tokenProvider, &QWaylandXdgActivationTokenV1::done, tokenProvider, &QObject::deleteLater);
            return true;
        }
    }
    return false;
}

bool QWaylandLayerSurface::requestActivateOnShow()
{
    if (!m_interface->activateOnShow()) {
        return false;
    }

    if (m_interface->keyboardInteractivity() == Window::KeyboardInteractivityNone) {
        return false;
    }

    if (platformWindow()->window()->property("_q_showWithoutActivating").toBool()) {
        return false;
    }

    return requestActivate();
}

void QWaylandLayerSurface::setXdgActivationToken(const QString &token)
{
    m_activationToken = token;
}

void QWaylandLayerSurface::requestXdgActivationToken(quint32 serial)
{
    QWaylandXdgActivationV1 *activation = m_shell->activation();
    if (!activation->isActive()) {
        return;
    }
    const auto waylandApp = qGuiApp->nativeInterface<QNativeInterface::QWaylandApplication>();
    auto tokenProvider = activation->requestXdgActivationToken(waylandApp->lastInputSeat(), wlSurface(), serial, QString());

    connect(tokenProvider, &QWaylandXdgActivationTokenV1::done, this, [this](const QString &token) {
        // FIXME  No way to do this without knowing QWaylandWindow because QPlatofrmWindow is not QObject
        // But we wanted to make a better API as well or alternatively the signal needs to be on QWaylandShellSurface as well
        // Q_EMIT window()->xdgActivationTokenCreated(token);
        // QMetaObject::invokeMethod(window(), "xdgActivationTokenCreated", token);
    });
    connect(tokenProvider, &QWaylandXdgActivationTokenV1::done, tokenProvider, &QObject::deleteLater);
}

void QWaylandLayerSurface::sendExpose()
{
    // FIXME No way to call this, needs wrapper in QWaylandShellSurface
    // window()->updateExposure();
}
}
