#include "IncomingConnection.h"

IncomingConnection::IncomingConnection(qintptr descriptor, QSslConfiguration current_ssl_configuration, QObject *parent) :
	QThread(parent),
	socket_descriptor_(descriptor),
	current_ssl_configuration_(current_ssl_configuration)
{

}

void IncomingConnection::run()
{
	QSslSocket *ssl_socket = new QSslSocket();

	if (!ssl_socket)
	{
		THROW(Exception, "Could not create a socket");
		return;
	}
	ssl_socket->setSslConfiguration(current_ssl_configuration_);

	if (!ssl_socket->setSocketDescriptor(socket_descriptor_))
	{
		THROW(Exception, "Could not set a socket descriptor");
		delete ssl_socket;
		return;
	}



	typedef void (QSslSocket::* sslFailed)(const QList<QSslError> &);
	connect(ssl_socket, static_cast<sslFailed>(&QSslSocket::sslErrors), this, &IncomingConnection::sslFailed);
	connect(ssl_socket, &QSslSocket::peerVerifyError, this, &IncomingConnection::verificationFailed);
	connect(ssl_socket, &QSslSocket::encrypted, this, &IncomingConnection::securelyConnected);
//	addPendingConnection(ssl_socket);

	ssl_socket->startServerEncryption();
















//	// thread starts here
//	qDebug() << " Thread started";

//	socket = new QSslSocket();

//	// set the ID
//	if(!socket->setSocketDescriptor(this->socket_descriptor))
//	{
//		// something's wrong, we just emit a signal
////        emit error(socket->error());
//		return;
//	}

//	// connect socket and signal
//	// note - Qt::DirectConnection is used because it's multithreaded
//	//        This makes the slot to be invoked immediately, when the signal is emitted.

//	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
//	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

//	// We'll have multiple clients, we want to know which is which
//	qDebug() << socket_descriptor << " Client connected";

//	// make this thread a loop,
//	// thread will stay alive so that signal/slot to function properly
//	// not dropped out in the middle when thread dies

	exec();
}

//void IncomingConnection::readyRead()
//{
//	// get the information
//	QByteArray Data = socket->readAll();

//	// will write on server side window
//	qDebug() << socket_descriptor << " Data in: " << Data;

//	socket->write(Data);
//}

//void IncomingConnection::disconnected()
//{
//	qDebug() << socket_descriptor << " Disconnected";


//	socket->deleteLater();
//	exit(0);
//}
