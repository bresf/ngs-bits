#ifndef FILECACHE_H
#define FILECACHE_H

#include <QMap>
#include <QDebug>
#include <QMutex>

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
	static QString getFileIdIfInCache(QString filename_with_path);
	static bool isInCacheAlready(QString filename_with_path);
	static CacheItem getFileById(QString id);

private:
	QMutex mutex_;
	FileCache();
	static FileCache& instance();
	QMap<QString, CacheItem> file_cache_;
};

#endif // FILECACHE_H
