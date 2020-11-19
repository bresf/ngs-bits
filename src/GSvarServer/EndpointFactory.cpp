#include "EndpointFactory.h"

EndpointFactory::EndpointFactory()
{
}

ParamProps::ParamType EndpointFactory::getParamTypeFromString(QString in)
{
	if (in.toLower() == "string") return ParamProps::ParamType::STRING;
	if ((in.toLower() == "int") || (in.toLower() == "integer")) return ParamProps::ParamType::INTEGER;

	return ParamProps::ParamType::UNKNOWN;
}

void EndpointFactory::validateInputData(Request request)
{
	if (instance().endpoint_list_.count() == 0)
	{
		instance().endpoint_list_ = readEndpointConfig();
	}

	Endpoint endpoint = getEndpointByUrl(request.path.trimmed().replace("/", ""));

	QMap<QString, QString> params {}; // for now we can only have ether url or post params
	if (request.method == Request::MethodType::POST)
	{
		params = request.form_urlencoded;
	}
	else
	{
		params = request.url_params;
	}

	qDebug() << request.path.trimmed();
	qDebug() << params.count();
	qDebug() << endpoint.params.count();

	if (params.count() != endpoint.params.count())
	{
		THROW(ArgumentException, "Parameter number mismatch");
	}

	QMap<QString, QString>::const_iterator i = params.constBegin();
	while (i != params.constEnd())
	{
		if (!isParamTypeValid(i.value(), endpoint.params[i.key()].type))
		{
			THROW(ArgumentException, "Parameter " + i.key() + " has an invalid type");
		}
		++i;
	}
}

QString EndpointFactory::generateGlobalHelp()
{
	return getEndpointHelpTemplate(&instance().endpoint_list_);
}

QString EndpointFactory::generateEntityHelp(QString url)
{
	QList<Endpoint> selected_endpoints {};
	selected_endpoints.append(getEndpointByUrl(url));
	return getEndpointHelpTemplate(&selected_endpoints);
}

EndpointFactory& EndpointFactory::instance()
{
	static EndpointFactory endpoint_factory;
	return endpoint_factory;
}

QList<Endpoint> EndpointFactory::readEndpointConfig()
{
	qDebug() << "Reading the list of endpoints";
	QList<Endpoint> endpoints {};

	QFile file(ServerHelper::getAppBaseName() + "_endpoints.ini");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		THROW(FileAccessException, "Cannot get endpoint information");

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();
		QList<QString> line_items = line.split("\t");
		if (line_items.count() == 6)
		{
			Endpoint current_endpoint {};
			current_endpoint.url = line_items[0];

			QList<QString> param_list = line_items[1].split(";");
			QList<QString> type_list = line_items[2].split(";");
			if (param_list.count() != type_list.count())
			{
				THROW(ArgumentException, "Parameter or parameter type has not been provided in the config");
			}

			for (int i = 0; i < param_list.count(); ++i)
			{
				current_endpoint.params.insert(param_list[i], ParamProps{getParamTypeFromString(type_list[i]), ParamProps::GET_URL_VAR, false});
			}

			current_endpoint.method = WebEntity::getMethodTypeFromString(line_items[3]);
			current_endpoint.return_type = WebEntity::getContentTypeFromString(line_items[4]);
			current_endpoint.comment = line_items[5];
			endpoints.append(current_endpoint);
		}
		qDebug() << line;
	}

	return endpoints;
}

bool EndpointFactory::isParamTypeValid(QString param, ParamProps::ParamType type)
{
	switch (type)
	{
		case ParamProps::ParamType::STRING: return true;
		case ParamProps::ParamType::INTEGER: return ServerHelper::canConvertToInt(param);

		default: return false;
	}
}

Endpoint EndpointFactory::getEndpointByUrl(QString url)
{
	for (int i = 0; i < instance().endpoint_list_.count(); ++i)
	{
		if (instance().endpoint_list_[i].url.toLower() == url.toLower())
			return instance().endpoint_list_[i];


		qDebug() << instance().endpoint_list_[i].url;
	}

	return Endpoint{};
}

QString EndpointFactory::getEndpointHelpTemplate(QList<Endpoint> *endpoint_list)
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
