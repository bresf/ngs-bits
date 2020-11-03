#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include "WebEntity.h"
#include "Exceptions.h"
#include "NGSD.h"
#include "Settings.h"

Q_DECLARE_METATYPE(Response)

class RequestHandler : public QObject
{
    Q_OBJECT

public:
	RequestHandler(QTcpSocket *socket);
	~RequestHandler();

private slots:
	void dataReceived();

private:
	QTcpSocket *socket;	
	Request::MethodType inferRequestMethod(QByteArray in);
	void writeResponse(Response response);
	bool hasEndOfLineCharsOnly(QByteArray line);
	void handleResults(const Response &response);
	Request processRequest();
	void processHeaders(Request &request);
};

#endif // REQUESTHANDLER
