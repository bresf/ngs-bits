#include "BedFile.h"
#include "ToolBase.h"
#include "Statistics.h"
#include <QFileInfo>

#include "NGSHelper.h"
#include <QTextStream>
#include "Helper.h"
#include "Exceptions.h"

class ConcreteTool
		: public ToolBase
{
	Q_OBJECT

public:
	ConcreteTool(int& argc, char *argv[])
		: ToolBase(argc, argv)
	{
	}

	virtual void setup()
	{
		setDescription("Extracts base frequencies for given regions from BAMs files.");
		addInfileList("bam", "Input BAM file(s).", false);
		//optional
		addInfile("in", "Input BED file. If unset, reads from STDIN.", true);
		addOutfile("out", "Output TSV file. If unset, writes to STDOUT.", true);

	}

	virtual void main()
	{
		//init
		QStringList bams = getInfileList("bam");

		//open output stream
		QString out = getOutfile("out");
		QSharedPointer<QFile> outfile = Helper::openFileForWriting(out, true);
		QTextStream outstream(outfile.data());
		outstream << "#chr\tstart\tend\tsample\tA\tC\tG\tT\ttotal\n";

		//open BAM files
		QList<QSharedPointer<BamReader>> bams_open;
		foreach(QString bam, bams)
		{
			bams_open.append(QSharedPointer<BamReader>(new BamReader(bam)));
		}

		//extract base counts from BAMs
		BedFile file;
		file.load(getInfile("in"));
		for(int i=0; i<file.count(); ++i)
		{
			if(file[i].length()!=1)
			{
				THROW(ToolFailedException, "BED file contains region with length > 1, which is not supported: " + file[i].toString(true));
			}

			for(int j=0; j<bams.count(); ++j)
			{
				Pileup pileup = NGSHelper::getPileup(*bams_open[j].data(), file[i].chr(), file[i].end());
				outstream << file[i].toString(false)+"\t"+QFileInfo(bams[j]).baseName()+"\t"+QString::number(pileup.a())+"\t"+QString::number(pileup.c())+"\t"+QString::number(pileup.g())+"\t"+QString::number(pileup.t())+"\t"+QString::number(pileup.depth(false)) + "\n";
			}
		}
	}
};

#include "main.moc"

int main(int argc, char *argv[])
{
	ConcreteTool tool(argc, argv);
	return tool.execute();
}

