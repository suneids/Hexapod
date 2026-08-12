#ifndef RTPJPEGRECEIVER_H
#define RTPJPEGRECEIVER_H
#include <QObject>
#include <QUdpSocket>
#include <QImage>
#include <QByteArray>

class RtpJpegReceiver : public QObject
{
    Q_OBJECT

public:
    explicit RtpJpegReceiver(quint16 port = 5600, QObject *parent = nullptr);

signals:
    void frameReady(const QImage &image);
    void statsMessage(const QString &text);

private slots:
    void onReadyRead();

private:
    struct FrameAssembly{
        bool active        = false;
        quint32 timestamp  = 0;
        quint16 lastSeq    = 0;
        quint32 nextOffset = 0;

        quint8 jpegType    = 0;
        quint8 q           = 0;
        int width          = 0;
        int height         = 0;

        QByteArray quantTables;
        QByteArray scanData;
    };

    QUdpSocket m_socket;
    FrameAssembly m_frame;

    static quint16 be16(const uchar *p);
    static quint32 be24(const uchar *p);
    static quint32 be32(const uchar *p);

    void resetFrame();
    void processDatagram(const QByteArray &datagram);
    QByteArray buildFullJpeg(const FrameAssembly &f) const;

    static void appendMarker(QByteArray &out, quint8 a, quint8 b);
    static void appendWord(QByteArray &out, quint16 v);
    static void appendDqt(QByteArray &out, quint8 tableId, const uchar *table64);
    static void appendSof0(QByteArray &out, int width, int height, quint8 type);
    static void appendStandardDht(QByteArray &out);
    static void appendSos(QByteArray &out);


};

#endif // RTPJPEGRECEIVER_H
