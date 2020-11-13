#include "Helper.h"

QString Helper::getAppName()
{
	return QCoreApplication::applicationName();
}

QString Helper::getApplicationBaseName()
{
	return QCoreApplication::applicationDirPath() + QDir::separator() + QCoreApplication::applicationName();
}

Helper& Helper::instance()
{
	static Helper helper;
	return helper;
}
