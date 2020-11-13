#ifndef ENDPOINTFACTORY_H
#define ENDPOINTFACTORY_H

#include <QDebug>
#include <QFile>
#include "Helper.h"
#include "WebEntity.h"

struct Endpoint
{
	QString url;
	QList<QString> params;
	QList<QString> param_types;
	Request::MethodType method;
	WebEntity::ContentType return_type;
	QString comment;
};

class EndpointFactory
{

public:
	static Response processRequestData(Request request);

private:
	EndpointFactory();
	static EndpointFactory& instance();
	static QList<Endpoint> readEndpointConfig();
	QList<Endpoint> endpoint_list_;
};

#endif // ENDPOINTFACTORY_H
