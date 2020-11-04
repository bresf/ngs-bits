#ifndef FILECACHE_H
#define FILECACHE_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QDebug>

QMutex mutex;

struct CacheItem
{
	QString filename_with_path;
	QByteArray content;
};

class FileCache
{
public:	
	static void addFileToCache(QString id, QString filename_with_path, QByteArray content);
	static void removeFileFromCache(QString id);
	static bool isInCacheAlready(QString filename_with_path);
	static CacheItem getFileById(QString id);

private:
	FileCache();
	static FileCache& instance();
	QMap<QString, CacheItem> file_cache_;
};

#endif // FILECACHE_H
