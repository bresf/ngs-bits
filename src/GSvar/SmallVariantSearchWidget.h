#ifndef SmallVariantSearchWidget_H
#define SmallVariantSearchWidget_H

#include <QWidget>
#include "ui_SmallVariantSearchWidget.h"
#include "DelayedInitializationTimer.h"
#include "Chromosome.h"
#include "GeneSet.h"
#include "VariantList.h"

class SmallVariantSearchWidget
	: public QWidget
{
	Q_OBJECT

public:
	SmallVariantSearchWidget(QWidget* parent = 0);
	void setGene(const QString& gene);

signals:
	void openVariantTab(Variant);

private slots:
	void changeSearchType();
	void updateVariants();
	void copyToClipboard();
	void variantContextMenu(QPoint pos);

private:
	Ui::SmallVariantSearchWidget ui_;
	DelayedInitializationTimer init_timer_;

	void getVariantsForRegion(Chromosome chr, int start, int end, QByteArray gene, const GeneSet& gene_symbols, QList<QStringList>& output, QStringList& messages);

};

#endif // SmallVariantSearchWidget_H
