#include "FileCache.h"

FileCache::FileCache()
	: file_cache_()
{
}

FileCache& FileCache::instance()
{
	static FileCache file_cache;
	return file_cache;
}

void FileCache::addFileToCache(QString id, QString filename_with_path, QByteArray content)
{
	instance().file_cache_.insert(id, CacheItem{filename_with_path, content});
}

void FileCache::removeFileFromCache(QString id)
{
	if (instance().file_cache_.contains(id))
	{
		instance().file_cache_.remove(id);
	}
}

bool FileCache::isInCacheAlready(QString filename_with_path)
{
	QMapIterator<QString, CacheItem> i(instance().file_cache_);
	while (i.hasNext()) {
		i.next();
		qDebug() << i.key() << ": " << i.value().filename_with_path;
		if (i.value().filename_with_path == filename_with_path)
		{
			return true;
		}
	}

	return false;
}

CacheItem FileCache::getFileById(QString id)
{
	if (instance().file_cache_.contains(id))
	{
		return instance().file_cache_[id];
	}
	return CacheItem{};
}
