#include "TestFramework.h"
#include "WebEntity.h"
#include "WebEntity.cpp"

TEST_CLASS(WebEntity_Test)
{
Q_OBJECT
private slots:
	void test_getContentTypeFromString()
	{				
		WebEntity::ContentType type = WebEntity::getContentTypeFromString("application/octet-stream");
		S_EQUAL(type, WebEntity::ContentType::APPLICATION_OCTET_STREAM);

		type = WebEntity::getContentTypeFromString("application/json");
		S_EQUAL(type, WebEntity::ContentType::APPLICATION_JSON);

		type = WebEntity::getContentTypeFromString("application/javascript");
		S_EQUAL(type, WebEntity::ContentType::APPLICATION_JAVASCRIPT);

		type = WebEntity::getContentTypeFromString("image/jpeg");
		S_EQUAL(type, WebEntity::ContentType::IMAGE_JPEG);

		type = WebEntity::getContentTypeFromString("image/png");
		S_EQUAL(type, WebEntity::ContentType::IMAGE_PNG);

		type = WebEntity::getContentTypeFromString("image/svg+xml");
		S_EQUAL(type, WebEntity::ContentType::IMAGE_SVG_XML);

		type = WebEntity::getContentTypeFromString("text/plain");
		S_EQUAL(type, WebEntity::ContentType::TEXT_PLAIN);

		type = WebEntity::getContentTypeFromString("text/csv");
		S_EQUAL(type, WebEntity::ContentType::TEXT_CSV);

		type = WebEntity::getContentTypeFromString("text/html");
		S_EQUAL(type, WebEntity::ContentType::TEXT_HTML);

		type = WebEntity::getContentTypeFromString("text/xml");
		S_EQUAL(type, WebEntity::ContentType::TEXT_XML);

		type = WebEntity::getContentTypeFromString("text/css");
		S_EQUAL(type, WebEntity::ContentType::TEXT_CSS);

		type = WebEntity::getContentTypeFromString("multipart/form-data");
		S_EQUAL(type, WebEntity::ContentType::MULTIPART_FORM_DATA);
	}

	void test_getMethodTypeFromString()
	{
		Request::MethodType type = WebEntity::getMethodTypeFromString("get");
		S_EQUAL(type, Request::MethodType::GET);

		type = WebEntity::getMethodTypeFromString("post");
		S_EQUAL(type, Request::MethodType::POST);

		type = WebEntity::getMethodTypeFromString("delete");
		S_EQUAL(type, Request::MethodType::DELETE);

		type = WebEntity::getMethodTypeFromString("put");
		S_EQUAL(type, Request::MethodType::PUT);

		type = WebEntity::getMethodTypeFromString("patch");
		S_EQUAL(type, Request::MethodType::PATCH);
	}

	void test_convertMethodTypeToString()
	{
		QString type = WebEntity::convertMethodTypeToString(Request::MethodType::GET);
		S_EQUAL(type, "get");

		type = WebEntity::convertMethodTypeToString(Request::MethodType::POST);
		S_EQUAL(type, "post");

		type = WebEntity::convertMethodTypeToString(Request::MethodType::DELETE);
		S_EQUAL(type, "delete");

		type = WebEntity::convertMethodTypeToString(Request::MethodType::PUT);
		S_EQUAL(type, "put");

		type = WebEntity::convertMethodTypeToString(Request::MethodType::PATCH);
		S_EQUAL(type, "patch");
	}

	void test_convertContentTypeToString()
	{
		QString type = WebEntity::convertContentTypeToString(WebEntity::ContentType::APPLICATION_OCTET_STREAM);
		S_EQUAL(type, "application/octet-stream");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::APPLICATION_JSON);
		S_EQUAL(type, "application/json");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::APPLICATION_JAVASCRIPT);
		S_EQUAL(type, "application/javascript");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::IMAGE_JPEG);
		S_EQUAL(type, "image/jpeg");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::IMAGE_PNG);
		S_EQUAL(type, "image/png");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::IMAGE_SVG_XML);
		S_EQUAL(type, "image/svg+xml");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::TEXT_PLAIN);
		S_EQUAL(type, "text/plain");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::TEXT_CSV);
		S_EQUAL(type, "text/csv");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::TEXT_HTML);
		S_EQUAL(type, "text/html");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::TEXT_XML);
		S_EQUAL(type, "text/xml");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::TEXT_CSS);
		S_EQUAL(type, "text/css");

		type = WebEntity::convertContentTypeToString(WebEntity::ContentType::MULTIPART_FORM_DATA);
		S_EQUAL(type, "multipart/form-data");
	}

	void test_getContentTypeByFilename()
	{
		WebEntity::ContentType type = WebEntity::getContentTypeByFilename("json");
		S_EQUAL(type, WebEntity::ContentType::APPLICATION_JSON);

		type = WebEntity::getContentTypeByFilename("js");
		S_EQUAL(type, WebEntity::ContentType::APPLICATION_JAVASCRIPT);

		type = WebEntity::getContentTypeByFilename("jpeg");
		S_EQUAL(type, WebEntity::ContentType::IMAGE_JPEG);

		type = WebEntity::getContentTypeByFilename("jpg");
		S_EQUAL(type, WebEntity::ContentType::IMAGE_JPEG);

		type = WebEntity::getContentTypeByFilename("png");
		S_EQUAL(type, WebEntity::ContentType::IMAGE_PNG);

		type = WebEntity::getContentTypeByFilename("svg");
		S_EQUAL(type, WebEntity::ContentType::IMAGE_SVG_XML);

		type = WebEntity::getContentTypeByFilename("txt");
		S_EQUAL(type, WebEntity::ContentType::TEXT_PLAIN);

		type = WebEntity::getContentTypeByFilename("csv");
		S_EQUAL(type, WebEntity::ContentType::TEXT_CSV);

		type = WebEntity::getContentTypeByFilename("html");
		S_EQUAL(type, WebEntity::ContentType::TEXT_HTML);

		type = WebEntity::getContentTypeByFilename("htm");
		S_EQUAL(type, WebEntity::ContentType::TEXT_HTML);

		type = WebEntity::getContentTypeByFilename("xml");
		S_EQUAL(type, WebEntity::ContentType::TEXT_XML);

		type = WebEntity::getContentTypeByFilename("css");
		S_EQUAL(type, WebEntity::ContentType::TEXT_CSS);
	}

	void test_convertIconNameToString()
	{
		QString icon = WebEntity::convertIconNameToString(WebEntity::FolderItemIcon::TO_PARENT_FOLDER);
		S_EQUAL(icon, "up_dir");

		icon = WebEntity::convertIconNameToString(WebEntity::FolderItemIcon::GENERIC_FILE);
		S_EQUAL(icon, "generic_file");

		icon = WebEntity::convertIconNameToString(WebEntity::FolderItemIcon::BINARY_FILE);
		S_EQUAL(icon, "binary_file");

		icon = WebEntity::convertIconNameToString(WebEntity::FolderItemIcon::CODE_FILE);
		S_EQUAL(icon, "code_file");

		icon = WebEntity::convertIconNameToString(WebEntity::FolderItemIcon::PICTURE_FILE);
		S_EQUAL(icon, "picture_file");

		icon = WebEntity::convertIconNameToString(WebEntity::FolderItemIcon::TEXT_FILE);
		S_EQUAL(icon, "text_file");

		icon = WebEntity::convertIconNameToString(WebEntity::FolderItemIcon::TABLE_FILE);
		S_EQUAL(icon, "table_file");

		icon = WebEntity::convertIconNameToString(WebEntity::FolderItemIcon::FOLDER);
		S_EQUAL(icon, "folder");
	}

	void test_convertErrorTypeToText()
	{
		QString error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::BAD_REQUEST);
		S_EQUAL(error_msg, "Bad Request");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::UNAUTHORIZED);
		S_EQUAL(error_msg, "Unauthorized");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::PAYMENT_REQUIRED);
		S_EQUAL(error_msg, "Payment Required");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::FORBIDDEN);
		S_EQUAL(error_msg, "Forbidden");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::NOT_FOUND);
		S_EQUAL(error_msg, "Not Found");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::METHOD_NOT_ALLOWED);
		S_EQUAL(error_msg, "Method Not Allowed");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::NOT_ACCEPTABLE);
		S_EQUAL(error_msg, "Not Acceptable");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::PROXY_AUTH_REQUIRED);
		S_EQUAL(error_msg, "Proxy Authentication Required");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::REQUEST_TIMEOUT);
		S_EQUAL(error_msg, "Request Timeout");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::CONFLICT);
		S_EQUAL(error_msg, "Conflict");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::GONE);
		S_EQUAL(error_msg, "Gone");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::LENGTH_REQUIRED);
		S_EQUAL(error_msg, "Length Required");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::PRECONDITION_FAILED);
		S_EQUAL(error_msg, "Precondition Failed");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::ENTITY_TOO_LARGE);
		S_EQUAL(error_msg, "Request Entity Too Large");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::URI_TOO_LONG);
		S_EQUAL(error_msg, "Request-URI Too Long");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::UNSUPPORTED_MEDIA_TYPE);
		S_EQUAL(error_msg, "Unsupported Media Type");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::RANGE_NOT_SATISFIABLE);
		S_EQUAL(error_msg, "Requested Range Not Satisfiable");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::EXPECTATION_FAILED);
		S_EQUAL(error_msg, "Expectation Failed");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::INTERNAL_SERVER_ERROR);
		S_EQUAL(error_msg, "Internal Server Error");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::NOT_IMPLEMENTED);
		S_EQUAL(error_msg, "Not Implemented");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::BAD_GATEWAY);
		S_EQUAL(error_msg, "Bad Gateway");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::SERVICE_UNAVAILABLE);
		S_EQUAL(error_msg, "Service Unavailable");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::GATEWAY_TIMEOUT);
		S_EQUAL(error_msg, "Gateway Timeout");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::VERSION_NOT_SUPPORTED);
		S_EQUAL(error_msg, "HTTP Version Not Supported");

		error_msg = WebEntity::convertErrorTypeToText(WebEntity::ErrorType::UNKNOWN_ERROR);
		S_EQUAL(error_msg, "Unknown Error");
	}

	void test_getErrorCodeByType()
	{
		int code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::BAD_REQUEST);
		I_EQUAL(code, 400);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::UNAUTHORIZED);
		I_EQUAL(code, 401);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::PAYMENT_REQUIRED);
		I_EQUAL(code, 402);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::FORBIDDEN);
		I_EQUAL(code, 403);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::NOT_FOUND);
		I_EQUAL(code, 404);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::METHOD_NOT_ALLOWED);
		I_EQUAL(code, 405);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::NOT_ACCEPTABLE);
		I_EQUAL(code, 406);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::PROXY_AUTH_REQUIRED);
		I_EQUAL(code, 407);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::REQUEST_TIMEOUT);
		I_EQUAL(code, 408);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::CONFLICT);
		I_EQUAL(code, 409);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::GONE);
		I_EQUAL(code, 410);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::LENGTH_REQUIRED);
		I_EQUAL(code, 411);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::PRECONDITION_FAILED);
		I_EQUAL(code, 412);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::ENTITY_TOO_LARGE);
		I_EQUAL(code, 413);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::URI_TOO_LONG);
		I_EQUAL(code, 414);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::UNSUPPORTED_MEDIA_TYPE);
		I_EQUAL(code, 415);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::RANGE_NOT_SATISFIABLE);
		I_EQUAL(code, 416);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::EXPECTATION_FAILED);
		I_EQUAL(code, 417);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::INTERNAL_SERVER_ERROR);
		I_EQUAL(code, 500);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::NOT_IMPLEMENTED);
		I_EQUAL(code, 501);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::BAD_GATEWAY);
		I_EQUAL(code, 502);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::SERVICE_UNAVAILABLE);
		I_EQUAL(code, 503);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::GATEWAY_TIMEOUT);
		I_EQUAL(code, 504);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::VERSION_NOT_SUPPORTED);
		I_EQUAL(code, 505);

		code = WebEntity::getErrorCodeByType(WebEntity::ErrorType::UNKNOWN_ERROR);
		I_EQUAL(code, 0);
	}
};