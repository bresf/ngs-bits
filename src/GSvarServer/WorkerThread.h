#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QFile>
#include <QDebug>
#include "Exceptions.h"
#include "WebEntity.h"

class WorkerThread : public QThread
{
	Q_OBJECT
public:
	explicit WorkerThread(Request request);
	void run();

private:
	QByteArray readFileContent(QString filename);
	QByteArray generateHeaders(int length, WebEntity::ContentType type);
	Request request_;

signals:
	void resultReady(const QByteArray &headers, const QByteArray &body);
};

#endif // WORKERTHREAD_H
