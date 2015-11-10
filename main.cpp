#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QWindow>
#include <QFile>

#include <ircclient.h>

QFile logFile("derpy.log");

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
 {
    QString logMessage;
    switch (type) {
    case QtDebugMsg:
        logMessage = QStringLiteral("Debug: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    case QtInfoMsg:
        logMessage = QStringLiteral("Info: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    case QtWarningMsg:
        logMessage = QStringLiteral("Warning: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    case QtCriticalMsg:
        logMessage = QStringLiteral("Critical: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    case QtFatalMsg:
        logMessage = QStringLiteral("Fatal: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        logFile.write(logMessage.toUtf8());
        logFile.flush();
        abort();
    }
    logFile.write(logMessage.toUtf8());
    logFile.flush();
 }

int main(int argc, char *argv[])
{
    logFile.open(QIODevice::WriteOnly | QIODevice:: Truncate);
    qInstallMessageHandler(myMessageOutput);
    QGuiApplication app(argc, argv);

    qmlRegisterType<IrcClient>("IrcClient", 1, 0, "IrcClient");
    QQmlApplicationEngine engine;
//    engine.addImportPath("qrc:/plugins/qml"); // Why doesn't it work with dynamic build?
    engine.load(QUrl(QStringLiteral("qrc:/qml/welcome.qml")));

//    if(engine.rootObjects().first()->isWindowType()) {
//        QWindow *window = qobject_cast<QWindow *>( engine.rootObjects().first() );
//        window->setFlags(Qt::FramelessWindowHint);
//    }

    return app.exec();
}

