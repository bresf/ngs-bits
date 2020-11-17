#include "EndpointFactory.h"

EndpointFactory::EndpointFactory()
{
}

Endpoint::ParamType EndpointFactory::getEndpointParamTypeFromString(QString in)
{
	if (in.toLower() == "string") return Endpoint::ParamType::STRING;
	if ((in.toLower() == "int") || (in.toLower() == "integer")) return Endpoint::ParamType::INTEGER;

	return Endpoint::ParamType::UNKNOWN;
}

void EndpointFactory::validateInputData(Request request)
{
	if (instance().endpoint_list_.count() == 0)
	{
		instance().endpoint_list_ = readEndpointConfig();
	}

	Endpoint endpoint = getEndpointByUrl(request.path.trimmed());

	QMap<QString, QString> params {}; // for now we can only have ether url or post params
	if (request.method == Request::MethodType::POST)
	{
		params = request.form_urlencoded;
	}
	else
	{
		params = request.url_params;
	}

	if (params.count() != endpoint.params.count())
	{
		THROW(ArgumentException, "Parameter number mismatch");
	}

	QMap<QString, QString>::const_iterator i = params.constBegin();
	while (i != params.constEnd())
	{
		if (isParamTypeValid(i.value(), endpoint.params[i.key()]))
		{
			THROW(ArgumentException, "Parameter " + i.key() + " has an invalid type");
		}
		++i;
	}
}

QString EndpointFactory::generateGlobalHelp()
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


	for (int i = 0; i < instance().endpoint_list_.count(); ++i)
	{
		stream << "			<div class=\"row\">\n";
		stream << "				<div class=\"column-25\">" << instance().endpoint_list_[i].url << "</div>\n";
		stream << "				<div class=\"column-25\">" << WebEntity::convertMethodTypeToString(instance().endpoint_list_[i].method).toUpper() << "</div>\n";
		stream << "				<div class=\"column-25\"></div>\n";
		stream << "				<div class=\"column-25\">" << instance().endpoint_list_[i].comment << "</div>\n";
		stream << "			</div>\n";
	}

	stream << WebEntity::getPageFooter();

	return output;
}

QString EndpointFactory::generateEntityHelp(QString endpoint_url)
{
	return "";
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

	QFile file(Helper::getAppBaseName() + "_endpoints.ini");
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
				current_endpoint.params.insert(param_list[i], getEndpointParamTypeFromString(type_list[i]));
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

bool EndpointFactory::isParamTypeValid(QString param, Endpoint::ParamType type)
{
	switch (type)
	{
		case Endpoint::ParamType::STRING: return true;
		case Endpoint::ParamType::INTEGER: return Helper::canConvertToInt(param);

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
