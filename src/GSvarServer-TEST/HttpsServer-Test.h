#include "TestFramework.h"
#include "HttpsServer.h"

TEST_CLASS(HttpServer_Test)
{
Q_OBJECT
private slots:
	void textOutput_test()
	{
		QString text = "Test";
		S_EQUAL(text, "Test");
	}
};
