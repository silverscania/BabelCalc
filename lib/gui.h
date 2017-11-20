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

#include <QtWidgets>
#include <memory>
#include "button.h"
#include "calculator.h"
#include <map>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

struct Value;
class Input;
class HorizontalButtonGroup;

class GUI : public QMainWindow
{
    Q_OBJECT

public:
	GUI(Calculator* calc, QWidget* parent = 0);

	static constexpr int DECIMAL_DISP_LEN {20};

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
	void openCustomBaseDialog();

private:
	QString inputBaseToString(int base);

	Input* findFocusedInput();
	Input* findInputForBase(int base);
	void unfocusAllInputs();

	void toggleAngleInputMode();

	void inputDigit(const QString& str);
	void addNewInputBase(Input* input, bool canDisable, bool enabled);
	void setInputBaseEnabled(int base, bool enabled);
	void clearCustomBaseList();
	void saveInputBaseEnabled(int base, bool enabled);

	template <class T>
	Button* createButton(const QString& text, T callback, const QString& name = "Normal");

	template <class T>
	Button* createButton(const QString &text, const QString& name = "Normal");

	QLineEdit* binaryDisplay;
	HorizontalButtonGroup* intModeToggleGroup;
	Calculator* calculator;
	Input* activeInput;
	QMenu* settingsMenu;
	QBoxLayout* mainLayout;
	QAction* settingsMenuSeparator;

	std::map<int, Input*> inputs;
	std::map<int, QAction*> settingsMenuBases;

	Mode lastIntMode;
	QSettings settings;
};

