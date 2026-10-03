#include "radioclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

RadioClient::RadioClient(QObject *parent)
    : QObject(parent)
{
    reconnectTimer.setInterval(1000);

    connect(
        &reconnectTimer,
        &QTimer::timeout,
        this,
        &RadioClient::connectToDaemon
        );

    connect(
        &socket,
        &QLocalSocket::readyRead,
        this,
        &RadioClient::onReadyRead
        );

    connect(
        &socket,
        &QLocalSocket::connected,
        this,
        [this]()
        {
            reconnectTimer.stop();
            qDebug() << "Radio daemon connected";
        }
        );

    connect(
        &socket,
        &QLocalSocket::disconnected,
        this,
        [this]()
        {
            qDebug() << "Radio daemon disconnected";

            if(!reconnectTimer.isActive())
                reconnectTimer.start();
        }
        );

    connect(
        &socket,
        &QLocalSocket::errorOccurred,
        this,
        [this](QLocalSocket::LocalSocketError)
        {
            if(socket.state() == QLocalSocket::UnconnectedState &&
                !reconnectTimer.isActive())
            {
                reconnectTimer.start();
            }
        }
        );
}


void RadioClient::connectToDaemon()
{
    if(socket.state() != QLocalSocket::UnconnectedState)
        return;

    socket.connectToServer("radio-daemon");

    if(!reconnectTimer.isActive())
        reconnectTimer.start();
}


bool RadioClient::isConnected() const
{
    return socket.state() == QLocalSocket::ConnectedState;
}


void RadioClient::send(
    const QByteArray &packet,
    const QString &name,
    int afterDelayMs
    )
{
    QJsonObject obj;

    obj["type"] = "send";

    obj["packet"] =
        QString::fromLatin1(
            packet.toBase64()
            );

    obj["name"] = name;
    obj["afterDelayMs"] = afterDelayMs;

    sendJson(obj);
}


void RadioClient::sendScene(
    const QList<QByteArray> &packets,
    int afterDelayMs
    )
{
    QJsonArray array;

    for(const QByteArray &packet : packets)
    {
        array.append(
            QString::fromLatin1(
                packet.toBase64()
                )
            );
    }

    QJsonObject obj;

    obj["type"] = "sendScene";
    obj["packets"] = array;
    obj["afterDelayMs"] = afterDelayMs;

    sendJson(obj);
}


void RadioClient::sendJson(const QJsonObject &obj)
{
    if(!isConnected())
        return;

    QByteArray data =
        QJsonDocument(obj).toJson(
            QJsonDocument::Compact
            );

    data += '\n';

    socket.write(data);
}


void RadioClient::request(
    const QByteArray &packet,
    const QString &name,
    quint8 expectId,
    quint8 expectCmd,
    int timeoutMs,
    int afterDelayMs,
    bool isPoll
    )
{
    QJsonObject obj;

    obj["type"] = "request";
    obj["packet"] =
        QString::fromLatin1(packet.toBase64());

    obj["name"] = name;
    obj["expectId"] = static_cast<int>(expectId);
    obj["expectCmd"] = static_cast<int>(expectCmd);
    obj["timeoutMs"] = timeoutMs;
    obj["afterDelayMs"] = afterDelayMs;
    obj["isPoll"] = isPoll;

    sendJson(obj);
}


void RadioClient::sendLatest(
    const QString &key,
    const QByteArray &packet,
    const QString &name,
    int afterDelayMs
    )
{
    QJsonObject obj;

    obj["type"] = "sendLatest";
    obj["key"] = key;

    obj["packet"] =
        QString::fromLatin1(packet.toBase64());

    obj["name"] = name;
    obj["afterDelayMs"] = afterDelayMs;

    sendJson(obj);
}


void RadioClient::onReadyRead()
{
    rxBuffer += socket.readAll();

    while(true)
    {
        int pos = rxBuffer.indexOf('\n');

        if(pos < 0)
            return;

        QByteArray raw =
            rxBuffer.left(pos);

        rxBuffer.remove(0, pos + 1);


        QJsonParseError error;

        QJsonDocument doc =
            QJsonDocument::fromJson(
                raw,
                &error
                );

        if(error.error != QJsonParseError::NoError)
            continue;


        QJsonObject obj = doc.object();

        QString type =
            obj["type"].toString();


        if(type == "rx")
        {
            quint8 id =
                static_cast<quint8>(
                    obj["id"].toInt()
                    );

            quint8 cmd =
                static_cast<quint8>(
                    obj["cmd"].toInt()
                    );

            QByteArray payload =
                QByteArray::fromBase64(
                    obj["payload"]
                        .toString()
                        .toLatin1()
                    );

            emit packetReceived(
                id,
                cmd,
                payload
                );
        }
        else if(type == "sceneFinished")
        {
            emit sceneFinished();
        }
    }
}
