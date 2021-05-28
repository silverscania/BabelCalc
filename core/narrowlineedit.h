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

#include <QLineEdit>

class NarrowLineEdit : public QLineEdit
{
	Q_OBJECT
	public:
		NarrowLineEdit(const QString& prefix, bool stripLeadingZeros);

		QSize sizeHint() const override;

		void recalcSize();
		inline void setNextInputClears(bool inputClears) { nextInputClears = inputClears; }
		void insert(const QString &);
		QString getStrippedInput() const;
		void setStripLeadingZeros(bool strip);
		const QString prefix;

	private:
		bool stripLeadingZeros;

		QSize getNarrowLineSize() const;
		void handleKeyInput(QKeyEvent* event);
		void handleControlInput(QKeyEvent *event);
		bool canEnterChar(const QString& text);
		void fixupInput();
		bool nextInputClears {false};
		//void paintEvent(QPaintEvent *);

	signals:
		void focussed(bool hasFocus);

	protected:
		void focusInEvent(QFocusEvent *e) override;
		void focusOutEvent(QFocusEvent *e) override;

		void keyPressEvent(QKeyEvent * event) override;
};
