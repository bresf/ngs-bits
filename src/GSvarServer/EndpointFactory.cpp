#include "EndpointFactory.h"

EndpointFactory::EndpointFactory()
{
	instance().endpoint_list_ = readEndpointConfig();
}

EndpointFactory& EndpointFactory::instance()
{
	static EndpointFactory endpoint_factory;
	qDebug() << "Creating an EndpointFactory instance";
	return endpoint_factory;
}

QList<Endpoint> EndpointFactory::readEndpointConfig()
{
	QFile file(Helper::getApplicationBaseName() + "_endpoints.ini");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	   return;

	QTextStream in(&file);
	while (!in.atEnd()) {
	   QString line = in.readLine();
	   process_line(line);
	}
}
