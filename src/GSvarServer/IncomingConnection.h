#ifndef INCOMINGCONNECTION_H
#define INCOMINGCONNECTION_H

#include <QObject>
#include <QThread>
#include <QSslSocket>

class IncomingConnection : public QThread
{
	Q_OBJECT

public:
	explicit IncomingConnection(qintptr descriptor, QObject *parent = 0);
	void run();

public slots:
	void readyRead();
	void disconnected();

private:
	QSslSocket *socket;
	qintptr socket_descriptor;

};

#endif // INCOMINGCONNECTION_H
