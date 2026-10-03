#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->m_receiver = new RtpJpegReceiver(5600, ui->label);
    connect(m_receiver, &RtpJpegReceiver::frameReady,
            this, &MainWindow::onFrameReady);

    connect(m_receiver, &RtpJpegReceiver::statsMessage,
            this, &MainWindow::onStats);

    ui->label->setAlignment(Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFrameReady(const QImage &img){
    m_lastFrame = img;
    QPixmap px = QPixmap::fromImage(img);
    ui->label->setPixmap(px.scaled(ui->label->size(),
                                   Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation));
}

void MainWindow::onStats(const QString &text)
{
    //qDebug() << text;
}
