#include "RtfDocument.h"
#include "Helper.h"


RtfSourceCode RtfText::RtfCode()
{
	QByteArrayList output;
	output << "{";

	output << "\\fs" + QByteArray::number(font_size_);
	if(font_number_ > 0) output << "\\f" + QByteArray::number(font_number_);
	if(font_color_ > 0) output << "\\cf" + QByteArray::number(font_color_);

	if(bold_) output << "\\b";
	if(italic_) output << "\\i";
	if(highlight_color_ != 0) output << "\\highlight" + QByteArray::number(highlight_color_);
	output << "\\q" + horizontal_alignment_;

	//if(!content_.isEmpty())
	output << RtfDocument::escapeUmlauts(content_);
	//else output << "";

	output << "}";
	return output.join("\n");
}


RtfSourceCode RtfParagraph::RtfCode()
{
	QByteArrayList output;

	output << "\\pard";

	if(part_of_a_cell_) output << "\\q" + horizontal_alignment_;

	//Spacing
	if(space_after_ != 0) output << "\\sa" + QByteArray::number(space_after_);
	if(space_before_ != 0) output << "\\sb" + QByteArray::number(space_before_);

	//Indenting
	if(indent_block_left_ != 0) output << "\\li" + QByteArray::number(indent_block_left_);
	if(indent_block_right_ != 0) output << "\\ri" + QByteArray::number(indent_block_right_);
	if(indent_first_line_ != 0) output << "\\fi" + QByteArray::number(indent_first_line_);

	output.append(RtfText::RtfCode());

	//Skip paragraph end line break if paragraph is part of a table cell
	if(!part_of_a_cell_) output << "\\par\n";

	return output.join("\n");
}

RtfDocument::RtfDocument()
	: width_(11905)
	, height_(15840)
	, margin_top_(1134)
	, margin_bottom_(1134)
	, margin_left_(1134)
	, margin_right_(1134)
	, fonts_({"Arial"})
	, default_font_size_(18)
{
}


RtfSourceCode RtfDocument::header()
{
	QByteArrayList output;
	output << "{\\rtf\\ansi";
	output << "\\deff0";

	//Create list of fonts
	//default font
	QByteArray font_table = "{\\fonttbl{\\f0 " + fonts_[0] + ";";
	//additional fonts
	if(fonts_.count() > 1)
	{
		for(int i=1;i<fonts_.count();++i)
		{
			font_table.append("\\f" + QByteArray::number(i+1) + " " + fonts_[i] + ";");
		}
	}
	font_table.append("}}");
	output << font_table;

	//page size
	output << "\\paperw" + QByteArray::number(width_);
	output << "\\paperh" + QByteArray::number(height_);

	//margins
	output << "\\margl" + QByteArray::number(margin_left_);
	output << "\\margr" + QByteArray::number(margin_right_);
	output << "\\margt" + QByteArray::number(margin_top_);
	output << "\\margb" + QByteArray::number(margin_bottom_);


	//colors used in document
	if(!colors_.isEmpty())
	{
		QByteArray tmp_out_color_table = "{\\colortbl;";
		foreach(auto rgb_value,colors_)
		{
			int red,green,blue;
			std::tie(red,green,blue) = rgb_value;
			tmp_out_color_table.append("\\red" + QByteArray::number(red) + "\\green" +  QByteArray::number(green) + "\\blue" +  QByteArray::number(blue) +";");
		}
		tmp_out_color_table.append("}");
		output << tmp_out_color_table;
	}

	//Set document language to German, specifiy default font size, turn on widow control and auto hyphenation
	output << "\\deflang1031\\plain\\fs" + QByteArray::number(default_font_size_)  + "\\widowctrl\\hyphauto";

	return output.join("\n");
}

RtfSourceCode RtfDocument::footer()
{
	return "\n}";
}

RtfSourceCode RtfDocument::escapeUmlauts(const QByteArray &text)
{
	QByteArray output = text;
	output.replace("ß","\\u223;");
	output.replace("ä","\\u228;");
	output.replace("ö","\\u246;");
	output.replace("ü","\\u252;");
	output.replace("Ä","\\u196;");
	output.replace("Ö","\\u214;");
	output.replace("Ü","\\u220;");
	return output;
}

double RtfDocument::cm2twip(double input_cm)
{
	return 566.929133858264*input_cm;
}

void RtfDocument::setMargins(int left, int top, int right, int bottom)
{
	margin_top_ = top;
	margin_bottom_ = bottom;
	margin_right_ = right;
	margin_left_ = left;
}

void RtfDocument::save(const QByteArray &file_name)
{
	QSharedPointer<QFile> outfile = Helper::openFileForWriting(file_name);
	QTextStream stream(outfile.data());

	stream << header();

	foreach(RtfSourceCode part,body_parts_)
	{
		stream << part << endl;
	}

	stream << footer();
	outfile->close();
}

RtfTableCell::RtfTableCell(int width, const RtfParagraph& text_format)
{
	paragraph_ = text_format;
	paragraph_.setPartOfACell(true);
	width_ = width;
}

void RtfTableCell::setBorder(int left,int top,int right,int bottom, const QByteArray& type)
{
	border_left_ = left;
	border_right_ = right;
	border_bottom_ = bottom;
	border_top_ = top;
	border_type_ = type;
}

void RtfTableRow::addCell(int width, const RtfParagraph& paragraph)
{
	RtfParagraph temp_paragraph = paragraph;
	temp_paragraph.setPartOfACell(true);
	cells_ << RtfTableCell(width, paragraph);
}

void RtfTableRow::addCell(int width,const QByteArray& content)
{
	RtfParagraph temp_paragraph;
	temp_paragraph.setContent(content);
	temp_paragraph.setPartOfACell(true);
	addCell(width,temp_paragraph);
}

void RtfTableRow::addCell(const QByteArrayList& cell_contents, int width, const RtfParagraph& par_format)
{
	RtfParagraph temp_par = par_format;
	temp_par.setPartOfACell(true);
	temp_par.setContent(cell_contents.join("\\line\n"));
	addCell(width,temp_par);
}

RtfTableRow& RtfTableRow::setBorders(int width, const QByteArray& type)
{
	for(auto &cell : cells_)
	{
		cell.setBorder(width,width,width,width,type);
	}
	return *this;
}

RtfTableRow& RtfTableRow::setBackgroundColor(int color)
{
	for(RtfTableCell& cell : cells_)
	{
		cell.setBackgroundColor(color);
	}
	return *this;
}

QByteArray RtfTableCell::writeCell()
{
	QByteArray output = "{";

	output.append("\\intbl " + paragraph_.RtfCode());

	output.append("\\cell");
	output.append("}");
	return output;
}


RtfTableRow::RtfTableRow()
{
}

RtfTableRow::RtfTableRow(const QByteArray& cell_content, int width, const RtfParagraph& format)
{
	RtfParagraph temp_par = format;
	temp_par.setContent(cell_content);
	addCell(width,temp_par);
}

RtfTableRow::RtfTableRow(const QList<QByteArray>& cell_contents, const QList<int>& cell_widths, const RtfParagraph& format)
{
	if(cell_contents.count() != cell_widths.count()) //Create empty instance if no does not match
	{
		RtfTableRow();
		return;
	}

	for(int i=0;i<cell_contents.count();++i)
	{
		RtfParagraph temp_par = format;
		temp_par.setContent(cell_contents.at(i));
		addCell(cell_widths.at(i),temp_par);
	}

}

RtfSourceCode RtfTableRow::writeRowHeader()
{
	QByteArray output = "\\trowd\\trgraph" + QByteArray::number(table_row_gap_half) + (tr_left_ > 0 ? "\\trleft" + QByteArray::number(tr_left_) : "");

	//Position of the rightmost extreme of a single cell
	int right_cell_offset = tr_left_;
	foreach(auto cell, cells_) //Cell specification
	{
		right_cell_offset += cell.width();

		//cell border
		if(cell.border_top_ != 0) output.append("\\clbrdrt\\brdrw" + QByteArray::number(cell.border_top_) + "\\" + cell.border_type_);
		if(cell.border_bottom_ != 0) output.append("\\clbrdrb\\brdrw" + QByteArray::number(cell.border_bottom_) + "\\" + cell.border_type_);
		if(cell.border_left_ != 0) output.append("\\clbrdrl\\brdrw" + QByteArray::number(cell.border_left_) + "\\" + cell.border_type_);
		if(cell.border_right_ != 0) output.append("\\clbrdrr\\brdrw" + QByteArray::number(cell.border_right_) + "\\" + cell.border_type_);

		if(cell.background_color_ != 0) output.append("\\clcbpat" + QByteArray::number(cell.background_color_));

		//Add specific control words
		if(cell.controlWords().count() > 0)
		{
			for(auto control_word : cell.controlWords())
			{
				output.append("\\" + control_word);
			}
		}

		//cell width
		output.append("\\cellx" + QByteArray::number(right_cell_offset));
	}

	output.append("\n");
	return output;
}

RtfSourceCode RtfTableRow::writeRow()
{
	QByteArrayList output;

	output << writeRowHeader();

	foreach(RtfTableCell cell,cells_)
	{
		output << cell.writeCell();
	}

	output << "\\row";

	return output.join("\n");
}

RtfTable::RtfTable()
{

}

RtfTable::RtfTable(const QList< QList<QByteArray> >& contents, const QList< QList<int> >& widths, const RtfParagraph& format)
{
	for(int i=0;i<contents.count();++i)
	{
		rows_.append(RtfTableRow(contents[i],widths[i],format));
	}
}

RtfSourceCode RtfTable::RtfCode()
{
	QByteArrayList output;
	for(int i=0;i<rows_.count();++i)
	{
		output << rows_[i].writeRow();
	}

	return output.join("\n");
}

RtfTable& RtfTable::setUniqueBorder(int border, const QByteArray &border_type)
{
	for(int i=0;i<rows_.count();++i)
	{
		rows_[i].setBorders(border,border_type);
	}
	return *this;
}

void RtfTable::sortByCol(int i_col)
{
	QMultiMap<QByteArray,RtfTableRow> cell_row;
	foreach(RtfTableRow row,rows_)
	{
		cell_row.insert(row[i_col].format().content(),row);
	}

	QList<QByteArray> keys_sorted = cell_row.uniqueKeys();
	std::sort(keys_sorted.begin(),keys_sorted.end());

	QList<RtfTableRow> sorted_table;
	foreach(const QByteArray& key, keys_sorted)
	{
		QList<RtfTableRow> values = cell_row.values(key);
		for(int i=0;i<values.size();++i)
		{
			sorted_table << values.at(i);
		}
	}
	rows_ = sorted_table;
}