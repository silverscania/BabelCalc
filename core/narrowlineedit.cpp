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

#include "narrowlineedit.h"

#include <iostream>
#include <QEvent>
#include <QKeyEvent>
#include <QValidator>
#include <QStyleOption>

#include "input.h"

NarrowLineEdit::NarrowLineEdit(const QString& prefix, bool stripLeadingZeros,
							   const Input& parentInput) :
	QLineEdit(prefix+"0"),
	prefix(prefix),
	stripLeadingZeros(stripLeadingZeros)
{
	connect(this, &NarrowLineEdit::textChanged, [=]() { recalcSize();});
	setValidator(new Validator(parentInput));
}

QSize NarrowLineEdit::getNarrowLineSize() const
{
	ensurePolished();
	QFontMetrics fm(font());

	auto size = fm.size(0, displayText());

	int marginLeft, marginRight, marginTop, marginBottom;
	getTextMargins(&marginLeft, &marginRight, &marginTop, &marginBottom);
	size += QSize(marginLeft+marginRight, marginTop+marginBottom);

	getContentsMargins(&marginLeft, &marginRight, &marginTop, &marginBottom);
	size += QSize(marginLeft+marginRight, marginTop+marginBottom);

    // Add a bit otherwise the text gets cropped, not sure why
	return size + QSize(30, 0);
}

QString NarrowLineEdit::getStrippedInput() const
{
	QString stripped = displayText();
	return stripped.replace(prefix, "").replace(" ", "");
}

void NarrowLineEdit::setStripLeadingZeros(bool strip)
{
	stripLeadingZeros = strip;
}

void NarrowLineEdit::fixupInput() {
	bool modified = false;

	// If the field is completely empty, insert a zero
	// so that there is always something displaying
	QString stripped = getStrippedInput();
	if(stripped.length() == 0) {
		stripped = "0";
		modified = true;
	}

	// Make sure that there are no leading zeros. There is a special
	// case for floating point numbers starting with "0.".
	if(stripLeadingZeros) {
		while(stripped.length() > 1 && stripped[0] == '0' &&
			  stripped.startsWith("0.") == false) {
			stripped = stripped.mid(1);
			modified = true;
		}
	}

	if(modified) {
		setText(prefix + stripped);
	}
}

/**
 * Used for input Keys on GUI, rather than keyboard input.
 */
void NarrowLineEdit::insert(const QString& text)
{
	if(nextInputClears) {
		setText(prefix + "0");
	}

	QLineEdit::insert(text);

	fixupInput();
}

void NarrowLineEdit::recalcSize()
{
	setMinimumSize(getNarrowLineSize());
}

QSize NarrowLineEdit::sizeHint() const
{
	return getNarrowLineSize();
}

void NarrowLineEdit::focusInEvent(QFocusEvent *e)
{
	QLineEdit::focusInEvent(e);
	emit focussed(true);
}

void NarrowLineEdit::focusOutEvent(QFocusEvent *e)
{
	QLineEdit::focusOutEvent(e);
	emit focussed(false);
}

void NarrowLineEdit::keyPressEvent(QKeyEvent *event)
{
	if(nextInputClears) {
		setText(prefix + "0");
	}

	// Special case - sign being inserted at the first character to negate the
	// number, otherwise send the - to the GUI to do subtraction operation.
	if(event->text() == "-" && cursorPosition() != 0) {
		event->ignore();
		return;
	}

	QLineEdit::keyPressEvent(event);
	fixupInput();
}

QValidator::State Validator::validate(QString &inputText, int &pos) const
{
	if (input.validate(inputText))
		return QValidator::Acceptable;
	else
		return QValidator::Invalid;
}
