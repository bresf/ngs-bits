#ifndef HELPER_H
#define HELPER_H

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include "Exceptions.h"

class Helper
{
public:
	static QString getAppName();
	static QString getAppBaseName();
	static int strToInt(QString in);
	static bool canConvertToInt(QString in);

private:
	Helper();
	static Helper& instance();

};

#endif // HELPER_H
