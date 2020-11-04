#include "WebEntity.h"

WebEntity::WebEntity()
{
}

WebEntity& WebEntity::instance()
{
	static WebEntity web_entity;
	return web_entity;
}

QString WebEntity::contentTypeToString(WebEntity::ContentType in)
{
	switch(in)
	{
		case APPLICATION_OCTET_STREAM: return "application/octet-stream";
		case APPLICATION_JSON: return "application/json";
		case IMAGE_JPEG: return "image/jpeg";
		case IMAGE_PNG: return "image/png";
		case IMAGE_SVG_XML: return "image/svg+xml";
		case TEXT_PLAIN: return "text/plain";
		case TEXT_CSV: return "text/csv";
		case TEXT_HTML: return "text/html";
		case MULTIPART_FORM_DATA: return "multipart/form-data";
	}
	return "";
}

QString WebEntity::errorTypeToText(WebEntity::ErrorType in)
{
	switch(in)
	{
		case BAD_REQUEST: return "Bad Request";
		case UNAUTHORIZED: return "Unauthorized";
		case PAYMENT_REQUIRED: return "Payment Required";
		case FORBIDDEN: return "Forbidden";
		case NOT_FOUND: return "Not Found";
		case METHOD_NOT_ALLOWED: return "Method Not Allowed";
		case NOT_ACCEPTABLE: return "Not Acceptable";
		case PROXY_AUTH_REQUIRED: return "Proxy Authentication Required";
		case REQUEST_TIMEOUT: return "Request Timeout";
		case CONFLICT: return "Conflict";
		case GONE: return "Gone";
		case LENGTH_REQUIRED: return "Length Required";
		case PRECONDITION_FAILED: return "Precondition Failed";
		case ENTITY_TOO_LARGE: return "Request Entity Too Large";
		case URI_TOO_LONG: return "Request-URI Too Long";
		case UNSUPPORTED_MEDIA_TYPE: return "Unsupported Media Type";
		case RANGE_NOT_SATISFIABLE: return "Requested Range Not Satisfiable";
		case EXPECTATION_FAILED: return "Expectation Failed";
		case INTERNAL_SERVER_ERROR: return "Internal Server Error";
		case NOT_IMPLEMENTED: return "Not Implemented";
		case BAD_GATEWAY: return "Bad Gateway";
		case SERVICE_UNAVAILABLE: return "Service Unavailable";
		case GATEWAY_TIMEOUT: return "Gateway Timeout";
		case VERSION_NOT_SUPPORTED: return "HTTP Version Not Supported";
		case UNKNOWN_ERROR:
		default: return "Unknown Error";
	}
}

int WebEntity::errorCodeByType(WebEntity::ErrorType in)
{
	switch(in)
	{
		case BAD_REQUEST: return 400;
		case UNAUTHORIZED: return 401;
		case PAYMENT_REQUIRED: return 402;
		case FORBIDDEN: return 403;
		case NOT_FOUND: return 404;
		case METHOD_NOT_ALLOWED: return 405;
		case NOT_ACCEPTABLE: return 406;
		case PROXY_AUTH_REQUIRED: return 407;
		case REQUEST_TIMEOUT: return 408;
		case CONFLICT: return 409;
		case GONE: return 410;
		case LENGTH_REQUIRED: return 411;
		case PRECONDITION_FAILED: return 412;
		case ENTITY_TOO_LARGE: return 413;
		case URI_TOO_LONG: return 414;
		case UNSUPPORTED_MEDIA_TYPE: return 415;
		case RANGE_NOT_SATISFIABLE: return 416;
		case EXPECTATION_FAILED: return 417;
		case INTERNAL_SERVER_ERROR: return 500;
		case NOT_IMPLEMENTED: return 501;
		case BAD_GATEWAY: return 502;
		case SERVICE_UNAVAILABLE: return 503;
		case GATEWAY_TIMEOUT: return 504;
		case VERSION_NOT_SUPPORTED: return 505;
		case UNKNOWN_ERROR:
		default: return 0;
	}
}

QString WebEntity::generateToken()
{
	return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString WebEntity::getErrorPageTemplate()
{
	return R"html(
			<!doctype html>
			<html lang="en">
				<head>
					<meta charset="utf-8">
					<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
					<title>%TITLE%</title>

					<style>
						html, body {
							height: 100%;
						}
						.centered {
							text-align: center;
						}
						.main-content {
							min-height: 100%;
							min-height: 100vh;
							display: -webkit-box;
							display: -moz-box;
							display: -ms-flexbox;
							display: -webkit-flex;
							display: flex;

							-webkit-box-align: center;
							-webkit-align-items: center;
							-moz-box-align: center;
							-ms-flex-align: center;
							align-items: center;

							width: 100%;

							-webkit-box-pack: center;
							-moz-box-pack: center;
							-ms-flex-pack: center;
							-webkit-justify-content: center;
							justify-content: center;
						}
						.data-container {
							width: 640px;
						}
						pre {
							font-size: 14px;
						}
					</style>

				</head>
				<body>
					<div class="main-content">
						<div class="data-container">
							<h1 class="centered">%TITLE%</h1>
							<p>An error has occurred. Below you will find a short summary
							that may help to fix it or to prevent it from happening:</p>
							<pre>%MESSAGE%</pre>
							<pre class="centered">
O       o O       o O       o
| O   o | | O   o | | O   o |
| | O | | | | O | | | | O | |
| o   O | | o   O | | o   O |
o       O o       O o       O
							</pre>
						</div>
					</div>
				</body>
			</html>
	)html";
}

Response WebEntity::createError(WebEntity::ErrorType type, QString message)
{
	qDebug() << "An error has been detected:" << message;

	QByteArray headers {};
	QString caption = errorTypeToText(type);
	QString body = getErrorPageTemplate();

	if (message.isEmpty())
	{
		message	= "No information provided";
	}
	body.replace("%TITLE%", "Error " + QString::number(errorCodeByType(type)) + " - " + errorTypeToText(type));
	body.replace("%MESSAGE%", message);

	headers.append("HTTP/1.1 " + QString::number(errorCodeByType(type)) + " FAIL\n");
	headers.append("Content-Length: " + QString::number(body.length()) + "\n");
	headers.append("\n");

	return Response{headers, body.toLocal8Bit()};
}
