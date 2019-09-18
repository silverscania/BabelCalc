// Copyright (C) 2017 James Clark <james@bitsofbeards.co.uk>
//
// This file is part of BabelCalc, a calculator that operates on
// multiple numerical bases simultaneously.
//
// BabelCalc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// BabelCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with BabelCalc.  If not, see <http://www.gnu.org/licenses/>.

#include "basicinput.h"
#include "narrowlineedit.h"
#include <QHBoxLayout>
#include <QLabel>
#include "gui.h"

BasicInput::BasicInput(int base, Mode mode, ReprMode reprMode, const QString& labelText, const QString& prefix) :
	Input(base, mode, reprMode, labelText, true, prefix)
{
	updateValidator();
	updateLabelText();

	lineEdit->setAlignment(Qt::AlignRight | Qt::AlignBottom);
	lineEdit->clearFocus(); //todo: not working to stop default focus on app start
    lineEdit->setObjectName("Basic");

	connect(lineEdit, &NarrowLineEdit::textEdited, this, &BasicInput::digitEdit);
	connect(lineEdit, &NarrowLineEdit::focussed, this, &BasicInput::lineEditFocus);

	label->setAlignment(Qt::AlignRight | Qt::AlignBottom);
	label->setObjectName("baseLabel");

	setLayout(new QHBoxLayout);

	layout()->setSpacing(0);
	layout()->setMargin(0);
	setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
	layout()->setAlignment(Qt::AlignRight);
	layout()->addWidget(label);
	layout()->addWidget(lineEdit);
	layout()->setAlignment(label, Qt::AlignRight);
	layout()->setAlignment(lineEdit, Qt::AlignRight);
}

void BasicInput::updateValidator()
{
	constexpr char nums[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	constexpr char lowerAlph[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
								  'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
								  'w', 'x', 'y', 'z'};
	//constexpr char upperAlph[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O'}; //todo more

	//set line len. Base 10 supports e notation and floating point so don't do a hard limit.
	if(base == 10) {
		lineEdit->setMaxLength(prefix.length() + 1 + GUI::DECIMAL_DISP_LEN + 5); // 5 digits for exp ("E+999")
	}
	else {
		lineEdit->setMaxLength(getMaxLength());
	}

	const QString signedPrefix = mode == Mode::Signed ? "-?" : "";
	QString validDigits;
	if(base <= 10) {
		validDigits = QString("[0-%1]").arg(QString(nums[base-1]));
	}
	else {
		validDigits = QString("[0-9,a-%1]").arg(QString(lowerAlph[base-10-1]));
	}

	QString pattern;
	//set validator.
	if(mode == Mode::Float) {
		// -?[0-9]+(\.[0-9]+)?(e-?[0-9]+)?
		pattern = QString(R"(-?%1%2+\.?%2*e?-?%2*)").arg(prefix, validDigits);
	}
	else if(base <= 10) {
		//TODO: pattern = support E notation for ints QString("%1%2[0-9]+e*-*[0-9]*").arg(signedPrefix, prefix);
		//TODO: toULongLong() doesn't support e notation though...
		pattern = QString("%3%1[0-%2]+").arg(prefix, QString(nums[base-1]), signedPrefix);
	}
	else {
		pattern = QString("%3%1[0-9,a-%2]+").arg(prefix, QString(lowerAlph[base-10-1]), signedPrefix);
	}

	const QRegExp regExp(pattern,Qt::CaseInsensitive);
	lineEdit->setValidator(new QRegExpValidator(regExp));
}

/**
 * @return Max length of an integer input based on base and bitWidth.
 * Doesn't apply to base 10 floating point nums
 */
int BasicInput::getMaxLength()
{
	//TODO: max length doesn't need to be set if input is converted
	//TODO: to a number on every input. Only successful inputs need to
	//TODO: update the box.
	CalcInt max;
	if(bitWidth == BitWidth::ThirtyTwo) {
		max = -0xFFFF'FFFF;
	}
	else  {
		max = -0xFFFF'FFFF'FFFF'FFFF;
	}
	return 10;
}
