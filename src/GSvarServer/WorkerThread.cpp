#include "WorkerThread.h"

WorkerThread::WorkerThread(Request request)
	: request_(request)
{
}

QByteArray WorkerThread::readFileContent(QString filename_with_path)
{
	qDebug() << "Reading file:" << filename_with_path;
	QByteArray content {};

	QString found_id = FileCache::getFileIdIfInCache(filename_with_path);
	if (found_id.length() > 0)
	{
		qDebug() << "File has been found in the cache:" << found_id;
		return FileCache::getFileById(found_id).content;
	}


	QFile file(filename_with_path);
	if (!file.open(QIODevice::ReadOnly))
	{
		THROW(FileAccessException, "File could not be found: " + filename_with_path);
	}

	if (!file.atEnd())
	{
		content = file.readAll();
	}

	qDebug() << "Adding file to the cache:" << filename_with_path;
	FileCache::addFileToCache(WebEntity::generateToken(), filename_with_path, content);
	return content;
}

Response WorkerThread::serverStaticFile(QString filename_with_path, WebEntity::ContentType type, bool is_downloadable)
{
	QByteArray body {};
	try
	{
		body = readFileContent(filename_with_path);
	}
	catch(Exception& e)
	{
		return WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, e.message());
	}

	return (Response{generateHeaders(getFileNameAndExtension(filename_with_path), body.length(), type, is_downloadable), body});
}

QByteArray WorkerThread::generateHeaders(QString filename, int length, WebEntity::ContentType type, bool is_downloadable)
{
	QByteArray headers {};
	headers.append("HTTP/1.1 200 OK\n");
	headers.append("Connection: Keep-Alive\n");
	headers.append("Keep-Alive: timeout=5, max=1000\n");
	headers.append("Content-Length: " + QString::number(length) + "\n");
	headers.append("Content-Type: " + WebEntity::contentTypeToString(type) + "\n");
	if (is_downloadable)
	{
		headers.append("Content-Disposition: form-data; name=file_download; filename=" + filename + "\n");
	}

	headers.append("\n");

	return headers;
}

QString WorkerThread::getUrlPartWithoutParams(QByteArray url)
{
	QList<QByteArray> url_parts = url.split('?');
	return QString(url_parts[0]);
}

void WorkerThread::run()
{
	qDebug() << "Processing path:" << request_.path;
	QByteArray body {};

	QList<QByteArray> path_items = request_.path.split('/');
	if (path_items.isEmpty())
	{
		emit resultReady(WebEntity::createError(WebEntity::BAD_REQUEST, "No path has been provided"));
		return;
	}

	QString first_url_part = getUrlPartWithoutParams(path_items[1]);
	QMap<QString, QString> url_vars = request_.url_params;
	qDebug() << "Variables from the request URL:" << url_vars;

	// index page
	if ((first_url_part == "") && request_.method == Request::MethodType::GET)
	{
		emit resultReady(serverStaticFile(":/assets/client/info.html", WebEntity::TEXT_HTML, false));
		return;
	}

	// api info page (e.g. version info)
	if ((first_url_part == "info") && request_.method == Request::MethodType::GET)
	{
		emit resultReady(serverStaticFile(":/assets/client/api.json", WebEntity::APPLICATION_JSON, false));
		return;
	}

	if ((first_url_part == "file") && request_.method == Request::MethodType::GET)
	{
		emit resultReady(serverStaticFile(":/assets/client/example.png", WebEntity::APPLICATION_OCTET_STREAM, true));
		return;
	}

	Response response = WebEntity::createError(WebEntity::NOT_FOUND, "The page you are looking for does not exist");
	if (request_.method != Request::MethodType::GET)
	{
		response = WebEntity::createError(WebEntity::NOT_IMPLEMENTED, "Only GET requests are supported at the moment");
	}
	emit resultReady(response);
}

QString WorkerThread::getFileNameAndExtension(QString filename_with_path)
{
	QList<QString> path_items = filename_with_path.split('/');
	return path_items.takeLast();
}
