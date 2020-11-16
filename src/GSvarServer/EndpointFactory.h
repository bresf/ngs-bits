#ifndef ENDPOINTFACTORY_H
#define ENDPOINTFACTORY_H

#include <QDebug>
#include <QFile>
#include "Helper.h"
#include "WebEntity.h"
#include "WebExceptions.h"


struct Endpoint
{
	enum ParamType
	{
		INTEGER,
		STRING,
		ENUM,
		UNKNOWN
	};
	QString url;
	QMap<QString, ParamType> params;
	Request::MethodType method;
	WebEntity::ContentType return_type;
	QString comment;
};

class EndpointFactory
{

public:
	static Endpoint::ParamType getEndpointParamTypeFromString(QString in);
	static void validateInputData(Request request);

private:
	EndpointFactory();
	static EndpointFactory& instance();
	static QList<Endpoint> readEndpointConfig();
	static bool isParamTypeValid(QString param, Endpoint::ParamType type);
	static Endpoint getEndpointByUrl(QString url);
	QList<Endpoint> endpoint_list_;
};

#endif // ENDPOINTFACTORY_H
