#ifndef ENDPOINTMANAGER_H
#define ENDPOINTMANAGER_H

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

	bool operator==(const ParamProps& p) const
	{
		return type==p.type && category==p.category && is_optional==p.is_optional;
	}
};

struct Endpoint
{
	QString url;
	QMap<QString, ParamProps> params;
	Request::MethodType method;
	WebEntity::ContentType return_type;
	QString comment;

	bool operator==(const Endpoint& e) const
	{
		return url==e.url && params==e.params && method==e.method && return_type==e.return_type;
	}
};

class EndpointManager
{

public:
	static ParamProps::ParamType getParamTypeFromString(QString in);	
	static void validateInputData(Request request);
	static void appendEndpoint(Endpoint new_endpoint);
	static void initialize();
	static QString generateGlobalHelp();
	static QString generateEntityHelp(QString url);

private:
	EndpointManager();
	static EndpointManager& instance();
	static bool isParamTypeValid(QString param, ParamProps::ParamType type);
	static Endpoint getEndpointByUrl(QString url);
	static QString getEndpointHelpTemplate(QList<Endpoint> *endpoint_list);
	QList<Endpoint> endpoint_registry_;
};

#endif // ENDPOINTMANAGER_H
