#ifndef SERVERHELPER_H
#define SERVERHELPER_H

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QUuid>
#include "Exceptions.h"

class ServerHelper
{
public:
	static QString getAppName();
	static QString getAppBaseName();
	static int strToInt(QString in);
	static bool canConvertToInt(QString in);
	static QString generateUniqueStr();

private:
	ServerHelper();
	static ServerHelper& instance();

};

#endif // SERVERHELPER_H
