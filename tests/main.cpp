/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <interfaces/shell.h>
#include <interfaces/window.h>

using namespace LayerShellQt;

QStringList enumsToStringList(QMetaEnum metaEnum)
{
    QStringList ret;
    ret.reserve(metaEnum.keyCount());
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        ret.append(metaEnum.key(i));
    }
    return ret;
}

template<typename T>
T stringToEnum(QMetaEnum metaEnum, const QString &str)
{
    T ret = {};
    const auto splitted = str.split(QLatin1Char('|'));
    for (const auto &value : splitted) {
        ret |= T(metaEnum.keyToValue(qPrintable(value)));
    }
    return ret;
}

int main(int argc, char **argv)
{
    Shell::useLayerShell();

    QGuiApplication app(argc, argv);

    const auto layerMetaEnum = QMetaEnum::fromType<Window::Layer>();
    const auto anchorMetaEnum = QMetaEnum::fromType<Window::Anchor>();

    QCommandLineParser parser;
    QCommandLineOption marginsOption(QStringLiteral("margins"), QStringLiteral("Window margins"), QStringLiteral("pixels"), QStringLiteral("0"));
    QCommandLineOption scopeOption(QStringLiteral("scope"), QStringLiteral("Window scope"), QStringLiteral("namespace"), QStringLiteral("normal"));
    QCommandLineOption anchorsOption(QStringLiteral("anchors"),
                                     QStringLiteral("Either ") + enumsToStringList(anchorMetaEnum).join(QLatin1String("|")),
                                     QStringLiteral("anchors"),
                                     QStringLiteral("AnchorTop|AnchorBottom|AnchorLeft|AnchorRight"));
    QCommandLineOption layerOption(QStringLiteral("layer"),
                                   QStringLiteral("One of ") + enumsToStringList(layerMetaEnum).join(QLatin1String("|")),
                                   QStringLiteral("layer"),
                                   QStringLiteral("LayerTop"));
    parser.addOptions({marginsOption, scopeOption, anchorsOption, layerOption});
    parser.addHelpOption();
    parser.process(app);

    static int margins = 0;
    if (parser.isSet(marginsOption)) {
        margins = parser.value(marginsOption).toInt();
        Shell::setDefaultMargins({margins, margins, margins, margins});
    }
    if (parser.isSet(scopeOption)) {
        Shell::setDefaultScope(parser.value(scopeOption));
    }
    if (parser.isSet(layerOption)) {
        Shell::setDefaultLayer(Window::Layer(layerMetaEnum.keyToValue(qPrintable(parser.value(layerOption)))));
    }
    if (parser.isSet(anchorsOption)) {
        Shell::setDefaultAnchors(stringToEnum<Window::Anchors>(anchorMetaEnum, parser.value(anchorsOption)));
    }

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

    return app.exec();
}
