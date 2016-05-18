#include "mytcpserver.h"
#include <QTime>

QString MyTcpServer::simData[VS_ID_LAST] =
{
    "60",       //  0 - Heart rate
    "27"        //  1 - Temperature
};

MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
  , m_server()
  , m_socket(NULL)
  , m_delayTimer()
{
    connect(&m_delayTimer, SIGNAL(timeout()), this, SLOT(delayTimeoutHanlder()));
    m_server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(m_server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));

    if(!m_server->listen(QHostAddress::Any, 2346))
        qDebug() << "Server could not start";
    else
        qDebug() << "Server started!";

    if(m_server->isListening())
        qDebug() << "Server is listening";

    m_delayTimer.start(200);

    for(int i = VS_ID_HR; i < VS_ID_LAST; i++)
        m_vsData[i] = "---";
}

MyTcpServer::~MyTcpServer()
{

}

void MyTcpServer::delayTimeoutHanlder()
{
    writeToSocket();
}

// This function will get called everytime a new telnet connection is established
void MyTcpServer::newConnection()
{
    // If there are no pending connections, bail
    if(m_server->hasPendingConnections())
    {
        m_socket = m_server->nextPendingConnection();
        if(m_socket)
        {
            qDebug() << "New connection established: " << m_socket;
            for(int i = VS_ID_HR; i < VS_ID_LAST; i++)
                m_vsData[i] = simData[i];
        }
        else
            qDebug() << "Failed to establish new connection: " << m_socket;
    }
    else
    {
        qDebug() << "No Pending Connections";
        return;
    }
}

void MyTcpServer::writeToSocket()
{
    char data[100];
    static int index = VS_ID_HR;

    if(!m_socket)
        return;

    if(m_socket)
    {
        sprintf(data, "%d^%s", index, m_vsData[index].toStdString().data());

        if(++index >= VS_ID_LAST)
            index = VS_ID_HR;

        if( -1 == m_socket->write(data) )
        {
            qDebug() << "Socket Write error: " << m_socket->localAddress() << "Port: " << m_socket->localPort();

            if(m_socket != NULL)
            {
                m_socket->close();
                delete m_socket;
                m_socket = NULL;

                for(int i = VS_ID_HR; i < VS_ID_LAST; i++)
                    m_vsData[i] = "---";
            }

            return;
        }

        qDebug() << "Writing to socket: " << m_socket->localAddress() << "Port: " << m_socket->localPort();
        m_socket->flush();
//        m_socket->waitForBytesWritten(500);
    }
}
