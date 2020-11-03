#include "WorkerThread.h"

WorkerThread::WorkerThread(Request request)
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

QList<QByteArray> WorkerThread::getKeyValuePair(QByteArray in)
{
	QList<QByteArray> result {};

	if (in.indexOf('=')>-1)
	{
		result = in.split('=');
	}

	return result;
}

QMap<QString, QString> WorkerThread::getVariables(QByteArray in)
{
	QMap<QString, QString> url_vars {};
	QList<QByteArray> var_list = in.split('&');
	QByteArray cur_key {};

	for (int i = 0; i < var_list.count(); ++i)
	{
		QList<QByteArray> pair = getKeyValuePair(var_list[i]);
		if (pair.length()==2)
		{
			url_vars.insert(pair[0], pair[1]);
		}
	}

	return url_vars;
}

QByteArray WorkerThread::getVariableSequence(QByteArray url)
{
	QByteArray var_string {};
	if (url.indexOf('?') == -1) return var_string;
	return url.split('?')[1];
}

void WorkerThread::run()
{
	qDebug() << "Processing path:" << request_.path;
	QByteArray body {};

	QList<QByteArray> path_items = request_.path.split('/');
	if (path_items.isEmpty())
	{

		Response response = WebEntity::createError(WebEntity::BAD_REQUEST, "No path has been provided");
		emit resultReady(response);
		return;
	}


	qDebug() << "Var list = " << getVariables(getVariableSequence(path_items[1]));

	// index page
	if ((path_items[1] == "") && request_.method == Request::MethodType::GET)
	{
		try
		{
			body = readFileContent(":/assets/client/info.html");
		}
		catch(Exception& e)
		{
			Response result = WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, e.message());
			emit resultReady(result);
			return;
		}

		emit resultReady(Response{generateHeaders(body.length(), WebEntity::TEXT_HTML), body});
		return;
	}

	// api info page (e.g. version info)
	if ((path_items[1] == QByteArrayLiteral("info")) && request_.method == Request::MethodType::GET)
	{
		try
		{
			body = readFileContent(":/assets/client/api.json");
		}
		catch(Exception& e)
		{
			Response result = WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, e.message());
			emit resultReady(result);
			return;
		}

		emit resultReady(Response{generateHeaders(body.length(), WebEntity::APPLICATION_JSON), body});
		return;
	}

	Response response = WebEntity::createError(WebEntity::NOT_FOUND, "The page you are looking for does not exist");
	emit resultReady(response);
}
