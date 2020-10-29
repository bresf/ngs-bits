#ifndef INCOMINGCONNECTION_H
#define INCOMINGCONNECTION_H

#include <QObject>
#include <QThread>
#include <QSslSocket>
#include <QSslConfiguration>
#include "Exceptions.h"

class IncomingConnection : public QThread
{
	Q_OBJECT

public:
	explicit IncomingConnection(qintptr descriptor, QSslConfiguration current_ssl_configuration, QObject *parent = 0);
	void run();

Q_SIGNALS:
	void sslFailed(const QList<QSslError> &error);
	void verificationFailed(const QSslError &error);
	void securelyConnected();

private:
	QSslSocket *socket_;
	qintptr socket_descriptor_;
	QSslConfiguration current_ssl_configuration_;

};

#endif // INCOMINGCONNECTION_H
