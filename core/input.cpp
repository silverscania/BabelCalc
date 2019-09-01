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

Input::Input(int base, Mode mode, ReprMode reprMode, const QString& labelText, bool stripLeadingZeros, const QString &prefix) :
	base(base),
	prefix(prefix),
	labelText(labelText),
	lineEdit(new NarrowLineEdit(prefix, stripLeadingZeros)),
	label(new QLabel()),
	mode(mode),
	reprMode(reprMode)
{

	// For some reason on a static build, setting opacity to 1 makes the input fields invisible...
	// Use 0.99 instead :(
	QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
	effect->setOpacity(0.99);
	lineEdit->setGraphicsEffect(effect);

//QTextEdit::setCursorWidth()
	blinkAnim = new QPropertyAnimation(effect,"opacity");
	blinkAnim->setDuration(300);
	blinkAnim->setStartValue(0);
	blinkAnim->setEndValue(0.99); // See comment above
	blinkAnim->setEasingCurve(QEasingCurve::OutQuad);
}

void Input::displayValueChanged(const Value &value, bool userInput)
{
	this->value = value;

	//line edit currently being used ignores new values
	if(lineEdit->hasFocus() && userInput) {
		return;
	}

	switch (reprMode) {
		case ReprMode::Human:
			setDisplayValueHuman(value);
			break;
		case ReprMode::Machine:
			setDisplayValueMachine(value);
			break;
	}
}

/**
 * Signed values are displayed with the +- signs.
 * Unsigned values are displayed as integers
 * Float values are displayed as floating point in any base
 */
void Input::setDisplayValueHuman(const Value& value) {
	switch (mode) {
		case Mode::Signed: {
			// Work out our own - sign so we can put the prefix inbetween the - sign and the number.
			const QString sign = value.intVal < 0 ? "-" : "";
			lineEdit->setText(sign + prefix + QString::number(std::abs(value.intVal), base));
			break;
		}
		case Mode::Unsigned:
			lineEdit->setText(prefix + QString::number(value.uIntVal, base));
			break;

		case Mode::Float:
			lineEdit->setText(floatToString(value.floatVal, base, prefix));
			break;
	}
}

/**
 * Signed values are displayed as twos complement
 * Unsigned values are displayed raw
 * Floating point values are displayed in IEE-745 in all bases
 */
void Input::setDisplayValueMachine(const Value& value) {
	switch (mode) {
		case Mode::Signed:
			lineEdit->setText(prefix + QString::number(Value::intToRaw(value.intVal), base));
			break;

		case Mode::Unsigned:
			lineEdit->setText(prefix + QString::number(value.uIntVal, base));
			break;

		case Mode::Float:
			lineEdit->setText(prefix + QString::number(Value::floatToRaw(value.floatVal), base));
			break;
	}
}

CalcFloat Input::stringToFloat(QString string, int base, bool* ok)
{
	Q_ASSERT(ok);
	*ok = true;
	//TODO: must have for MVP
	//TODO: validation should use the ok parameter of this function

	// Prepend with "0" in case the string is empty or starts with "."
	string = "0" + string;

	if(base == 10) {
		// Use Qt for support of e notation which is only available in
		// base 10.
		return string.toFloat(ok);
	}

	// Split on "." to get units and fractional part. "." is optional
	const QStringList split = string.split(".");
	if(split.length() > 2) {
		*ok = false;
		return 0.0f;
	}
	const QString units = split[0];
	const QString fractions = split.length()>1 ? split[1] : "0";

	CalcFloat value = 0;

	// Convert units
	for(int i = 0; i < units.length(); ++i) {
		const CalcFloat floatBase = static_cast<CalcFloat>(base);
		const CalcFloat multiplier = floatBase * units.length() - i - 1;
		const int digit = units.midRef(i,1).toInt(ok, base);
		if(*ok == false) {
			return 0.0f;
		}
		value += digit *  multiplier;
	}

	return value;
}

QString Input::floatToString(CalcFloat value, int base, const QString& prefix)
{
	const int MAX_DECIMALS = 6;
	const int MIN_DECIMALS = 1;
	// Take sign, and then make it +ve for the rest of the function
	const QString sign = value < 0 ? "-" : "";
	value = fabs(value);

	// Start with integer part and "."
	QString string = QString::number((CalcUInt) floor(value), base) + ".";

	// Stop once MAX_DIGITS is reached, or the exact value was represented
	// TODO: support e notation
	bool ok = false;
	for(int i = 0; i < MIN_DECIMALS ||
		(i < MAX_DECIMALS && stringToFloat(sign + string, base, &ok) == value); ++i)
	{
		if(ok == false) {
			break;
		}

		CalcFloat fractionAsInteger = floor(value * pow((double) base, i));
		QString fractionDigit = QString::number((CalcInt)floor(fractionAsInteger), base);

		// Take the last character. Anything higher is from previous digits
		string += fractionDigit.right(1);
	}

	return sign + prefix + string;
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

	switch (reprMode) {
		case ReprMode::Human:
			emit inputChanged(humanInputToValue(inputText), base, mode);
			break;

		case ReprMode::Machine:
			emit inputChanged(machineInputToValue(inputText), base, mode);
			break;
	}
}

/**
 * Convert text entered by user in human mode to a value. Prefix
 * should already have been stripped before calling this.
 */
Value Input::humanInputToValue(const QString& inputText)
{
	bool conversionOk = false;
	switch(mode) {
		case Mode::Signed: {
			CalcInt val {inputText.toLongLong(&conversionOk, base)};
			return Value(val, static_cast<CalcFloat>(val), static_cast<CalcUInt>(val));
		}
		case Mode::Unsigned: {
			CalcUInt val {inputText.toULongLong(&conversionOk, base)};
			return Value(static_cast<CalcInt>(val), static_cast<CalcFloat>(val), val);
		}
		case Mode::Float: {
			bool ok = false;
			// TODO: pass ok out of this function (or use exceptions?)
			CalcFloat val {stringToFloat(inputText, base, &ok)};
			return Value(static_cast<CalcInt>(val), static_cast<CalcFloat>(val), val);
		}
	}
}

Value Input::machineInputToValue(const QString& inputText)
{
	// All inputs take raw value in machine mode
	bool conversionOk = false;
	const CalcUInt unsignedVal {inputText.toULongLong(&conversionOk, base)};

	switch(mode) {
		case Mode::Signed: {
			CalcInt val = Value::rawToInt(unsignedVal);
			return Value(val, static_cast<CalcFloat>(val), unsignedVal);
		}
		case Mode::Unsigned: {
			return Value(static_cast<CalcInt>(unsignedVal), static_cast<CalcFloat>(unsignedVal), unsignedVal);
		}
		case Mode::Float: {
			CalcFloat val = Value::rawToFloat(unsignedVal);
			// TODO: should all these be cast? Or raw converted?
			return Value(static_cast<CalcInt>(val), val, static_cast<CalcUInt>(val)); //todo what to do here with float to int overflows?
		}
	}
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
	QString modeName;

	switch (mode) {
		case Mode::Float:
			modeName = tr("float");
			break;
		case Mode::Signed:
			modeName = tr("signed");
			break;
		case Mode::Unsigned:
			modeName = tr("unsigned");
			break;
	}

	switch (reprMode) {
		case ReprMode::Human:
			modeName += " - human";
			break;
		case ReprMode::Machine:
			modeName += " - machine";
			break;
	}

	label->setText(modeName + "\n" + labelText);
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

void Input::setReprMode(ReprMode mode)
{
	this->reprMode = mode;
	updateValidator();
	updateLabelText();
	displayValueChanged(value, false); //update displayed value
}

void Input::setBitWidth(BitWidth bitWidth)
{
	this->bitWidth = bitWidth;
}

