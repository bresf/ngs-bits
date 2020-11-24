#include "FileLocationProviderServer.h"

FileLocationProviderServer::FileLocationProviderServer(QString gsvar_file, const SampleHeaderInfo header_info, const AnalysisType analysis_type)
  : gsvar_file_(gsvar_file)
  , header_info_(header_info)
  , analysis_type_(analysis_type)
{
}

void FileLocationProviderServer::setIsFoundFlag(FileLocation& file)
{
	file.is_found = false;
	if (QFile::exists(file.filename))
	{
		file.is_found = true;
	}
}

QList<FileLocation> FileLocationProviderServer::getBamFiles()
{
	QList<FileLocation> output;

	if (gsvar_file_ == nullptr)
	{
		THROW(ArgumentException, "File name has not been specified")
		return output;
	}

	if (header_info_.empty())
	{
		THROW(ArgumentException, "Header information has not been specified");
		return output;
	}

	///

//	static HttpHandler http_handler(HttpHandler::INI); //static to allow caching of credentials
//	try
//	{
//		HttpHeaders add_headers;
//		add_headers.insert("Accept", "application/json");

//		QString reply = http_handler.get("http://rest.genenames.org/fetch/symbol/"+gene_symbol, add_headers);
//		QJsonDocument json = QJsonDocument::fromJson(reply.toLatin1());
//		QJsonArray docs = json.object().value("response").toObject().value("docs").toArray();
//		if (docs.count()!=1)
//		{
//			THROW(Exception, "Could not convert gene symbol to NCBI identifier: HGNC REST API returned " + QString::number(docs.count()) + " entries!");
//		}
//		QString ncbi_id = docs.at(0).toObject().value("entrez_id").toString();
//		url.replace("[gene_id_ncbi]", ncbi_id);
//	}
//	catch(Exception& e)
//	{
//		QMessageBox::warning(QApplication::activeWindow(), "Could not get NCBI gene identifier from HGNC", e.message());
//		return;
//	}



	return output;
}

QList<FileLocation> FileLocationProviderServer::getSegFilesCnv()
{
	QList<FileLocation> output;

	////

	return output;
}

QList<FileLocation> FileLocationProviderServer::getIgvFilesBaf()
{
	QList<FileLocation> output;
	///

	return output;
}

QList<FileLocation> FileLocationProviderServer::getMantaEvidenceFiles()
{
	QList<FileLocation> evidence_files;
	///
	return evidence_files;
}
