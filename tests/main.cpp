/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QCommandLineParser>

#include <QGuiApplication>
#include <QRasterWindow>
#include <QWindow>
#include <QPainter>
#include <QTimer>

#include <QMetaEnum>

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

class BasicWindow : public QRasterWindow
{
    void paintEvent(QPaintEvent *) {
        QPainter p(this);
        p.fillRect(QRect(0,0,width(), height()), Qt::red);
    }
};

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


    BasicWindow window;

    LayerShellQt::Window* layerShell = LayerShellQt::Window::get(&window);
    if (parser.isSet(marginsOption)) {
        int margins = parser.value(marginsOption).toInt();
        layerShell->setMargins({margins, margins, margins, margins});
    }

    if (parser.isSet(scopeOption)) {
        layerShell->setScope(parser.value(scopeOption));
    }
    if (parser.isSet(layerOption)) {
        layerShell->setLayer(Window::Layer(layerMetaEnum.keyToValue(qPrintable(parser.value(layerOption)))));
    }
    if (parser.isSet(anchorsOption)) {
        layerShell->setAnchor(stringToEnum<Window::Anchors>(anchorMetaEnum, parser.value(anchorsOption)));
    }
    window.show();


    // just so you don't block yourself out whilst testing
    QTimer::singleShot(5000, &app, &QGuiApplication::quit);
    return app.exec();
}
