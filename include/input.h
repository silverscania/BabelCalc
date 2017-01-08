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

#pragma once

#include <QFrame>
#include "calculator.h"

struct Value;
class NarrowLineEdit;
class QLabel;
class QPropertyAnimation;

class Input : public QFrame //Inherit from QFrame instead of QWidget to support stylesheets (http://stackoverflow.com/a/22094319/3640643)
{
	Q_OBJECT
	public:
		explicit Input(int base, Mode mode, const QString& labelText, bool stripLeadingZeros, const QString& prefix = "");
		inline virtual ~Input() {}

		void inputDigits(const QString& digit);
		void unfocus() const;
		void focus() const;
		bool hasFocus() const;
		void setMode(Mode mode);
		void blink();

		const int base;
		const QString prefix;
		const QString labelText;

	signals:
		void inputChanged(const Value& value, int base, Mode mode);

	public slots:
		virtual void displayValueChanged(const Value& value, bool userInput);
		void setNextInputClears(bool inputClears);

	protected slots:
		void digitEdit(const QString& text);
		void lineEditFocus(bool focused);

	protected:
		NarrowLineEdit* lineEdit;
		QLabel* label;
		Mode mode;
		Value value;
		QPropertyAnimation* blinkAnim;

		virtual void updateValidator();
		void updateLabelText();
};

