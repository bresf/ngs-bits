#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include "WebEntity.h"
#include "Exceptions.h"
#include "NGSD.h"
#include "Settings.h"


class RequestHandler : public QObject
{
    Q_OBJECT

public:
    enum State {
		PROCESSING_REQUEST,
		PROCESSING_HEADERS,
		FINISHED
    };

	RequestHandler(QTcpSocket *socket);
	~RequestHandler();

private slots:
	void dataReceived();


private:
    State state;
	QTcpSocket *socket;	
	Request::MethodType inferRequestMethod(QByteArray in);
	void writeResponse(Response response);
	bool hasEndOfLineCharsOnly(QByteArray line);
	void handleResults(const QByteArray &headers, const QByteArray &body);
	Request processRequest();
	void processHeaders(Request &request);
};

#endif // REQUESTHANDLER
