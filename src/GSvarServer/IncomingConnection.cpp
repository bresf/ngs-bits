#include "IncomingConnection.h"

#include "IncomingConnection.h"

IncomingConnection::IncomingConnection(qintptr descriptor, QObject *parent) :
	QThread(parent)
{
	this->socket_descriptor = descriptor;
}

void IncomingConnection::run()
{
	// thread starts here
	qDebug() << " Thread started";

	socket = new QSslSocket();

	// set the ID
	if(!socket->setSocketDescriptor(this->socket_descriptor))
	{
		// something's wrong, we just emit a signal
//        emit error(socket->error());
		return;
	}

	// connect socket and signal
	// note - Qt::DirectConnection is used because it's multithreaded
	//        This makes the slot to be invoked immediately, when the signal is emitted.

	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

	// We'll have multiple clients, we want to know which is which
	qDebug() << socket_descriptor << " Client connected";

	// make this thread a loop,
	// thread will stay alive so that signal/slot to function properly
	// not dropped out in the middle when thread dies

	exec();
}

void IncomingConnection::readyRead()
{
	// get the information
	QByteArray Data = socket->readAll();

	// will write on server side window
	qDebug() << socket_descriptor << " Data in: " << Data;

	socket->write(Data);
}

void IncomingConnection::disconnected()
{
	qDebug() << socket_descriptor << " Disconnected";


	socket->deleteLater();
	exit(0);
}
