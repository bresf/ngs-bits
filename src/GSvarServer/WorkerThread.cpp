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
	FileCache::addFileToCache(ServerHelper::generateUniqueStr(), filename_with_path, content);
	return content;
}

Response WorkerThread::serveStaticFile(QString filename_with_path, WebEntity::ContentType type, bool is_downloadable)
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

	return (Response{generateHeaders(getFileNameWithExtension(filename_with_path), body.length(), type, is_downloadable), body});
}

Response WorkerThread::serveFolderContent(QString folder)
{
	QDir dir(folder);
	if (!dir.exists())
	{
		return WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, "Requested folder does not exist");
	}

	dir.setFilter(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks);

	QFileInfoList list = dir.entryInfoList();
	QList<FolderItem> files {};
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if ((fileInfo.fileName() == ".") || (fileInfo.fileName() == "..")) continue;

		FolderItem current_item;
		current_item.name = fileInfo.fileName();
		current_item.size = fileInfo.size();
		current_item.modified = fileInfo.metadataChangeTime();
		current_item.is_folder = fileInfo.isDir() ? true : false;
		files.append(current_item);
		qDebug() << "File:" << fileInfo.fileName() << ", " << fileInfo.size() << fileInfo.isDir();

	}
	return WebEntity::cretateFolderListing(files);
}

QByteArray WorkerThread::generateHeaders(QString filename, int length, WebEntity::ContentType type, bool is_downloadable)
{
	QByteArray headers {};
	headers.append("HTTP/1.1 200 OK\n");
	headers.append("Connection: Keep-Alive\n");
	headers.append("Keep-Alive: timeout=5, max=1000\n");
	headers.append("Content-Length: " + QString::number(length) + "\n");
	headers.append("Content-Type: " + WebEntity::convertContentTypeToString(type) + "\n");
	if (is_downloadable)
	{
		headers.append("Content-Disposition: form-data; name=file_download; filename=" + filename + "\n");
	}

	headers.append("\n");

	return headers;
}

QByteArray WorkerThread::generateHeaders(int length, WebEntity::ContentType type)
{
	return generateHeaders("", length, type, false);
}

bool WorkerThread::isValidUser(QString user_id, QString password)
{
	try
	{
		NGSD db;
		QString message = db.checkPassword(user_id, password, true);
		if (message.isEmpty())
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	catch (DatabaseException& e)
	{
		qDebug() << e.message();
		emit resultReady(WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, e.message()));
	}
	return false;
}

QString WorkerThread::getFileNameWithExtension(QString filename_with_path)
{
	QList<QString> path_items = filename_with_path.split('/');
	return path_items.takeLast();
}

bool WorkerThread::isEligibileToAccess()
{
	if ((!request_.form_urlencoded.contains("token")) && (!request_.url_params.contains("token")))
	{
		emit resultReady(WebEntity::createError(WebEntity::FORBIDDEN, "Secure token has not been provided"));
		return false;
	}
	if ((!SessionManager::isTokenValid(request_.form_urlencoded["token"])) && (!SessionManager::isTokenValid(request_.url_params["token"])))
	{
		emit resultReady(WebEntity::createError(WebEntity::UNAUTHORIZED, "Secure token is invalid"));
		return false;
	}
	return true;
}

void WorkerThread::processRequest()
{
	QByteArray body {};

	try
	{
		EndpointManager::validateInputData(request_);
	}
	catch (ArgumentException& e)
	{
		emit resultReady(WebEntity::createError(WebEntity::BAD_REQUEST, e.message()));
		return;
	}

	// index page
	if ((request_.path == "") && request_.method == Request::MethodType::GET)
	{
		emit resultReady(serveStaticFile(":/assets/client/info.html", WebEntity::TEXT_HTML, false));
		return;
	}

	// api info page (e.g. version info)
	if ((request_.path == "info") && request_.method == Request::MethodType::GET)
	{
		emit resultReady(serveStaticFile(":/assets/client/api.json", WebEntity::APPLICATION_JSON, false));
		return;
	}

	if ((request_.path == "folder") && request_.method == Request::MethodType::GET)
	{
		emit resultReady(serveFolderContent("./"));
		return;
	}

	if ((request_.path == "static") && request_.method == Request::MethodType::GET)
	{
		qDebug() << "Accessing static content";
		QString path = Settings::string("server_root");
		path = WebEntity::getUrlWithoutParams(path.trimmed() + request_.path_params[0]);
		emit resultReady(serveStaticFile(path, WebEntity::getContentTypeByFilename(path), false));
		return;
	}


	if ((request_.path == "file_location") && request_.method == Request::MethodType::GET)
	{
		qDebug() << "File location service";

		QString filename = "Sample_NA12878_01/example_output/NA12878_01.GSvar";
		VariantList variants {};
		variants.load(filename);
		GlobalServiceProvider::instance().setfileLocationsProvider(QSharedPointer<FileLocationProviderFileSystem>(new FileLocationProviderFileSystem(filename, variants.getSampleHeader(), variants.type())));

		QList<FileLocation> file_list = GlobalServiceProvider::instance().fileLocationsProvider()->getBamFiles();

		QJsonDocument json_doc_output {};
		QJsonArray json_list_output {};
		QJsonObject json_obj_output {};

		for (int i = 0; i < file_list.count(); ++i)
		{
			qDebug() << file_list[i].filename;
			json_list_output.append(file_list[i].filename);
		}
		json_doc_output.setArray(json_list_output);

		emit resultReady(Response{generateHeaders(json_doc_output.toJson().length(), WebEntity::APPLICATION_JSON), json_doc_output.toJson()});
		return;
	}



	if ((request_.path == "help") && request_.method == Request::MethodType::GET)
	{
		if (request_.path_params.count() == 0)
		{
			body = EndpointManager::generateGlobalHelp().toLocal8Bit();
		}
		else
		{
			body = EndpointManager::generateEntityHelp(request_.path_params[0]).toLocal8Bit();
		}
		emit resultReady(Response{generateHeaders(body.length(), WebEntity::TEXT_HTML), body});
		return;
	}

	if ((request_.path == "file") && (request_.method == Request::MethodType::GET))
	{
		if (!isEligibileToAccess()) return;

		emit resultReady(serveStaticFile(":/assets/client/example.png", WebEntity::APPLICATION_OCTET_STREAM, true));
		return;
	}

	if ((request_.path == "login") && request_.method == Request::MethodType::POST)
	{
		if (!request_.form_urlencoded.contains("name") || !request_.form_urlencoded.contains("password"))
		{
			emit resultReady(WebEntity::createError(WebEntity::FORBIDDEN, "No username or/and password were found"));
			return;
		}

		if (isValidUser(request_.form_urlencoded["name"], request_.form_urlencoded["password"]))
		{
			QString secure_token = ServerHelper::generateUniqueStr();
			Session cur_session = Session{request_.form_urlencoded["name"], QDateTime::currentDateTime()};

			SessionManager::addNewSession(secure_token, cur_session);
			body = secure_token.toLocal8Bit();
			emit resultReady(Response{generateHeaders(body.length(), WebEntity::TEXT_PLAIN), body});
			return;
		}

		emit resultReady(WebEntity::createError(WebEntity::UNAUTHORIZED, "Invalid username or password"));
		return;
	}

	if ((request_.path == "logout") && request_.method == Request::MethodType::POST)
	{
		if (!request_.form_urlencoded.contains("token"))
		{
			emit resultReady(WebEntity::createError(WebEntity::FORBIDDEN, "Secure token has not been provided"));
			return;
		}
		if (SessionManager::isTokenValid(request_.form_urlencoded["token"]))
		{
			try
			{
				SessionManager::removeSession(request_.form_urlencoded["token"]);
			} catch (Exception& e)
			{
				emit resultReady(WebEntity::createError(WebEntity::INTERNAL_SERVER_ERROR, e.message()));
				return;
			}
			body = request_.form_urlencoded["token"].toLocal8Bit();
			emit resultReady(Response{generateHeaders(body.length(), WebEntity::TEXT_PLAIN), body});
			return;
		}
		emit resultReady(WebEntity::createError(WebEntity::FORBIDDEN, "You have provided an invalid token"));
		return;

	}

	emit resultReady(WebEntity::createError(WebEntity::NOT_FOUND, "This page does not exist. Check the URL and try again"));
}

void WorkerThread::run()
{
	qDebug() << "Processing path:" << request_.path;
	processRequest();
}
