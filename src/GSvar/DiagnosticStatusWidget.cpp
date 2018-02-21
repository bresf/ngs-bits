#include "DiagnosticStatusWidget.h"
#include "NGSD.h"

DiagnosticStatusWidget::DiagnosticStatusWidget(QWidget *parent)
	: QWidget(parent)
	, ui()
	, initial_status_text_()
{
	ui.setupUi(this);

	//init comboboxes from DB enums
	NGSD db;
	ui.status->addItems(db.getEnum("diag_status", "status"));
	ui.outcome->addItems(db.getEnum("diag_status", "outcome"));
	ui.inheritance->addItems(db.getEnum("diag_status", "inheritance_mode"));

	//signals+slots
	connect(ui.outcome, SIGNAL(currentTextChanged(QString)), this, SIGNAL(outcomeChanged(QString)));
}

void DiagnosticStatusWidget::setStatus(DiagnosticStatusData data)
{
	QString last_edit = data.user + " / " + data.date.toString(Qt::ISODate).replace('T', ' ');
	if (last_edit.trimmed()=="/") last_edit.clear();
	ui.user_date->setText(last_edit);
	ui.status->setCurrentText(data.dagnostic_status);
	ui.outcome->setCurrentText(data.outcome);
	ui.genes_causal->setText(data.genes_causal);
	ui.inheritance->setCurrentText(data.inheritance_mode);
	ui.genes_incidental->setText(data.genes_incidental);
	ui.comment->setText(data.comments);

	//store initial status to check if sample is scheduled for re-sequencin
	initial_status_text_ = data.dagnostic_status;
}

DiagnosticStatusData DiagnosticStatusWidget::status() const
{
	DiagnosticStatusData output;

	output.dagnostic_status = ui.status->currentText();
	output.outcome = ui.outcome->currentText();
	output.genes_causal = ui.genes_causal->text().trimmed();
	output.inheritance_mode = ui.inheritance->currentText();
	output.genes_incidental = ui.genes_incidental->text().trimmed();
	output.comments = ui.comment->text().trimmed();

	//set user/date only if available
	QStringList user_date_parts = ui.user_date->text().split('/');
	if (user_date_parts.count()==2)
	{
		output.user = user_date_parts[0].trimmed();
		output.date = QDateTime::fromString(user_date_parts[1].trimmed().replace(' ', 'T'), Qt::ISODate);
	}

	return output;
}
