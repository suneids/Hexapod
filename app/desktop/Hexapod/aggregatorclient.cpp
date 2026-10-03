#include "aggregatorclient.h"

#include <QJsonDocument>
#include <QJsonObject>

AggregatorClient::AggregatorClient(
    const QString &appName,
    QObject *parent
    )
    : QObject(parent),
    appName(appName)
{
    reconnectTimer.setInterval(1000);

    connect(
        &reconnectTimer,
        &QTimer::timeout,
        this,
        &AggregatorClient::connectToAggregator
        );

    connect(
        &socket,
        &QLocalSocket::connected,
        this,
        [this]()
        {
            reconnectTimer.stop();
            sendRegister();
        }
        );

    connect(
        &socket,
        &QLocalSocket::disconnected,
        this,
        [this]()
        {
            if(!reconnectTimer.isActive())
                reconnectTimer.start();
        }
        );

    connect(
        &socket,
        &QLocalSocket::readyRead,
        this,
        &AggregatorClient::onReadyRead
        );
}


void AggregatorClient::connectToAggregator()
{
    if(socket.state() != QLocalSocket::UnconnectedState)
        return;

    socket.connectToServer("aggregator");

    if(!reconnectTimer.isActive())
        reconnectTimer.start();
}


void AggregatorClient::sendRegister()
{
    QJsonObject obj;

    obj["type"] = "register";
    obj["app"] = appName;

    QByteArray data =
        QJsonDocument(obj).toJson(QJsonDocument::Compact);

    data += '\n';

    socket.write(data);
}


void AggregatorClient::onReadyRead()
{
    rxBuffer += socket.readAll();

    while(true)
    {
        int pos = rxBuffer.indexOf('\n');

        if(pos < 0)
            return;

        QByteArray raw = rxBuffer.left(pos);
        rxBuffer.remove(0, pos + 1);

        QJsonDocument doc =
            QJsonDocument::fromJson(raw);

        if(!doc.isObject())
            continue;

        QJsonObject obj = doc.object();

        QString type   = obj["type"].toString();
        QString target = obj["app"].toString();

        // Команда либо конкретно этому приложению,
        // либо broadcast для всех.
        if(target != appName && target != "*")
            continue;

        if(type == "show")
            emit showRequested();

        else if(type == "hide")
            emit hideRequested();
    }
}
