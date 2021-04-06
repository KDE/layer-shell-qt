/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <interfaces/shell.h>
#include <interfaces/window.h>

int main(int argc, char **argv)
{
    LayerShellQt::Shell::useLayerShell();

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.loadData(
        "import QtQuick.Controls 2.10\n"
        "import QtQuick 2.10\n"
        "\n"
        "ApplicationWindow {"
        "   width: 100; height: 100\n"
        "   visible: true\n"
        "   Rectangle { color: 'red'; anchors.fill: parent }"
        "}"

        ,
        QStringLiteral("bananaland:/potato.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [](QObject *object) {
        auto layerWindow = LayerShellQt::Window::get(qobject_cast<QWindow *>(object));
        Q_ASSERT(layerWindow);
        layerWindow->setMargins({50, 50, 50, 50});
    });

    return app.exec();
}
