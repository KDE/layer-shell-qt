/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleix.pol_gonzalez@mercedes-benz.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

import QtQuick 2.15
import org.kde.layershell 1.0 as LayerShell

Item
{
    Text {
        text: "A normal Window"
        anchors.centerIn: parent
    }

    Window {
        LayerShell.Window.anchors: LayerShell.Window.AnchorLeft
        LayerShell.Window.layer: LayerShell.Window.LayerBackground
        LayerShell.Window.exclusionZone: -1

        width: 200
        height: 150
        Rectangle {
            anchors.fill: parent
            color: "green"

            Text {
                anchors.centerIn: parent
                text: "left bg"
            }
        }
        visible: true
    }


    Window {
        LayerShell.Window.scope: "dock"
        LayerShell.Window.anchors: LayerShell.Window.AnchorLeft
        LayerShell.Window.layer: LayerShell.Window.LayerTop
        LayerShell.Window.exclusionZone: width

        width: 100
        height: 100
        Rectangle {
            anchors.fill: parent
            color: "red"

            Text {
                anchors.centerIn: parent
                text: "left"
            }
        }
        visible: true
    }

    Window {
        LayerShell.Window.scope: "normal"
        LayerShell.Window.anchors: LayerShell.Window.AnchorRight

        width: 100
        height: 100
        Rectangle {
            anchors.fill: parent
            color: "red"

            Text {
                anchors.centerIn: parent
                text: "right"
            }
        }
        visible: true
    }

    Window {
        LayerShell.Window.scope: "normal"
        LayerShell.Window.anchors: LayerShell.Window.AnchorTop

        width: 100
        height: 100
        Rectangle {
            anchors.fill: parent
            color: "red"

            Text {
                anchors.centerIn: parent
                text: "top"
            }
        }
        visible: true
    }

    Window {
        LayerShell.Window.scope: "normal"
        LayerShell.Window.anchors: LayerShell.Window.AnchorBottom

        width: 100
        height: 100
        Rectangle {
            anchors.fill: parent
            color: "red"

            Text {
                anchors.centerIn: parent
                text: "bottom"
            }
        }
        visible: true
    }
}
