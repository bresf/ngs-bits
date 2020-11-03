#ifndef FILECACHE_H
#define FILECACHE_H

#include <QObject>
#include <QMutex>
#include <QMap>

QMutex mutex;

struct CacheItem
{
	QString filename_with_path;
	QByteArray content;
};

class FileCache : public QObject
{
	Q_OBJECT

public:
	FileCache();
	~FileCache();

	void addFileToCache(QString id, QString filename_with_path, QByteArray content);
	void removeFileFromCache(QString id);
	bool isInCacheAlready(QString filename_with_path);
	CacheItem getFileById(QString id);

private:
	QMap<QString, CacheItem> file_cache_;
};

#endif // FILECACHE_H
