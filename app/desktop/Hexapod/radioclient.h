#ifndef RADIOCLIENT_H
#define RADIOCLIENT_H

#include <QObject>
#include <QLocalSocket>
#include <QByteArray>
#include <QJsonObject>
#include <QList>
#include <QTimer>

class RadioClient : public QObject
{
    Q_OBJECT

public:
    explicit RadioClient(QObject *parent = nullptr);

    void connectToDaemon();

    bool isConnected() const;

    void request(const QByteArray &packet, const QString &name, quint8 expectId, quint8 expectCmd, int timeoutMs, int afterDelayMs, bool isPoll);

    void sendLatest(const QString &key, const QByteArray &packet, const QString &name, int afterDelayMs);

    void send(const QByteArray &packet, const QString &name, int afterDelayMs = 200);

    void sendScene(const QList<QByteArray> &packets, int afterDelayMs = 250);

signals:
    void packetReceived(quint8 id, quint8 cmd, QByteArray payload);
    void sceneFinished();
private slots:
    void onReadyRead();

private:
    QLocalSocket socket;
    QByteArray rxBuffer;
    QTimer reconnectTimer;
    void sendJson(const QJsonObject &obj);
};

#endif
