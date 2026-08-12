#include "rtpjpegreceiver.h"
#include <QNetworkDatagram>
#include <QString>
#include <QObject>
#include <QVariant>
namespace{

    void appendHuffmanTable(QByteArray &out,
                            quint8 tableClass, quint8 tableId,
                            const uchar *bits16, int bitsLen,
                            const uchar *vals, int valsLen)
    {
        out.append(char(0xFF));
        out.append(char(0xC4));

        quint16 len = quint16(2 + 1 + bitsLen + valsLen);
        out.append(char((len >> 8) & 0xFF));
        out.append(char(len & 0xFF));

        out.append(char((tableClass << 4) | tableId));
        out.append(reinterpret_cast<const char*>(bits16), bitsLen);
        out.append(reinterpret_cast<const char*>(vals), valsLen);



    }
    namespace
    {
    static const uchar kDefaultQuantizers[128] = {
        // luma
        16, 11, 12, 14, 12, 10, 16, 14,
        13, 14, 18, 17, 16, 19, 24, 40,
        26, 24, 22, 22, 24, 49, 35, 37,
        29, 40, 58, 51, 61, 60, 57, 51,
        56, 55, 64, 72, 92, 78, 64, 68,
        87, 69, 55, 56, 80, 109, 81, 87,
        95, 98, 103, 104, 103, 62, 77, 113,
        121, 112, 100, 120, 92, 101, 103, 99,

        // chroma
        17, 18, 18, 24, 21, 24, 47, 26,
        26, 47, 99, 66, 56, 66, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99
    };

    static const uchar bits_dc_luminance[] =
        {0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0};
    static const uchar val_dc_luminance[] =
        {0,1,2,3,4,5,6,7,8,9,10,11};

    static const uchar bits_dc_chrominance[] =
        {0,0,3,1,1,1,1,1,1,1,1,0,0,0,0,0,0};
    static const uchar val_dc_chrominance[] =
        {0,1,2,3,4,5,6,7,8,9,10,11};

    static const uchar bits_ac_luminance[] =
        {0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d};
    static const uchar val_ac_luminance[] = {
        0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,
        0x22,0x71,0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,
        0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,
        0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
        0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
        0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
        0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,
        0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,
        0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,
        0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,
        0xF9,0xFA
    };

    static const uchar bits_ac_chrominance[] =
        {0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77};
    static const uchar val_ac_chrominance[] = {
        0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,
        0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,0xF0,
        0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,0x26,
        0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,
        0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,
        0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,
        0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,
        0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,
        0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,
        0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,
        0xF9,0xFA
    };

    void putMarker(QByteArray &out, quint8 code)
    {
        out.append(char(0xFF));
        out.append(char(code));
    }

    void putBe16(QByteArray &out, quint16 v)
    {
        out.append(char((v >> 8) & 0xFF));
        out.append(char(v & 0xFF));
    }

    int putHuffTablePayload(QByteArray &out,
                            quint8 tableClass,
                            quint8 tableId,
                            const uchar *bits17,
                            const uchar *vals)
    {
        int n = 0;
        out.append(char((tableClass << 4) | tableId));

        for (int i = 1; i <= 16; ++i) {
            n += bits17[i];
            out.append(char(bits17[i]));
        }
        out.append(reinterpret_cast<const char*>(vals), n);
        return n + 17;
    }

    QByteArray makeDefaultQTables(quint8 q)
    {
        QByteArray qt(128, Qt::Uninitialized);

        int factor = q;
        if (factor < 1) factor = 1;
        if (factor > 99) factor = 99;

        int S = (q < 50) ? (5000 / factor) : (200 - factor * 2);

        for (int i = 0; i < 128; ++i) {
            int val = (kDefaultQuantizers[i] * S + 50) / 100;
            if (val < 1) val = 1;
            if (val > 255) val = 255;
            qt[i] = char(val);
        }
        return qt;
    }

    QByteArray createJpegHeader(quint8 type,
                                int width,
                                int height,
                                const QByteArray &qtables)
    {
        const int nbQTables = qtables.size() / 64;
        if (nbQTables != 1 && nbQTables != 2)
            return {};

        QByteArray out;
        out.reserve(1024);

        // SOI
        putMarker(out, 0xD8);

        // APP0 / JFIF
        putMarker(out, 0xE0);
        putBe16(out, 16);
        out.append("JFIF", 5);       // "JFIF\0"
        putBe16(out, 0x0102);
        out.append(char(0));
        putBe16(out, 1);
        putBe16(out, 1);
        out.append(char(0));
        out.append(char(0));

        // DQT
        putMarker(out, 0xDB);
        putBe16(out, quint16(2 + nbQTables * (1 + 64)));
        for (int i = 0; i < nbQTables; ++i) {
            out.append(char(i));
            out.append(qtables.constData() + i * 64, 64);
        }

        // DHT: один сегмент на все 4 таблицы
        putMarker(out, 0xC4);
        const int dhtLenPos = out.size();
        putBe16(out, 0); // временно

        int dhtSize = 2;
        dhtSize += putHuffTablePayload(out, 0, 0, bits_dc_luminance,   val_dc_luminance);
        dhtSize += putHuffTablePayload(out, 0, 1, bits_dc_chrominance, val_dc_chrominance);
        dhtSize += putHuffTablePayload(out, 1, 0, bits_ac_luminance,   val_ac_luminance);
        dhtSize += putHuffTablePayload(out, 1, 1, bits_ac_chrominance, val_ac_chrominance);

        out[dhtLenPos]     = char((dhtSize >> 8) & 0xFF);
        out[dhtLenPos + 1] = char(dhtSize & 0xFF);

        // SOF0
        putMarker(out, 0xC0);
        putBe16(out, 17);
        out.append(char(8));
        putBe16(out, quint16(height));
        putBe16(out, quint16(width));
        out.append(char(3));

        out.append(char(1));
        out.append(char((2 << 4) | (type ? 2 : 1)));   // как у FFmpeg
        out.append(char(0));

        out.append(char(2));
        out.append(char(0x11));
        out.append(char(nbQTables == 2 ? 1 : 0));

        out.append(char(3));
        out.append(char(0x11));
        out.append(char(nbQTables == 2 ? 1 : 0));

        // SOS
        putMarker(out, 0xDA);
        putBe16(out, 12);
        out.append(char(3));

        out.append(char(1));
        out.append(char(0x00));

        out.append(char(2));
        out.append(char(0x11));

        out.append(char(3));
        out.append(char(0x11));

        out.append(char(0));
        out.append(char(63));
        out.append(char(0));

        return out;
    }
    }

}//namespace


RtpJpegReceiver::RtpJpegReceiver(quint16 port, QObject *parent)
    : QObject(parent)
{
        //qDebug() << "RtpJpegReceiver ctor start";
        m_socket.setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, QVariant(4 * 1024 * 1024));
        const bool c = connect(&m_socket, &QUdpSocket::readyRead,
                               this, &RtpJpegReceiver::onReadyRead);
        //qDebug() << "connect readyRead =" << c;

        const bool ok = m_socket.bind(QHostAddress::AnyIPv4, port);
        //qDebug() << "bind =" << ok
//                 << "port =" << port
//                 << "localPort =" << m_socket.localPort()
//                 << "error =" << m_socket.errorString();

        //qDebug() << "RtpJpegReceiver ctor end";
}


void RtpJpegReceiver::onReadyRead()
{
        //qDebug() << "readyRead fired";

        while (m_socket.hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(int(m_socket.pendingDatagramSize()));

            QHostAddress sender;
            quint16 senderPort = 0;

            const qint64 n = m_socket.readDatagram(datagram.data(),
                                                   datagram.size(),
                                                   &sender,
                                                   &senderPort);

            //qDebug() << "datagram" << n << "from" << sender.toString() << senderPort;

            if (n <= 0)
                continue;

            datagram.resize(int(n));
            processDatagram(datagram);
        }
}


    void RtpJpegReceiver::appendSof0(QByteArray &out, int width, int height, quint8 type)
    {
        quint8 ySampling = 0x21; // default 4:2:2 for type 0

        if (type == 0)
            ySampling = 0x21; // H=2 V=1
        else if (type == 1)
            ySampling = 0x22; // H=2 V=2
        else
            ySampling = 0x21;

        appendMarker(out, 0xFF, 0xC0);
        appendWord(out, 17); // 8 + 3*3
        out.append(char(8)); // precision

        appendWord(out, quint16(height));
        appendWord(out, quint16(width));

        out.append(char(3)); // components

        // Y
        out.append(char(1));
        out.append(char(ySampling));
        out.append(char(0));

        // Cb
        out.append(char(2));
        out.append(char(0x11));
        out.append(char(1));

        // Cr
        out.append(char(3));
        out.append(char(0x11));
        out.append(char(1));
    }


    void RtpJpegReceiver::appendStandardDht(QByteArray &out)
    {
        static const uchar bits_dc_luminance[] =
            {0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
        static const uchar val_dc_luminance[] =
            {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B};

        static const uchar bits_dc_chrominance[] =
            {0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00};
        static const uchar val_dc_chrominance[] =
            {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B};

        static const uchar bits_ac_luminance[] =
            {0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D};
        static const uchar val_ac_luminance[] = {
            0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,
            0x22,0x71,0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,
            0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,
            0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
            0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
            0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
            0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,
            0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,
            0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,
            0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,
            0xF9,0xFA
        };

        static const uchar bits_ac_chrominance[] =
            {0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,0x77};
        static const uchar val_ac_chrominance[] = {
            0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,
            0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,0xF0,
            0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,0x26,
            0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,
            0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,
            0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,
            0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,
            0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,
            0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,
            0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,
            0xF9,0xFA
        };

        appendHuffmanTable(out, 0, 0, bits_dc_luminance,   16, val_dc_luminance,   int(sizeof(val_dc_luminance)));
        appendHuffmanTable(out, 1, 0, bits_ac_luminance,   16, val_ac_luminance,   int(sizeof(val_ac_luminance)));
        appendHuffmanTable(out, 0, 1, bits_dc_chrominance, 16, val_dc_chrominance, int(sizeof(val_dc_chrominance)));
        appendHuffmanTable(out, 1, 1, bits_ac_chrominance, 16, val_ac_chrominance, int(sizeof(val_ac_chrominance)));
    }


    void RtpJpegReceiver::appendSos(QByteArray &out)
    {
        appendMarker(out, 0xFF, 0xDA);
        appendWord(out, 12);
        out.append(char(3));

        // Y
        out.append(char(1));
        out.append(char(0x00));

        // Cb
        out.append(char(2));
        out.append(char(0x11));

        // Cr
        out.append(char(3));
        out.append(char(0x11));

        out.append(char(0x00));
        out.append(char(0x3F));
        out.append(char(0x00));
    }




    void RtpJpegReceiver::appendMarker(QByteArray &out, quint8 a, quint8 b)
    {
        out.append(char(a));
        out.append(char(b));
    }

    void RtpJpegReceiver::appendWord(QByteArray &out, quint16 v)
    {
        out.append(char((v >> 8) & 0xFF));
        out.append(char(v & 0xFF));
    }

    void RtpJpegReceiver::appendDqt(QByteArray &out, quint8 tableId, const uchar *table64)
    {
        appendMarker(out, 0xFF, 0xDB);
        appendWord(out, 67); // 2 + 1 + 64
        out.append(char(tableId & 0x0F));
        out.append(reinterpret_cast<const char*>(table64), 64);
    }

    quint16 RtpJpegReceiver::be16(const uchar *p){
        return (quint16(p[0] << 8) |
                quint16(p[1]));
    }


    quint32 RtpJpegReceiver::be24(const uchar *p){
        return (quint16(p[0] << 16) |
                quint16(p[1] << 8) |
                quint16(p[2]));
    }


    quint32 RtpJpegReceiver::be32(const uchar *p){
        return (quint16(p[0] << 24) |
                quint16(p[1] << 16) |
                quint16(p[2] << 8) |
                quint16(p[3]));
    }


    void RtpJpegReceiver::resetFrame(){
        m_frame = FrameAssembly{};
    }


    void RtpJpegReceiver::processDatagram(const QByteArray &datagram)
    {
        const uchar *p = reinterpret_cast<const uchar*>(datagram.constData());
        const int size = datagram.size();

        if (size < 12)
        {
            emit statsMessage("Datagram too small for RTP");
            return;
        }

        const quint8 vpxcc = p[0];
        const quint8 mpt   = p[1];

        const quint8 version   = (vpxcc >> 6) & 0x03;
        const bool padding     = (vpxcc & 0x20) != 0;
        const bool extension   = (vpxcc & 0x10) != 0;
        const quint8 csrcCount = (vpxcc & 0x0F);

        const bool marker        = (mpt & 0x80) != 0;
        const quint8 payloadType = (mpt & 0x7F);

        const quint16 seq       = be16(p + 2);
        const quint32 timestamp = be32(p + 4);

        if (version != 2)
        {
            emit statsMessage(QString("Unsupported RTP version: %1").arg(version));
            return;
        }

        int rtpHeaderSize = 12 + csrcCount * 4;
        if (size < rtpHeaderSize)
        {
            emit statsMessage("Bad RTP header size");
            return;
        }

        if (extension)
        {
            if (size < rtpHeaderSize + 4)
            {
                emit statsMessage("Bad RTP extension header");
                return;
            }

            const quint16 extLenWords = be16(p + rtpHeaderSize + 2);
            rtpHeaderSize += 4 + extLenWords * 4;

            if (size < rtpHeaderSize)
            {
                emit statsMessage("Truncated RTP extension");
                return;
            }
        }

        int payloadSize = size - rtpHeaderSize;
        if (padding)
        {
            const quint8 padCount = quint8(p[size - 1]);
            if (padCount == 0 || padCount > payloadSize)
            {
                emit statsMessage("Invalid RTP padding");
                return;
            }
            payloadSize -= padCount;
        }

        if (payloadSize < 8)
        {
            emit statsMessage("Payload too small for RTP/JPEG");
            return;
        }

        const uchar *jp = p + rtpHeaderSize;

        const quint8 typeSpecific   = jp[0];
        const quint32 fragmentOffset = be24(jp + 1);
        const quint8 type           = jp[4];
        const quint8 q              = jp[5];
        const quint8 width8         = jp[6];
        const quint8 height8        = jp[7];

        Q_UNUSED(typeSpecific);

        const int width  = int(width8) * 8;
        const int height = int(height8) * 8;

        int jpegHeaderSize = 8;
        int payloadOffset = rtpHeaderSize + jpegHeaderSize;

        // restart header для type 64..127 пока не поддерживаем
        if (type >= 64 && type <= 127)
        {
            emit statsMessage(QString("Restart-marker JPEG type not supported yet: %1").arg(type));
            resetFrame();
            return;
        }

        QByteArray inBandQuantTables;

        // Q >= 128 => в первом пакете кадра после main JPEG header идет Quantization Table header
        if (q >= 128)
        {
            if (fragmentOffset == 0)
            {
                if (payloadSize < jpegHeaderSize + 4)
                {
                    emit statsMessage("No space for Quantization Table header");
                    resetFrame();
                    return;
                }

                const uchar *qt = jp + 8;
                const quint8 mbz       = qt[0];
                const quint8 precision = qt[1];
                const quint16 qtLen    = be16(qt + 2);

                Q_UNUSED(mbz);

                if (precision != 0)
                {
                    emit statsMessage(QString("Unsupported quant precision: %1").arg(precision));
                    resetFrame();
                    return;
                }

                if (qtLen != 64 && qtLen != 128)
                {
                    emit statsMessage(QString("Unsupported quant table length: %1").arg(qtLen));
                    resetFrame();
                    return;
                }

                if (payloadSize < jpegHeaderSize + 4 + qtLen)
                {
                    emit statsMessage("Truncated quant tables");
                    resetFrame();
                    return;
                }

                inBandQuantTables = QByteArray(reinterpret_cast<const char*>(qt + 4), qtLen);
                payloadOffset += 4 + qtLen;
            }
            else if (!m_frame.active || m_frame.timestamp != timestamp)
            {
                emit statsMessage("Got non-first fragment with Q>=128 before frame start");
                resetFrame();
                return;
            }
        }

        if (payloadOffset > size)
        {
            emit statsMessage("Bad payload offset");
            resetFrame();
            return;
        }

        const int jpegScanSize = size - payloadOffset - (padding ? quint8(p[size - 1]) : 0);
        if (jpegScanSize < 0)
        {
            emit statsMessage("Negative JPEG payload size");
            resetFrame();
            return;
        }

        const uchar *scanPtr = reinterpret_cast<const uchar*>(datagram.constData()) + payloadOffset;

        // старт нового кадра
        if (!m_frame.active || fragmentOffset == 0 || m_frame.timestamp != timestamp)
        {
            if (fragmentOffset != 0)
            {
                emit statsMessage(QString("Dropped fragment: frame start expected, got offset=%1").arg(fragmentOffset));
                resetFrame();
                return;
            }

            resetFrame();
            m_frame.active = true;
            m_frame.timestamp = timestamp;
            m_frame.lastSeq = seq;
            m_frame.nextOffset = 0;
            m_frame.jpegType = type;
            m_frame.q = q;
            m_frame.width = width;
            m_frame.height = height;
            m_frame.quantTables = inBandQuantTables;
            m_frame.scanData.clear();
        }
        else
        {
            const quint16 expectedSeq = quint16(m_frame.lastSeq + 1);
            if (seq != expectedSeq)
            {
                emit statsMessage(QString("Packet loss/reorder: expected seq=%1 got=%2").arg(expectedSeq).arg(seq));
                resetFrame();
                return;
            }

            if (timestamp != m_frame.timestamp)
            {
                emit statsMessage("Timestamp changed inside frame");
                resetFrame();
                return;
            }

            if (fragmentOffset != m_frame.nextOffset)
            {
                emit statsMessage(QString("Fragment offset mismatch: expected=%1 got=%2")
                                      .arg(m_frame.nextOffset)
                                      .arg(fragmentOffset));
                resetFrame();
                return;
            }

            m_frame.lastSeq = seq;
        }

        m_frame.scanData.append(reinterpret_cast<const char*>(scanPtr), jpegScanSize);
        m_frame.nextOffset += quint32(jpegScanSize);

        if (!marker)
            return;

        QByteArray fullJpeg = buildFullJpeg(m_frame);
        if (fullJpeg.isEmpty())
        {
            emit statsMessage("Failed to build JPEG");
            resetFrame();
            return;
        }

        QImage img;
        if (!img.loadFromData(fullJpeg, "JPEG"))
        {
            emit statsMessage(QString("JPEG decode failed, assembled size=%1").arg(fullJpeg.size()));
            resetFrame();
            return;
        }

        emit frameReady(img);
        emit statsMessage(QString("Frame OK: %1x%2, %3 bytes scan, ts=%4")
                              .arg(img.width())
                              .arg(img.height())
                              .arg(m_frame.scanData.size())
                              .arg(m_frame.timestamp));

        resetFrame();
    }


//    QByteArray RtpJpegReceiver::buildFullJpeg(const FrameAssembly &f) const
//    {
//        if (!f.active && f.scanData.isEmpty())
//            return {};

//        if (f.width <= 0 || f.height <= 0)
//            return {};

//        if (f.jpegType > 1)
//        {
//            // пока только базовые type 0/1
//            return {};
//        }

//        QByteArray out;
//        out.reserve(f.scanData.size() + 1024);

//        // SOI
//        appendMarker(out, 0xFF, 0xD8);

//        // DQT
//        if (!f.quantTables.isEmpty())
//        {
//            if (f.quantTables.size() == 64)
//            {
//                appendDqt(out, 0, reinterpret_cast<const uchar*>(f.quantTables.constData()));
//                appendDqt(out, 1, reinterpret_cast<const uchar*>(f.quantTables.constData()));
//            }
//            else if (f.quantTables.size() == 128)
//            {
//                appendDqt(out, 0, reinterpret_cast<const uchar*>(f.quantTables.constData()));
//                appendDqt(out, 1, reinterpret_cast<const uchar*>(f.quantTables.constData() + 64));
//            }
//            else
//            {
//                return {};
//            }
//        }
//        else
//        {
//            // q < 128, а генерацию стандартных quant-таблиц пока не делаем
//            // если надо, следующим сообщением добавлю makeTables(q)
//            return {};
//        }

//        appendSof0(out, f.width, f.height, f.jpegType);
//        appendStandardDht(out);
//        appendSos(out);

//        out.append(f.scanData);

//        // EOI
//        appendMarker(out, 0xFF, 0xD9);

//        return out;
//    }
    QByteArray RtpJpegReceiver::buildFullJpeg(const FrameAssembly &f) const
    {
        if (f.width <= 0 || f.height <= 0 || f.scanData.isEmpty())
            return {};

        if (f.jpegType > 1)
            return {};

        QByteArray qtables;

        if (!f.quantTables.isEmpty()) {
            qtables = f.quantTables;
        } else {
            if (f.q > 127)
                return {};
            qtables = makeDefaultQTables(f.q);
        }

        QByteArray header = createJpegHeader(f.jpegType, f.width, f.height, qtables);
        if (header.isEmpty())
            return {};

        QByteArray out;
        out.reserve(header.size() + f.scanData.size() + 2);
        out.append(header);
        out.append(f.scanData);

        // EOI
        out.append(char(0xFF));
        out.append(char(0xD9));

        return out;
    }

