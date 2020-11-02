#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>

class WorkerThread : public QThread
{
	Q_OBJECT
public:
	explicit WorkerThread(QObject *parent);
	void run();
signals:
	void resultReady(const QString &s);
};

#endif // WORKERTHREAD_H
