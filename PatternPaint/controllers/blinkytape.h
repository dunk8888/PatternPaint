#ifndef BLINKYTAPE_H
#define BLINKYTAPE_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QPointer>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "blinkycontroller.h"

/// Connect to a BlinkyTape over a serial port, and manage sending data to it.
class BlinkyTape : public BlinkyController
{
    Q_OBJECT
public:
    BlinkyTape(QSerialPortInfo info, QObject *parent);

    QString getName() const;

    bool isConnected();
    bool open();
    void sendUpdate(QByteArray colors);
    bool getPortInfo(QSerialPortInfo &info);

    bool getUploader(QPointer<BlinkyUploader> &uploader);

    // Atempt to reset the strip by setting it's baud rate to 1200 and closing it.
    void reset();

public slots:

    void close();

private:
    /// Serial port the BlinkyTape is connected to
    QPointer<QSerialPort> serial;

    QSerialPortInfo serialInfo;

    QTimer resetTimer;

    int resetTriesRemaining;

    bool hasError;  // If true, reset will not be attempted on close.

#if defined(Q_OS_WIN)
    // Windows only: Timer that periodically checks if the serial device is
    // still present (it seems to disappear without sending an error?)
    // Handle this by periodically scanning for available BlinkyTapes, and
    // close this one if it no longer in the list
    // TODO: Pull the latest QtSerialPort, it seems to have a fix for this
    QTimer connectionScannerTimer;
#endif

    /// serialWrite timer is used to limit the amount of data that can be
    /// transmitted to the device at one time. Chunks are sent one at a time,
    /// with a pause in between.
    QTimer serialWriteTimer;
    QList<QByteArray> chunks;

signals:
    void connectionStatusChanged(bool status);

private slots:
    void handleSerialError(QSerialPort::SerialPortError error);

    void handleSerialReadData();

    void handleBaudRateChanged(qint32 baudRate, QSerialPort::Directions);

    void resetTimer_timeout();

#if defined(Q_OS_WIN)
    void connectionScannerTimer_timeout();
#endif

    void sendChunk();
};

#endif // BLINKYTAPE_H
