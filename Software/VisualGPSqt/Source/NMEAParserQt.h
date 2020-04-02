/*
* MIT License
*
*  Copyright (c) 2018 VisualGPS, LLC
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*  SOFTWARE.
*
*/
#ifndef NMEAPARSERQT_H
#define NMEAPARSERQT_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QSerialPort>
#include "NMEAParser.h"

#define NP_MAXFIELD                     256                         ///< Max field length
#define NP_GPS_MIN_PRN                  1                           ///< Minimum PRN
#define NP_GPS_MAX_PRN                  32                          ///< Minimum PRN
#define NP_WAAS_MIN_PRN                 33                          ///< Minimum PRN
#define NP_WAAS_MAX_PRN                 64                          ///< Minimum PRN
#define NP_GLONASS_MIN_PRN              65                          ///< Minimum PRN
#define NP_GLONASS_MAX_PRN              96                          ///< Minimum PRN

class CNMEAParserQt : public QObject, public CNMEAParser
{
private:
    Q_OBJECT
    QSerialPort         m_SerialPort;                               ///< The serial port
    QFile *             m_pReadNmeaFile;                            ///< Read a nmea file
    QTimer *            m_pReadNmeaFilePollTimer;                   ///< Timer to read the NMEA read file a few bytes at a time

public:
    explicit CNMEAParserQt(QObject *parent = nullptr);
    bool Connect(QString strPort, quint32 nBaud);
    void CloseNmeaReadFile();

signals:
    void NewPositionUpdateGPS();
    void MessageProcessed();
    void NewMessageUpdateGPS(QByteArray &);

public slots:
    void on_SerialPortReadyRead();

    bool ConnectUsingFile(QString strFileName);

private slots:
    void OnNmeaReadFileTimer();

protected:
    ///
    /// \brief This method is redefined from CNMEAParserPacket::ProcessRxCommand(char *pCmd, char *pData)
    ///
    /// \param pCmd Pointer to the NMEA command string
    /// \param pData Comma separated data that belongs to the command
    /// \return Returns CNMEAParserData::ERROR_OK If successful
    ///
    virtual CNMEAParserData::ERROR_E ProcessRxCommand(char *pCmd, char *pData);

};

#endif // NMEAPARSERQT_H
