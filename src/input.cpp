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

#include "input.h"
#include "narrowlineedit.h"
#include <QLabel>
#include <QHBoxLayout>
#include "gui.h"

Input::Input(int base, Mode mode, const QString& labelText, bool stripLeadingZeros, const QString &prefix) :
	base(base),
	prefix(prefix),
	labelText(labelText),
	lineEdit(new NarrowLineEdit(prefix, stripLeadingZeros)),
	label(new QLabel()),
	mode(mode)
{
	QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
	lineEdit->setGraphicsEffect(effect);
	effect->setOpacity(1);
	blinkAnim = new QPropertyAnimation(effect,"opacity");
	blinkAnim->setDuration(300);
	blinkAnim->setStartValue(0);
	blinkAnim->setEndValue(1);
	blinkAnim->setEasingCurve(QEasingCurve::OutQuad);
}

void Input::displayValueChanged(const Value &value, bool userInput)
{
	this->value = value;

	//line edit currently being used ignores new values
	if(lineEdit->hasFocus() && userInput) {
		return;
	}

	if(mode == Mode::Signed) {
		const QString sign = value.intVal < 0 ? "-" : "";
		lineEdit->setText(sign + prefix + QString::number(std::abs(value.intVal), base));
	}
	else if(mode == Mode::Unsigned) {
		lineEdit->setText(prefix + QString::number(value.uIntVal, base));
	}
	else if(mode == Mode::TwosComp) {
		auto nosign = reinterpret_cast<const CalcUInt*>(&(value.intVal));
		lineEdit->setText(prefix + QString::number(*nosign, base));
	}
	else { //mode Float
		if(base == 10) {
			lineEdit->setText(prefix + QString::number(value.floatVal, 'G', GUI::DECIMAL_DISP_LEN));
		}
		else {
			auto nosign = reinterpret_cast<const CalcUInt*>(&(value.floatVal));
			lineEdit->setText(prefix + QString::number(*nosign, base));
		}
	}
}

void Input::setNextInputClears(bool inputClears)
{
	if(inputClears) {
		blink();
	}
	lineEdit->setNextInputClears(inputClears);
}

void Input::digitEdit(const QString&)
{
	const QString inputText {lineEdit->getStrippedInput()};
	Value RHS;

	bool conversionOk = false;
	if(mode == Mode::Signed) {
		CalcInt val {inputText.toLongLong(&conversionOk, base)};
		RHS = Value(val, static_cast<CalcDouble>(val), static_cast<CalcUInt>(val));
	}
	else if (mode == Mode::Unsigned) {
		CalcUInt val {inputText.toULongLong(&conversionOk, base)};
		RHS = Value(static_cast<CalcInt>(val), static_cast<CalcDouble>(val), val);
	}
	else if(mode == Mode::TwosComp){ //twos comp
		CalcUInt unsignedVal {inputText.toULongLong(&conversionOk, base)};
		CalcInt val = Value::rawToInt(unsignedVal);
		RHS = Value(val, static_cast<CalcDouble>(val), unsignedVal);
	}
	else if(mode == Mode::Float) {
		if(base == 10) {
			CalcDouble val = inputText.toDouble();
			RHS = Value(static_cast<CalcInt>(val), val, static_cast<CalcUInt>(val));
		}
		else {
			CalcUInt unsignedVal = inputText.toULongLong(&conversionOk, base);
			CalcDouble val = Value::rawToFloat(unsignedVal);
			RHS = Value(static_cast<CalcInt>(val), val, static_cast<CalcUInt>(val)); //todo what to do here with float to int overflows?
		}
	}

	emit inputChanged(RHS, base, mode);
}

void Input::lineEditFocus(bool)
{
//	if(focused) {

//		emit inputChanged(getStrippedInput(), base);
//	}
}

void Input::updateValidator()
{

}

void Input::updateLabelText()
{
	QString signModeName;

	switch (mode) {
		case Mode::Float:
			signModeName = tr("float");
			break;
		case Mode::Signed:
			signModeName = tr("signed");
			break;
		case Mode::Unsigned:
			signModeName = tr("unsigned");
			break;
		default:
		case Mode::TwosComp:
			signModeName = tr("2's comp");
			break;
	}

	label->setText(signModeName + "\n" + labelText);
}

void Input::blink()
{
	blinkAnim->stop();
	blinkAnim->start();
}

void Input::inputDigits(const QString &digit)
{
	lineEdit->setFocus();
	lineEdit->insert(digit);
}

void Input::unfocus() const
{
	lineEdit->clearFocus();
}

void Input::focus() const
{
	lineEdit->setFocus();
	lineEdit->setCursorPosition(lineEdit->text().length());
}

bool Input::hasFocus() const
{
	 return lineEdit->hasFocus();
}

/**
 * @brief Set mode creates a new validator for this input field based on the mode.
 * Also stores the mode to be used for value to string conversion
 * @param mode
 */
void Input::setMode(Mode mode)
{
	this->mode = mode;
	updateValidator();
	updateLabelText();
	displayValueChanged(value, false); //update displayed value
}

void Input::setBitWidth(BitWidth bitWidth)
{
	this->bitWidth = bitWidth;
}
