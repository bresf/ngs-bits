#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QFile>
#include <QDebug>
#include "Exceptions.h"
#include "WebEntity.h"
#include "FileCache.h"

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
	QString getUrlPartWithoutParams(QByteArray url);
	Request request_;

signals:
	void resultReady(const Response &response);
};

#endif // WORKERTHREAD_H
