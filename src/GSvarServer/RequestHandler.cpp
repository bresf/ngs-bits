#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QSslSocket>
#include "RequestHandler.h"
#include "WorkerThread.h"

static qint64 MAX_REQUEST_LENGTH = 2048; // for the IE compatibility

RequestHandler::RequestHandler(QTcpSocket *sock)
	: socket(sock)
{
	qRegisterMetaType<Response>();
	connect(sock, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}

RequestHandler::~RequestHandler()
{
}

Request::MethodType RequestHandler::inferRequestMethod(QByteArray in)
{
	if (in.toUpper() == QByteArrayLiteral("GET"))
	{
		return Request::MethodType::GET;
	}
	if (in.toUpper() == QByteArrayLiteral("POST"))
	{
		return Request::MethodType::POST;
	}
	if (in.toUpper() == QByteArrayLiteral("DELETE"))
	{
		return Request::MethodType::DELETE;
	}
	if (in.toUpper() == QByteArrayLiteral("PUT"))
	{
		return Request::MethodType::PUT;
	}
	if (in.toUpper() == QByteArrayLiteral("PATCH"))
	{
		return Request::MethodType::PATCH;
	}

	THROW(ArgumentException, "Incorrect request method");
}

Request RequestHandler::processRequest()
{
	Request request {};
	request.remote_address = socket->peerAddress().toString();

	if (socket->canReadLine())
	{
		QByteArray sent_data = socket->readLine();

		QList<QByteArray> sent_data_items = sent_data.split(' ');
		if (sent_data_items.length() < 2)
		{
			writeResponse(WebEntity::createError(WebEntity::BAD_REQUEST, "Cannot process the request. It is possible a URL is missing or incorrect"));
		}

		try
		{
			request.method = inferRequestMethod(sent_data_items[0].toUpper());
		}
		catch (ArgumentException& e)
		{
			writeResponse(WebEntity::createError(WebEntity::BAD_REQUEST, e.message()));
		}
		request.path = sent_data_items[1];

		return request;
	}
	else if (socket->bytesAvailable() > MAX_REQUEST_LENGTH)
	{
		writeResponse(WebEntity::createError(WebEntity::BAD_REQUEST, "Maximum request lenght has been exceeded"));
	}
	return request;
}

void RequestHandler::processHeaders(Request &request)
{
	while (socket->canReadLine())
	{
		QByteArray sent_data = socket->readLine();

		if (hasEndOfLineCharsOnly(sent_data))
		{
			WorkerThread *workerThread = new WorkerThread(request);
			connect(workerThread, &WorkerThread::resultReady, this, &RequestHandler::handleResults);
			connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
			workerThread->start();

			break;
		}

		int separator = sent_data.indexOf(':');
		if (separator == -1)
		{
			writeResponse(WebEntity::createError(WebEntity::BAD_REQUEST, "Malformed header: " + sent_data.toHex()));
			return;
		}

		request.headers.insert(sent_data.left(separator).toLower(), sent_data.mid(separator+1).trimmed());
	}
}

void RequestHandler::dataReceived()
{
	qDebug() << "New request received";
	try
	{
		NGSD db;


		//get CNV ID
		QStringList callset_id = db.getValues("SELECT comment FROM genome", "");
		qDebug() << callset_id;

	}
	catch (DatabaseException& e)
	{
		qDebug() << e.message();
	}

	Request request = processRequest();
	processHeaders(request);

	qDebug() << "Request headers";
	QMap<QString, QString>::const_iterator i = request.headers.constBegin();
	while (i != request.headers.constEnd())
	{
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
	if (line == QByteArrayLiteral("\r\n") || line == QByteArrayLiteral("\n"))
	{
		return true;
	}
	return false;
}

void RequestHandler::handleResults(const Response &response)
{
	writeResponse(response);
}
