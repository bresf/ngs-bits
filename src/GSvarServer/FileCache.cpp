#include "FileCache.h"


FileCache::FileCache()
	: file_cache_({})
{
}

void FileCache::addFileToCache(QString id, QString filename_with_path, QByteArray content)
{
	file_cache_.insert(id, CacheItem{filename_with_path, content});
}

void FileCache::removeFileFromCache(QString id)
{
	if (file_cache_.contains(id))
	{
		file_cache_.remove(id);
	}
}

bool FileCache::isInCacheAlready(QString filename_with_path)
{
	bool found = false;



	return found;
}

CacheItem FileCache::getFileById(QString id)
{
	if (file_cache_.contains(id))
	{
		return file_cache_[id];
	}
	return CacheItem{};
}
