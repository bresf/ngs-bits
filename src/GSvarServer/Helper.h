#ifndef HELPER_H
#define HELPER_H

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

class Helper
{
public:
	static QString getAppName();

private:
	Helper();
	static Helper& instance();

};

#endif // HELPER_H
