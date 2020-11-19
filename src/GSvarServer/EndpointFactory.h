#ifndef ENDPOINTFACTORY_H
#define ENDPOINTFACTORY_H

#include <QDebug>
#include <QFile>
#include "ServerHelper.h"
#include "WebEntity.h"
#include "WebExceptions.h"

struct ParamProps
{
	enum ParamType
	{
		INTEGER,
		STRING,
		ENUM,
		UNKNOWN
	};
	enum ParamCategory
	{
		PATH_PARAM, // http://url/{param}
		GET_URL_VAR, // http://url?var=val
		POST_URL_ENCODED, // application/x-www-form-urlencoded
		POST_FORM_DATA // multipart/form-data
	};

	ParamType type;
	ParamCategory category;
	bool is_optional;
};

struct Endpoint
{
	QString url;
	QMap<QString, ParamProps> params;
	Request::MethodType method;
	WebEntity::ContentType return_type;
	QString comment;
};

class EndpointFactory
{

public:
	static ParamProps::ParamType getParamTypeFromString(QString in);
	static void validateInputData(Request request);
	static QString generateGlobalHelp();
	static QString generateEntityHelp(QString url);

private:
	EndpointFactory();
	static EndpointFactory& instance();
	static QList<Endpoint> readEndpointConfig();
	static bool isParamTypeValid(QString param, ParamProps::ParamType type);
	static Endpoint getEndpointByUrl(QString url);
	static QString getEndpointHelpTemplate(QList<Endpoint> *endpoint_list);
	QList<Endpoint> endpoint_list_;
};

#endif // ENDPOINTFACTORY_H
