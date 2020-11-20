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
	int path_param_count = 0;
	QMap<QString, ParamProps>::const_iterator i = endpoint.params.constBegin();
	while (i != endpoint.params.constEnd())
	{
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
				if (!isParamTypeValid(request.url_params[i.key()], endpoint.params[i.key()].type))
				{
					THROW(ArgumentException, i.key() + " parameter inside URL has an invalid type");
				}
			}
		}

		if(i.value().category == ParamProps::ParamCategory::PATH_PARAM)
		{
			path_param_count++;
		}
		else if ((!i.value().is_optional) && (!is_found))
		{
			THROW(ArgumentException, "Parameter " + i.key() + " is missing");
		}
		++i;
	}

	if (request.path_params.count() != path_param_count)
	{
		THROW(ArgumentException, "Path parameters number mismatch");
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
					   QMap<QString, ParamProps>({
						   {"filename", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::PATH_PARAM, false}}
					   }),
					   Request::MethodType::GET,
					   WebEntity::ContentType::TEXT_HTML,
					   "Static content served from the server root folder (defined in the config file)"
					});
	appendEndpoint(Endpoint{
					   "login",
					   QMap<QString, ParamProps>({
							{"name", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::POST_URL_ENCODED, false}},
							{"password", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::POST_URL_ENCODED, false}}
					   }),
					   Request::MethodType::POST,
					   WebEntity::ContentType::TEXT_PLAIN,
					   "Secure token generation, the token will be used to access protected resources and to perform  certain API calls"
					});
	appendEndpoint(Endpoint{
					   "logout",
					   QMap<QString, ParamProps>({
							{"token", ParamProps{ParamProps::ParamType::STRING, ParamProps::ParamCategory::POST_URL_ENCODED, false}}
					   }),
					   Request::MethodType::POST,
					   WebEntity::ContentType::TEXT_PLAIN,
					   "Secure token invalidation, after this step the token cannot longer be used"
					});
}

QString EndpointManager::generateGlobalHelp()
{
	return getEndpointHelpTemplate(&instance().endpoint_registry_);
}

QString EndpointManager::generateEntityHelp(QString url)
{
	QList<Endpoint> selected_endpoints {};
	selected_endpoints.append(getEndpointByUrl(url));
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
	stream << "			<h1>API Help Page</h1>\n";
	stream << "			<div class=\"row\">\n";
	stream << "				<div class=\"column-25\"><b>URL</b></div>\n";
	stream << "				<div class=\"column-25\"><b>Method</b></div>\n";
	stream << "				<div class=\"column-25\"><b>Parameters</b></div>\n";
	stream << "				<div class=\"column-25\"><b>Description</b></div>\n";
	stream << "			</div>\n";


	for (int i = 0; i < endpoint_list->count(); ++i)
	{
		stream << "			<div class=\"row\">\n";
		stream << "				<div class=\"column-25\">" << (*endpoint_list)[i].url << "</div>\n";
		stream << "				<div class=\"column-25\">" << WebEntity::convertMethodTypeToString((*endpoint_list)[i].method).toUpper() << "</div>\n";
		stream << "				<div class=\"column-25\"></div>\n";
		stream << "				<div class=\"column-25\">" << (*endpoint_list)[i].comment << "</div>\n";
		stream << "			</div>\n";
	}

	stream << WebEntity::getPageFooter();

	return output;
}
