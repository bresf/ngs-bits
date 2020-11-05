#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QFile>
#include <QDebug>
#include "Exceptions.h"
#include "WebEntity.h"
#include "FileCache.h"
#include "NGSD.h"

class WorkerThread : public QThread
{
	Q_OBJECT
public:
	explicit WorkerThread(Request request);
	void run();

private:
	QString getFileNameAndExtension(QString filename_with_path);
	QByteArray readFileContent(QString filename);
	Response serverStaticFile(QString filename, WebEntity::ContentType type, bool is_downloadable);
	QByteArray generateHeaders(QString filename, int length, WebEntity::ContentType type, bool is_downloadable);
	QByteArray generateHeaders(int length, WebEntity::ContentType type);
	QString getUrlPartWithoutParams(QByteArray url);
	bool isValidUser(QString name, QString password);
	Request request_;

signals:
	void resultReady(const Response &response);
};

#endif // WORKERTHREAD_H
