#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QTimer>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    MyTcpServer(QObject *parent = 0);
    ~MyTcpServer();

    enum itemId_t
    {
        VS_ID_HR,					//  0 - Heart Rate
        VS_ID_TEMP,					//  1 - Temperatue
        VS_ID_LAST
    } itemId;

    void writeToSocket();
    
signals:
    
public slots:
    void newConnection();
    void delayTimeoutHanlder();

private:
    static QString simData[VS_ID_LAST];

    QTcpServer *m_server;
    QTcpSocket *m_socket;
    QTimer  m_delayTimer;
    QString m_vsData[VS_ID_LAST];
};

#endif // MYTCPSERVER_H
