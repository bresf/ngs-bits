#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "Exceptions.h"
#include "WebEntity.h"
#include "FileCache.h"
#include "NGSD.h"
#include "SessionManager.h"
#include "EndpointManager.h"

#include "GlobalServiceProvider.h"
#include "FileLocationProviderFileSystem.h"
#include "VariantList.h"

class WorkerThread : public QThread
{
	Q_OBJECT
public:
	explicit WorkerThread(Request request);
	void run();

private:
	bool isEligibileToAccess();
	QString getFileNameWithExtension(QString filename_with_path);
	QByteArray readFileContent(QString filename);
	Response serveStaticFile(QString filename, WebEntity::ContentType type, bool is_downloadable);
	Response serveFolderContent(QString folder);
	QByteArray generateHeaders(QString filename, int length, WebEntity::ContentType type, bool is_downloadable);
	QByteArray generateHeaders(int length, WebEntity::ContentType type);
	bool isValidUser(QString name, QString password);
	void processRequest();
	Request request_;

signals:
	void resultReady(const Response &response);
};

#endif // WORKERTHREAD_H
