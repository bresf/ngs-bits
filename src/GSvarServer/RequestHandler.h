#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include "WebEntity.h"
#include "Api.h"
#include "Exceptions.h"


class RequestHandler : public QObject
{
    Q_OBJECT

public:
    enum State {
		PROCESSING_REQUEST,
		PROCESSING_HEADERS,
		FINISHED
    };

	RequestHandler(QTcpSocket *socket, Api *api);
	~RequestHandler();

private slots:
	void dataReceived();


private:
    State state;
	QTcpSocket *socket;
	Api *api;
	Request::MethodType inferRequestMethod(QByteArray in);
	void writeResponse(Response response);
	bool hasEndOfLineCharsOnly(QByteArray line);
	void handleResults(const QByteArray &headers, const QByteArray &body);

//	Response showError(WebEntity::ErrorType type, QString message);

	QString completness;

};

#endif // REQUESTHANDLER
