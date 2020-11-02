#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QSslSocket>
#include "RequestHandler.h"
#include "WorkerThread.h"

static qint64 MAX_REQUEST_LENGTH = 2048; // for the IE compatibility

RequestHandler::RequestHandler(QTcpSocket *sock, Api *api)
	: state(State::PROCESSING_REQUEST),
	  socket(sock),
	  api(api)
{
	connect(sock, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}

RequestHandler::~RequestHandler()
{
}

Request::MethodType RequestHandler::inferRequestMethod(QByteArray in)
{
	if (in.toUpper() == QByteArrayLiteral("GET")) {
		return Request::MethodType::GET;
	}
	if (in.toUpper() == QByteArrayLiteral("POST")) {
		return Request::MethodType::POST;
	}
	if (in.toUpper() == QByteArrayLiteral("DELETE")) {
		return Request::MethodType::DELETE;
	}
	if (in.toUpper() == QByteArrayLiteral("PUT")) {
		return Request::MethodType::PUT;
	}
	if (in.toUpper() == QByteArrayLiteral("PATCH")) {
		return Request::MethodType::PATCH;
	}

	THROW(ArgumentException, "Incorrect request method");
}

void RequestHandler::dataReceived()
{
	Response result {};
	Request request {};
	request.remote_address = socket->peerAddress().toString();

	switch (state)
	{
		case State::PROCESSING_REQUEST:
		{
			if (socket->canReadLine()) {
				QByteArray sent_data = socket->readLine();

				QList<QByteArray> sent_data_items = sent_data.split(' ');
				if (sent_data_items.length() < 2)
				{
//					writeResponse(api->showError(WebEntity::BAD_REQUEST, "Cannot process the request. It is possible a URL is missing or incorrect"));
//					WorkerThread *workerThread = new WorkerThread(this, request);
//						connect(workerThread, &WorkerThread::resultReady, this, &RequestHandler::handleResults);
//						connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
//						workerThread->start();

					writeResponse(WebEntity::createError(WebEntity::BAD_REQUEST, "Cannot process the request. It is possible a URL is missing or incorrect"));
					return;
				}

				try
				{
					request.method = inferRequestMethod(sent_data_items[0].toUpper());
				}
				catch (ArgumentException& e)
				{
//					writeResponse(api->showError(WebEntity::BAD_REQUEST, e.message()));

					writeResponse(WebEntity::createError(WebEntity::BAD_REQUEST, e.message()));
					return;
				}
				request.path = sent_data_items[1];

				state = State::PROCESSING_HEADERS;
			}
			else if (socket->bytesAvailable() > MAX_REQUEST_LENGTH)
			{
//				writeResponse(api->showError(WebEntity::BAD_REQUEST, "Maximum request lenght has been exceeded"));
				writeResponse(WebEntity::createError(WebEntity::BAD_REQUEST, "Maximum request lenght has been exceeded"));
				return;
			}
			[[fallthrough]];
		}

		case State::PROCESSING_HEADERS:
		{
			while (socket->canReadLine()) {
				QByteArray sent_data = socket->readLine();

				if (hasEndOfLineCharsOnly(sent_data))
				{
//					try
//					{

						WorkerThread *workerThread = new WorkerThread(this, request);
							connect(workerThread, &WorkerThread::resultReady, this, &RequestHandler::handleResults);
							connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
							workerThread->start();

//						result = api->processRequest(request);

//					}
//					catch(FileAccessException& e)
//					{
//						writeResponse(WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, e.message()));
//					}
//					catch(Exception& e)
//					{
//						qDebug("NOT FOUND ----------------------");
//						writeResponse(WebEntity::createError(WebEntity::NOT_FOUND, e.message()));
//					}

					state = State::FINISHED;
					break;
				}

				int separator = sent_data.indexOf(':');
				if (separator == -1) {
//					writeResponse(api->showError(WebEntity::BAD_REQUEST, "Malformed header: " + sent_data.toHex()));

					writeResponse(WebEntity::createError(WebEntity::BAD_REQUEST, "Malformed header: " + sent_data.toHex()));
					return;
				}

				request.headers.insert(sent_data.left(separator).toLower(), sent_data.mid(separator+1).trimmed());
			}
			[[fallthrough]];
		}

		case State::FINISHED:
		{			
//			writeResponse(result);
			break;
		}
    }

	qDebug() << "Request headers";
	QMap<QString, QString>::const_iterator i = request.headers.constBegin();
	while (i != request.headers.constEnd()) {
		qDebug() << i.key() << ": " << i.value();
		++i;
	}
	qDebug() << "";
}

void RequestHandler::writeResponse(Response response)
{
	socket->write(response.headers);
	socket->write(response.body);
	socket->flush();
	socket->close();
	socket->deleteLater();
}

bool RequestHandler::hasEndOfLineCharsOnly(QByteArray line)
{
	if (line == QByteArrayLiteral("\r\n") || line == QByteArrayLiteral("\n")) {
		return true;
	}
	return false;
}

void RequestHandler::handleResults(const QByteArray &headers, const QByteArray &body)
{
	qDebug() << "Handle results";
	writeResponse(Response{headers, body});
	completness = "Text";
}

//Response RequestHandler::showError(WebEntity::ErrorType type, QString message)
//{
//	QByteArray headers {};
//	QString caption = WebEntity::errorTypeToText(type);


//	QString body = WebEntity::getErrorPageTemplate(); //readFileContent(":/assets/client/error.html");
////	QString body = QString(content);

//	if (message.isEmpty())
//	{
//		message	= "No information provided";
//	}
//	body.replace("%TITLE%", "Error " + QString::number(WebEntity::errorCodeByType(type)) + " - " + WebEntity::errorTypeToText(type));
//	body.replace("%MESSAGE%", message);

//	headers.append("HTTP/1.1 " + QString::number(WebEntity::errorCodeByType(type)) + " FAIL\n");
//	headers.append("Content-Length: " + QString::number(body.length()) + "\n");
//	headers.append("\n");

//	return Response{headers, body.toLocal8Bit()};
//}
