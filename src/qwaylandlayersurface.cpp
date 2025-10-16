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

#include <QtWaylandClient/private/qwaylandscreen_p.h>
#include <QtWaylandClient/private/qwaylandsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

#include <QGuiApplication>

namespace LayerShellQt
{
QWaylandLayerSurface::QWaylandLayerSurface(QWaylandLayerShellIntegration *shell, QtWaylandClient::QWaylandWindow *window)
    : QtWaylandClient::QWaylandShellSurface(window)
    , QtWayland::zwlr_layer_surface_v1()
    , m_shell(shell)
    , m_interface(Window::get(window->window()))
    , m_window(window)
{
    wl_output *output = nullptr;
    if (m_interface->screenConfiguration() == Window::ScreenFromQWindow) {
        auto waylandScreen = dynamic_cast<QtWaylandClient::QWaylandScreen *>(window->window()->screen()->handle());
        // Qt will always assign a screen to a window, but if the compositor has no screens available a dummy QScreen object is created
        // this will not cast to a QWaylandScreen
        if (!waylandScreen) {
            qCWarning(LAYERSHELLQT) << "Creating a layer shell for placeholder screen. This will be positioned incorrectly";
        } else {
            output = waylandScreen->output();
        }
    }
    init(shell->get_layer_surface(window->waylandSurface()->object(), output, m_interface->layer(), m_interface->scope()));
    connect(m_interface, &Window::layerChanged, this, [this]() {
        setLayer(m_interface->layer());
    });

    setAnchor(m_interface->anchors());
    connect(m_interface, &Window::anchorsChanged, this, [this]() {
        setAnchor(m_interface->anchors());
        if (m_interface->desiredSize().isNull()) {
            setDesiredSize(m_window->windowContentGeometry().size());
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
        setDesiredSize(window->windowContentGeometry().size());
    } else {
        setDesiredSize(m_interface->desiredSize());
    }
}

QWaylandLayerSurface::~QWaylandLayerSurface()
{
    destroy();
}

void QWaylandLayerSurface::zwlr_layer_surface_v1_closed()
{
    if (m_interface->closeOnDismissed()) {
        window()->window()->close();
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
        window()->applyConfigureWhenPossible();
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
    window()->resizeFromApplyConfigure(m_pendingSize);
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
        activation->activate(m_activationToken, window()->wlSurface());
        m_activationToken = {};
        return true;
    } else if (const auto token = qEnvironmentVariable("XDG_ACTIVATION_TOKEN"); !token.isEmpty()) {
        activation->activate(token, window()->wlSurface());
        qunsetenv("XDG_ACTIVATION_TOKEN");
        return true;
    } else {
        const auto focusWindow = QGuiApplication::focusWindow();
        const auto wlWindow = focusWindow ? static_cast<QtWaylandClient::QWaylandWindow *>(focusWindow->handle()) : window();
        if (const auto seat = wlWindow->display()->lastInputDevice()) {
            const auto tokenProvider = activation->requestXdgActivationToken(wlWindow->display(), wlWindow->wlSurface(), 0, QString());
            connect(tokenProvider, &QWaylandXdgActivationTokenV1::done, this, [this](const QString &token) {
                m_shell->activation()->activate(token, window()->wlSurface());
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

    if (m_window->window()->property("_q_showWithoutActivating").toBool()) {
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
    auto tokenProvider = activation->requestXdgActivationToken(window()->display(), window()->wlSurface(), serial, QString());

    connect(tokenProvider, &QWaylandXdgActivationTokenV1::done, this, [this](const QString &token) {
        Q_EMIT window()->xdgActivationTokenCreated(token);
    });
    connect(tokenProvider, &QWaylandXdgActivationTokenV1::done, tokenProvider, &QObject::deleteLater);
}

void QWaylandLayerSurface::sendExpose()
{
    window()->updateExposure();
}
}
