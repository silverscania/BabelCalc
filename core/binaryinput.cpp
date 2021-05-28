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

#include "binaryinput.h"
#include "narrowlineedit.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>

BinaryInput::BinaryInput(Mode mode, ReprMode reprMode) :
	BasicInput(2, mode, reprMode, "binary")
{
	// Object name is used by style sheets to set font size
	lineEdit->setObjectName("Binary");

	//binary markers
	QString binaryMarkerString = QString("63     56 %1 48 %1 40 %1 32 %1 24 %1 16 %2 8 %2 0").arg("      ", "       ");
	binaryMarker = new QLabel(binaryMarkerString);
	binaryMarker->setAlignment(Qt::AlignRight);
    binaryMarker->setStyleSheet("margin-bottom: 8px;");
    binaryMarker->setObjectName("Binary");
	grid->addWidget(binaryMarker, 1, 1);
}

/**
 * When in human repr mode, the binary display removes
 * the bit position markers because the decimal place
 * will cause the number to shift an arbitrary number of
 * digits to the left.
 */
void BinaryInput::setReprMode(ReprMode mode)
{
	Input::setReprMode(mode);
	if(mode == ReprMode::Human) {
		binaryMarker->setVisible(false);
	}
	else {
		binaryMarker->setVisible(true);
	}
}

void BinaryInput::displayValueChanged(const Value &value, bool userInput)
{
	this->value = value;

	//line edit currently being used ignores new values
	if(lineEdit->hasFocus() && userInput) {
		return;
	}

	QString binary;

	if(reprMode == ReprMode::Human) {
		switch (mode) {
			case Mode::Signed:
				BasicInput::displayValueChanged(value, userInput);
				return;
			case Mode::Unsigned:
				binary = QString("%1").arg(value.uIntVal, 64, base, QLatin1Char('0'));
				break;

			case Mode::Float:
				//TODO: convert binary to human readable type float (1011.11101)
				break;
			default:
				Q_ASSERT(false);
				break;
		}
	}
	else { //Machine version
		switch (mode) {
			case Mode::Signed:
			{
				binary = QString("%1").arg(Value::intToRaw(value.intVal),
										   64, base, QLatin1Char('0'));
				break;
			}
			case Mode::Unsigned:
				binary = QString("%1").arg(value.uIntVal, 64, base, QLatin1Char('0'));
				break;

			case Mode::Float:
			{
				binary = QString("%1").arg(Value::floatToRaw(value.floatVal),
										   64, base, QLatin1Char('0'));
				break;
			}
			default:
				Q_ASSERT(false);
				break;
		}
	}

	for(int i = 4; i < binary.length(); i+=5) {
		binary.insert(i, " ");
	}

	lineEdit->setText(binary);
}

void BinaryInput::updateValidator()
{
	if(reprMode == ReprMode::Human && mode == Mode::Signed)
	{
		// Use all base class behavior for Signed input mode so that the -
		// can be typed. That behavior is difficult to reproduce with an
		// input mask.
		lineEdit->setInputMask("");
		lineEdit->setStripLeadingZeros(true);
		BasicInput::updateValidator();
		displayValueChanged(value, false);
	}
	else {
		// Remove validator that might have been applied by BasicInput when
		// in human mode
		lineEdit->setValidator(nullptr);
		lineEdit->setMaxLength(32767);
		lineEdit->setStripLeadingZeros(false);
		lineEdit->setInputMask("BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB;0");
	}
}
