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

NarrowLineEdit::NarrowLineEdit(const QString& prefix, bool stripLeadingZeros) :
	QLineEdit(prefix+"0"),
	prefix(prefix),
	stripLeadingZeros(stripLeadingZeros)
{
	connect(this, &NarrowLineEdit::textChanged, [=]() { recalcSize();});
	//QCursor cursor;//= cursor();
	//cursor.setShape(Qt::CustomCursor);
//	QCursor cursor(QBitmap(10, 100), QBitmap(10, 100));
//	setCursor(cursor);
//style()->pix
//	set
//	QStyleOption option;
//	option.initFrom(this);
}

QSize NarrowLineEdit::getNarrowLineSize() const
{
	ensurePolished();
	QFontMetrics fm(font());

	auto size = fm.boundingRect(displayText()).size(); //QSize(300, 10);

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

void NarrowLineEdit::handleKeyInput(QKeyEvent *event)
{
	if(nextInputClears) {
		setText(prefix + "0");
	}

	//Don't do anything if not a valid char (also ignore the event so the gui gets the '-' key)
	if(!canEnterChar(event->text())) {
		event->ignore();
		return;
	}

	QLineEdit::keyPressEvent(event);

	fixupInput();
}

///Backspace and arrow keys
void NarrowLineEdit::handleControlInput(QKeyEvent *event)
{
	if(nextInputClears) {
		setText(prefix + "0");
	}

	QLineEdit::keyPressEvent(event);
	fixupInput();
}

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


bool NarrowLineEdit::canEnterChar(const QString& character) {
	QString newString = displayText();
	newString.insert(cursorPosition(), character);

	int notUsed = 0;
	if(validator())
		return validator()->validate(newString, notUsed) == QValidator::State::Acceptable;
	else
		return true;
}

void NarrowLineEdit::keyPressEvent(QKeyEvent *event)
{
	if(event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
		handleKeyInput(event);
	}
	else if(event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z) {
		handleKeyInput(event);
	}
	else {
		switch(event->key()) {
			case Qt::Key_Left: case Qt::Key_Right:
			case Qt::Key_Backspace:
				handleControlInput(event);
				break;

			case Qt::Key_Period:
			case Qt::Key_Minus:
				handleKeyInput(event);
				break;

			default:
				event->ignore();
				break;
		}
	}
}
