#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>

#include <inputsignal.h>
#include <kissfft.hh>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <signalprocess.h>
#include <chronogram.h>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "aTGsbm_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    InputSignal source_file;
    SignalProcess signal_process;

    QObject::connect(&source_file, &InputSignal::buffer, &signal_process, &SignalProcess::input);

    QQmlApplicationEngine engine;
    qmlRegisterType<Chronogram>("CustomItems", 1, 0, "Chronogram");
    qmlRegisterType<SignalProcess>("CustomItems",1,0,"SignalProcess");
    engine.rootContext()->setContextProperty("signalprocess", &signal_process);



    const QUrl url(QStringLiteral("qrc:/aTGsbm/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);
    signal_process.setSampleRate(48000);
    source_file.start();
    return app.exec();
}
