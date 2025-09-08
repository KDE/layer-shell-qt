/*
 *   SPDX-FileCopyrightText: 2025 Aleix Pol i Gonzalez <aleixpol@kde.org>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import org.kde.layershell 1.0 as LayerShell

Item
{
    Button {
        text: "Convert"
        anchors.centerIn: parent

        onClicked: {
            win.close()
            win.LayerShell.Window.anchors = LayerShell.Window.AnchorLeft;
            win.LayerShell.Window.layer = LayerShell.Window.LayerTop;
            win.LayerShell.Window.exclusionZone = -1;
            win.show()
        }
    }

    Window {
        id: win

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
}
