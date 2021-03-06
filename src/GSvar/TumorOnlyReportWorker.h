#ifndef TUMORONLYREPORTWORKER_H
#define TUMORONLYREPORTWORKER_H

#include <QObject>
#include "VariantList.h"
#include "RtfDocument.h"
#include "FilterCascade.h"

///Input configuration for TumorOnlyReportWorker
struct TumorOnlyReportWorkerConfig
{
	QString mapping_stat_qcml_file = "";
	QString target_file = "";
	QString low_coverage_file = "";
	QString bam_file = "";

	FilterResult filter_result;

	bool include_coverage_per_gap = false;
};

///Helper class for tumor-only report generation
class TumorOnlyReportWorker
{

public:
	///constructor
	TumorOnlyReportWorker(const VariantList& variants, const TumorOnlyReportWorkerConfig& config);
	///writes RTF file with report to file_path
	void writeRtf(QByteArray file_path);
	///checks whether all neccessary annotations are available in variants and throws FileParseException if not available
	static void checkAnnotation(const VariantList& variants);

private:
	const VariantList& variants_;
	const FilterResult& filter_result_;

	RtfDocument doc_;

	//input files
	QString mapping_stat_qcml_file_;
	QString target_file_;
	QString low_cov_file_;
	QString bam_file_;

	bool include_coverage_per_gap_;

	//variant annotation indices
	int i_co_sp_;
	int i_tum_af_;
	int i_cgi_driver_statem_;
	int i_ncg_oncogene_;
	int i_ncg_tsg;
	int i_germl_class_;
	int i_somatic_class_;

	///Returns variant description with information from NCG, CGI and in-house classification
	QByteArray variantDescription(const Variant& var);

	///translates somatic variant classification to German language
	QByteArray trans(QByteArray english);
};

#endif // TUMORONLYREPORTWORKER_H
