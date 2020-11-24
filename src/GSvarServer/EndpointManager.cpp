#include "EndpointManager.h"

EndpointManager::EndpointManager()
{
}

ParamProps::ParamType EndpointManager::getParamTypeFromString(QString in)
{
	if (in.toLower() == "string") return ParamProps::ParamType::STRING;
	if ((in.toLower() == "int") || (in.toLower() == "integer")) return ParamProps::ParamType::INTEGER;

	return ParamProps::ParamType::UNKNOWN;
}

void EndpointManager::validateInputData(Request request)
{	
	if (instance().endpoint_registry_.count() == 0)
	{
		instance().initialize();
	}

	Endpoint endpoint = getEndpointByUrl(request.path.trimmed().replace("/", ""));
	QMapIterator<QString, ParamProps> i(endpoint.params);
	while (i.hasNext()) {
		i.next();

		bool is_found = false;

		if(i.value().category == ParamProps::ParamCategory::POST_URL_ENCODED)
		{
			if (request.form_urlencoded.contains(i.key()))
			{
				is_found = true;
				if (!isParamTypeValid(request.form_urlencoded[i.key()], endpoint.params[i.key()].type))
				{
					THROW(ArgumentException, i.key() + " x-www-form-urlencoded parameter has an invalid type");
				}
			}
		}

		if(i.value().category == ParamProps::ParamCategory::GET_URL_PARAM)
		{
			if (request.url_params.contains(i.key()))
			{
				is_found = true;
				qDebug() << request.url_params[i.key()] << "," << endpoint.params[i.key()].type;
				if (!isParamTypeValid(request.url_params[i.key()], endpoint.params[i.key()].type))
				{
					THROW(ArgumentException, i.key() + " parameter inside URL has an invalid type");
				}
			}
		}

		qDebug() << "var:" << i.key();
		if ((!i.value().is_optional) && (!is_found))
		{
			THROW(ArgumentException, "Parameter " + i.key() + " is missing");
		}

	}
}

void EndpointManager::appendEndpoint(Endpoint new_endpoint)
{
	if (!instance().endpoint_registry_.contains(new_endpoint))
	{
		instance().endpoint_registry_.append(new_endpoint);
	}
}

void EndpointManager::initialize()
{
	appendEndpoint(Endpoint{"", QMap<QString, ParamProps>{}, Request::MethodType::GET, WebEntity::ContentType::TEXT_HTML, "Index page with general information"});
	appendEndpoint(Endpoint{"info", QMap<QString, ParamProps>{}, Request::MethodType::GET, WebEntity::ContentType::APPLICATION_JSON, "General information about this API"});
	appendEndpoint(Endpoint{
					   "static",
					   QMap<QString, ParamProps>{
						   {"filename", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::PATH_PARAM, false}}
					   },
					   Request::MethodType::GET,
					   WebEntity::ContentType::TEXT_HTML,
					   "Static content served from the server root folder (defined in the config file)"
					});

	appendEndpoint(Endpoint{
					   "help",
					   QMap<QString, ParamProps>{
						   {"endpoint", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::PATH_PARAM, true}}
					   },
					   Request::MethodType::GET,
					   WebEntity::ContentType::TEXT_HTML,
					   "Static content served from the server root folder (defined in the config file)"
					});

	appendEndpoint(Endpoint{
					   "file_location",
					   QMap<QString, ParamProps> {
						   {"ps", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::GET_URL_PARAM, false}},
						   {"type", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::GET_URL_PARAM, false}}
					   },
					   Request::MethodType::GET,
					   WebEntity::ContentType::TEXT_HTML,
					   "Retrieve file location information for scecific file types"
					});

	appendEndpoint(Endpoint{
					   "login",
					   QMap<QString, ParamProps>{
							{"name", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::POST_URL_ENCODED, false}},
							{"password", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::POST_URL_ENCODED, false}}
					   },
					   Request::MethodType::POST,
					   WebEntity::ContentType::TEXT_PLAIN,
					   "Secure token generation, the token will be used to access protected resources and to perform  certain API calls"
					});
	appendEndpoint(Endpoint{
					   "logout",
					   QMap<QString, ParamProps>{
							{"token", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::POST_URL_ENCODED, false}}
					   },
					   Request::MethodType::POST,
					   WebEntity::ContentType::TEXT_PLAIN,
					   "Secure token invalidation, after this step the token cannot longer be used"
					});
}

QString EndpointManager::generateGlobalHelp()
{
	return getEndpointHelpTemplate(&instance().endpoint_registry_);
}

QString EndpointManager::generateEntityHelp(QString path)
{
	QList<Endpoint> selected_endpoints {};
	selected_endpoints.append(getEndpointByUrl(path));
	return getEndpointHelpTemplate(&selected_endpoints);
}

EndpointManager& EndpointManager::instance()
{
	static EndpointManager endpoint_factory;
	return endpoint_factory;
}

bool EndpointManager::isParamTypeValid(QString param, ParamProps::ParamType type)
{
	switch (type)
	{
		case ParamProps::ParamType::STRING: return true;
		case ParamProps::ParamType::INTEGER: return ServerHelper::canConvertToInt(param);

		default: return false;
	}
}

Endpoint EndpointManager::getEndpointByUrl(QString url)
{
	for (int i = 0; i < instance().endpoint_registry_.count(); ++i)
	{
		if (instance().endpoint_registry_[i].url.toLower() == url.toLower())
			return instance().endpoint_registry_[i];


		qDebug() << instance().endpoint_registry_[i].url;
	}

	return Endpoint{};
}

QString EndpointManager::getEndpointHelpTemplate(QList<Endpoint> *endpoint_list)
{
	QString output;
	QTextStream stream(&output);

	stream << WebEntity::getPageHeader();
	stream << WebEntity::getApiHelpHeader("API Help Page");

	for (int i = 0; i < endpoint_list->count(); ++i)
	{
		stream << WebEntity::getApiHelpEntiry(
					  (*endpoint_list)[i].url,
					  WebEntity::convertMethodTypeToString((*endpoint_list)[i].method),
					  QList<QString>{},
					  (*endpoint_list)[i].comment
					);
	}

	stream << WebEntity::getPageFooter();

	return output;
}