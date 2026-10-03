#include "mainwindow.h"
#include "protocol.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),aggregatorClient("hexapod", this)
{
    hexapod_pg = new Hexapod(this);
    setCentralWidget(hexapod_pg);
    radioClient.connectToDaemon();

    connect(
        &radioClient,
        &RadioClient::packetReceived,
        this,
        &MainWindow::radioHandleParsedPacket
        );

    aggregatorClient.connectToAggregator();

    connect(
        &aggregatorClient,
        &AggregatorClient::showRequested,
        this,
        &MainWindow::showApp
        );

    connect(
        &aggregatorClient,
        &AggregatorClient::hideRequested,
        this,
        &MainWindow::hideApp
        );

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    connect(hexapod_pg, &Hexapod::moveChanged,
            this, [this](uint8_t move){

                currentMove = move;
        qDebug() << "MAIN MOVE ="
                 << QString::number(currentMove, 16);
                /*
     * Сразу пробуем отправить новое состояние.
     *
     * Например:
     * W press  -> 01
     * A press  -> 05
     * A release -> 01
     * W release -> 00
     */
                sendHexapodMove();
            });
    hexapodMoveTimer.setInterval(150);
    connect(&hexapodMoveTimer, &QTimer::timeout, this, &MainWindow::sendHexapodMove);
    hexapodMoveTimer.start();

}


MainWindow::~MainWindow()
{

}


void MainWindow::radioHandleParsedPacket(uint8_t id, uint8_t cmd, const QByteArray &payload)
{
    logLine(QString("RADIO RX | id=%1 cmd=%2 len=%3 | %4")
                .arg(id, 2, 16, QChar('0'))
                .arg(cmd, 2, 16, QChar('0'))
                .arg(payload.size())
                .arg(QString(payload.toHex(' ').toUpper())));

    // 1. Сначала обработать полезные данные.
    if(id == DEV_HEXAPOD && cmd == CMD_HEXAPOD_STATE){
        handleHexapodState(payload);
        return;
    }

    // 2. Потом закрыть ожидающий request, если это он.
}


void MainWindow::handleHexapodState(const QByteArray &data){
    if(data.size() != 18){
        return;
    }

    for(uint8_t leg = 0; leg < 6u; leg++){
        hexapod_angles[leg].coxa = static_cast<int8_t>(data[leg * 3 + 0]);

        hexapod_angles[leg].femur = static_cast<int8_t>(data[leg * 3 + 1]);

        hexapod_angles[leg].tibia = static_cast<int8_t>(data[leg * 3 + 2]);
    }

    hexapod_pg->updateHexapodModel(hexapod_angles);
}


void MainWindow::logLine(const QString &text){
    // ui->txt_log->appendPlainText(QTime::currentTime().toString("HH:mm:ss") + " " + text);
}



void MainWindow::sendHexapodMove()
{
    /*
     * Если страница гексапода закрыта —
     * вообще не посылаем команды движения.
     */
    if(!controlEnabled)
        return;

    if(!radioClient.isConnected())
        return;

    if(currentMove == 0u &&
        lastHexapodMoveSent == 0u)
    {
        return;
    }


    QByteArray payload;

    payload.append(
        static_cast<char>(currentMove)
        );


    QByteArray pkt =
        makePacket(
            DEV_HEXAPOD,
            CMD_HEXAPOD_MOVE,
            payload
            );


    /*
     * ВАЖНО:
     *
     * движение гексапода нельзя просто складывать
     * в обычную FIFO, иначе daemon может накопить:
     *
     * forward
     * forward
     * left
     * stop
     *
     * и потом честно проиграть устаревшие движения.
     *
     * Поэтому это будет специальная команда:
     * "в очереди должно остаться только последнее
     * состояние управления".
     */
    radioClient.sendLatest(
        "hexapod-move",
        pkt,
        QString("HEXAPOD MOVE %1")
            .arg(currentMove, 2, 16, QChar('0')),
        0
        );


    lastHexapodMoveSent = currentMove;
}

void MainWindow::showApp()
{
    controlEnabled = true;

    showMaximized();
    raise();
    activateWindow();
}


void MainWindow::hideApp()
{
    if(controlEnabled)
    {
        currentMove = 0;
        sendHexapodMove();
    }

    controlEnabled = false;

    hide();
}
