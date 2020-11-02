#include "WorkerThread.h"
#include <QDebug>

WorkerThread::WorkerThread(QObject* parent)
{

}

void WorkerThread::run()
{

	QString result;

	qDebug() << "RUN";
	result = "text";
	/* ... here is the expensive or blocking operation ... */
	emit resultReady(result);
}
