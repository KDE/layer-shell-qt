/*
 *   SPDX-FileCopyrightText: 2021 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <QCommandLineParser>

#include <QGuiApplication>
#include <QPainter>
#include <QRasterWindow>
#include <QTimer>
#include <QWindow>

#include <QMetaEnum>

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
    const auto splitted = str.split(QLatin1Char('|'), Qt::SkipEmptyParts);
    for (const auto &value : splitted) {
        ret |= T(metaEnum.keyToValue(qPrintable(value)));
    }
    return ret;
}

class BasicWindow : public QRasterWindow
{
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.fillRect(QRect(0, 0, width(), height()), Qt::red);
    }
};

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    const auto layerMetaEnum = QMetaEnum::fromType<Window::Layer>();
    const auto anchorMetaEnum = QMetaEnum::fromType<Window::Anchors>();

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
    QCommandLineOption widthOption(QStringLiteral("width"), QStringLiteral("Width of the window"), QStringLiteral("pixels"), QStringLiteral("0"));
    QCommandLineOption heightOption(QStringLiteral("height"), QStringLiteral("Height of the window"), QStringLiteral("pixels"), QStringLiteral("0"));

    parser.addOptions({marginsOption, scopeOption, anchorsOption, layerOption, widthOption, heightOption});
    parser.addHelpOption();
    parser.process(app);

    BasicWindow window;

    LayerShellQt::Window *layerShell = LayerShellQt::Window::get(&window);
    layerShell->setLayer(Window::LayerBottom);

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
        layerShell->setAnchors(stringToEnum<Window::Anchors>(anchorMetaEnum, parser.value(anchorsOption)));
    }
    if (parser.isSet(widthOption)) {
        window.setWidth(parser.value(widthOption).toInt());
    }
    if (parser.isSet(heightOption)) {
        window.setHeight(parser.value(heightOption).toInt());
    }

    window.show();

    BasicWindow window2;
    window2.resize(400, 400);
    window2.show();

    // just so you don't block yourself out whilst testing
    QTimer::singleShot(5000, &app, &QGuiApplication::quit);
    return app.exec();
}
