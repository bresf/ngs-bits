#include "WorkerThread.h"


WorkerThread::WorkerThread(QObject* parent, Request request)
	: request_(request)
{
}

QByteArray WorkerThread::readFileContent(QString filename)
{
	qDebug() << "Reading file:" << filename;

	QByteArray content {};
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		THROW(FileAccessException, "File could not be found: " + filename);
//		return content;
	}

	if (!file.atEnd())
	{
		content = file.readAll();
	}
	return content;
}

QByteArray WorkerThread::generateHeaders(int length, WebEntity::ContentType type)
{
	QByteArray headers {};
	headers.append("HTTP/1.1 200 OK\n");
	headers.append("Content-Length: " + QString::number(length) + "\n");
	headers.append("Content-Type: " + WebEntity::contentTypeToString(type) + "\n");
	headers.append("\n");

	return headers;
}



void WorkerThread::run()
{
	qDebug() << "Processing path:" << request_.path;


//	QByteArray headers {};
	QByteArray body {};

	QList<QByteArray> path_items = request_.path.split('/');
	if (path_items.isEmpty())
	{

		Response response = WebEntity::createError(WebEntity::BAD_REQUEST, "No path has been provided");
		emit resultReady(response.headers, response.body);
//		THROW(ArgumentException,"No path has been provided");
	}

	if ((path_items[1] == "") && request_.method == Request::MethodType::GET)
	{

		try
		{
			body = readFileContent(":/assets/client/info.html");
		}
//		catch(FileAccessException& e)
//		{
//			Response result = WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, e.message());
//			emit resultReady(result.headers, result.body);
//		}
		catch(Exception& e)
		{
			Response result = WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, e.message());
			emit resultReady(result.headers, result.body);
			return;
		}

//		headers = generateHeaders(body.length(), WebEntity::TEXT_HTML);
		emit resultReady(generateHeaders(body.length(), WebEntity::TEXT_HTML), body);
		return;
	}

	if ((path_items[1] == QByteArrayLiteral("info")) && request_.method == Request::MethodType::GET)
	{
		try
		{
			body = readFileContent(":/assets/client/api.json1");
		}
		catch(Exception& e)
		{
			Response result = WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, e.message());
			emit resultReady(result.headers, result.body);
			return;
		}

		emit resultReady(generateHeaders(body.length(), WebEntity::APPLICATION_JSON), body);
		return;
//		return serveStaticFile(":/assets/client/api.json1", WebEntity::APPLICATION_JSON);
	}

//	THROW(Exception, "The page you are looking for does not exist");
//	emit resultReady(b)


	Response response = WebEntity::createError(WebEntity::NOT_FOUND, "The page you are looking for does not exist");
	emit resultReady(response.headers, response.body);
//	emit resultReady(headers, body);
}
