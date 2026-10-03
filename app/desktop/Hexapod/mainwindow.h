#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "protocol.h"
#include "hexapod.h"
#include "radioclient.h"
#include "aggregatorclient.h"
#include <QQueue>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showApp();
    void hideApp();

private:

    bool serialWriteChunked(const QByteArray &pkt, int chunkSize, int gapMs);
    AggregatorClient aggregatorClient;
    bool controlEnabled = false;

    QTimer radioTxTimer;
    int radioTxDelayMs = 150;
    bool radioTxActive = false;

    bool ledSceneSending = false;
    RadioClient radioClient;

    Hexapod *hexapod_pg;

    uint8_t currentMove = 0u;
    uint8_t lastHexapodMoveSent = 0u;
    LegAngles_t hexapod_angles[6];

    QTimer hexapodMoveTimer;
    void sendHexapodMove();

protected:
    void logLine(const QString &text);
    void radioHandleParsedPacket(uint8_t id, uint8_t cmd, const QByteArray &payload);
    void handleHexapodState(const QByteArray &data);
};
#endif // MAINWINDOW_H
