#include "TestFramework.h"
#include "WebEntity.h"
#include "WebEntity.cpp"

TEST_CLASS(WebEntity_Test)
{
Q_OBJECT
private slots:
	void convert_contentTypeFromString()
	{				
		WebEntity::ContentType type = WebEntity::getContentTypeFromString("application/octet-stream");
		S_EQUAL(type, WebEntity::ContentType::APPLICATION_OCTET_STREAM);
	}
};
