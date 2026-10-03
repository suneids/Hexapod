#pragma once

#include <QObject>
#include <QLocalSocket>
#include <QTimer>
#include <QByteArray>

class AggregatorClient : public QObject
{
    Q_OBJECT

public:
    explicit AggregatorClient(
        const QString &appName,
        QObject *parent = nullptr
        );

    void connectToAggregator();

signals:
    void showRequested();
    void hideRequested();

private:
    QString appName;

    QLocalSocket socket;
    QByteArray rxBuffer;
    QTimer reconnectTimer;

    void sendRegister();
    void onReadyRead();
};
