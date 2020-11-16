#include "Helper.h"

Helper::Helper()
{
}

QString Helper::getAppName()
{
	return QCoreApplication::applicationName();
}

QString Helper::getAppBaseName()
{
	return QCoreApplication::applicationDirPath() + QDir::separator() + QCoreApplication::applicationName();
}

int Helper::strToInt(QString in)
{
	bool ok;
	int dec = in.toInt(&ok, 10);
	if (!ok) THROW(ArgumentException, "Could not convert string to integer");

	return dec;
}

bool Helper::canConvertToInt(QString in)
{
	bool ok;
	in.toInt(&ok, 10);
	return ok;
}

Helper& Helper::instance()
{
	static Helper helper;
	return helper;
}
