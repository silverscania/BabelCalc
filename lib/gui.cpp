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

#include <cmath>

#include "button.h"
#include "calculationhistoryview.h"
#include "gui.h"
#include "calculator.h"
#include <QButtonGroup>
#include "horizontalbuttongroup.h"
#include "basicinput.h"
#include "base10input.h"
#include "binaryinput.h"
#include "narrowlineedit.h"

GUI::GUI(Calculator* calc, QWidget* parent)
	: QMainWindow(parent),
	  calculator(calc),
	  lastIntMode(Mode::Signed),
	  lastReprMode(ReprMode::Human),
	  settings("Bits Of Beards", "BabelCalc")
{
	//purple transparent gradient effect TODO use for windows
	//	background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,\
	//									  stop: 0 #f854364b, stop: 1 #cc9d9e9d);

	QFile baseStyle(":/style/style.qss");
	baseStyle.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(baseStyle.readAll());

	//Append windows style
#ifndef Q_OS_MAC
	QFile winStyle(":/style/windowsstyle.qss");
	winStyle.open(QFile::ReadOnly);
	styleSheet += QLatin1String(winStyle.readAll());
#endif

	setStyleSheet(styleSheet);

	QWidget *mainWidget = new QWidget;
	setCentralWidget(mainWidget);

	mainLayout = new QHBoxLayout;
	mainLayout->setSpacing(0);
	mainLayout->setMargin(0);

	mainWidget->setLayout(mainLayout);
	mainWidget->setObjectName("Window");

	mainCalculatorLayout = new QVBoxLayout;
	mainCalculatorLayout->setSpacing(0);
	mainCalculatorLayout->setMargin(0);

	mainLayout->addLayout(mainCalculatorLayout);

	calculationHistoryView = new CalculationHistoryView;
	mainLayout->addWidget(calculationHistoryView);

	//setWindowFlags(Qt::Widget | Qt::FramelessWindowHint); //commented: (makes window transparent on windows)
	//setAttribute(Qt::WA_NoSystemBackground, true);

#ifdef Q_OS_MAC
	setAttribute( Qt::WA_TranslucentBackground );
#endif

	int rowCount = 0;

	QFontDatabase::addApplicationFont ( ":/font/Source Code Pro No Dot ExtraLight.ttf" );
	QFontDatabase::addApplicationFont ( ":/font/Source Code Pro No Dot Light.ttf" );

	auto cutAct = new QAction(tr("&History Window"), nullptr);
	cutAct->setCheckable(true);
	cutAct->setStatusTip(tr("Cut the current selection's contents to the "
						   "clipboard"));

	//menu bar
	auto fileMenu = menuBar()->addMenu(tr("&View"));
	fileMenu->addAction(cutAct);

	settingsMenu = menuBar()->addMenu(tr("&Settings"));
	settingsMenu->addSeparator()->setText(tr("Bases"));

	{
		settingsMenuSeparator = settingsMenu->addSeparator();
		auto custom = new QAction(tr("Custom"), nullptr);
		settingsMenu->addAction(custom);
		connect(custom, &QAction::triggered, this, &GUI::openCustomBaseDialog);

		auto clear = new QAction(tr("Clear Unused Inputs"), nullptr);
		settingsMenu->addAction(clear);
		connect(clear, &QAction::triggered, this, &GUI::clearCustomBaseList);
	}

	addNewInputBase(new Base10Input(/*10,*/ Mode::Signed, inputBaseToString(10)), false, true);
	addNewInputBase(new BasicInput(16, Mode::Signed, inputBaseToString(16), "0x"), false, true);
	addNewInputBase(new BasicInput(8, Mode::Signed, inputBaseToString(8)), true, false);
	addNewInputBase(new BinaryInput(Mode::Signed), false, true);
	//setInputBaseEnabled(8, false); //octal off by default

	for(int base = 2; base <= 26; ++base) {
		const QString baseName = QString("base-%1").arg(base); //dont translate settings keys
		if(settings.contains(baseName)) {
			const bool enabled = settings.value(baseName, false).toBool();
			if(findInputForBase(base) == nullptr) {
				addNewInputBase(new BasicInput(base, Mode::Signed, inputBaseToString(base)), true, enabled);
			}

			setInputBaseEnabled(base, enabled);
		}
	}

	QWidget* toggleButtonRow = new QWidget;
	toggleButtonRow->setObjectName("ToggleButtonRow");
	toggleButtonRow->setLayout(new QHBoxLayout);
	toggleButtonRow->layout()->setSpacing(0);
	toggleButtonRow->layout()->setMargin(0);
	toggleButtonRow->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
	mainCalculatorLayout->addWidget(toggleButtonRow);


	QSizePolicy toggleSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);

	//Int / float switch
	{
		HorizontalButtonGroup* groupBox = new HorizontalButtonGroup;

		QPushButton *radio1 = new QPushButton(tr("int"));
		QPushButton *radio2 = new QPushButton(tr("float"));
		radio1->setSizePolicy(toggleSizePolicy);
		radio2->setSizePolicy(toggleSizePolicy);
		groupBox->addButton(radio1);
		groupBox->addButton(radio2);
		groupBox->layout()->setAlignment(radio2, Qt::AlignLeft);
		radio1->setChecked(true);
		groupBox->setSizePolicy(toggleSizePolicy);
		connect(radio1, &QRadioButton::toggled, [=](bool toggled){if(toggled) {
				calculator->setMode(lastIntMode);
				for(const auto inp : inputs) {
					if(inp.first == 10)
						inp.second->setMode(Mode::Signed);
					else
						inp.second->setMode(lastIntMode);
				}
				intModeToggleGroup->setEnabled(true);
				floatModeToggleGroup->setEnabled(false);
			}});
		connect(radio2, &QRadioButton::toggled, [=](bool toggled){if(toggled) {
				calculator->setMode(Mode::Float);
				for(const auto inp : inputs) inp.second->setMode(Mode::Float);
				intModeToggleGroup->setEnabled(false);
				floatModeToggleGroup->setEnabled(true);
			}});
		toggleButtonRow->layout()->addWidget(groupBox);
	}

	//32/64 bit toggle
	{
		HorizontalButtonGroup *groupBox = new HorizontalButtonGroup();
		QPushButton *radio1 = new QPushButton(tr("32bit"));
		QPushButton *radio2 = new QPushButton(tr("64bit"));
		radio1->setSizePolicy(toggleSizePolicy);
		radio2->setSizePolicy(toggleSizePolicy);

		groupBox->addButton(radio1);
		groupBox->addButton(radio2);
		//groupBox->layout()->setAlignment(radio1, Qt::AlignLeft);
		groupBox->layout()->setAlignment(radio2, Qt::AlignLeft);

		radio1->setChecked(true);

		groupBox->setSizePolicy(toggleSizePolicy);
		//TODO:
		//connect(radio2, &QPushButton::toggled, [=](bool toggled){if(toggled) calculator->setInputBase(InputBase::Dec);});
		//connect(radio3, &QPushButton::toggled, [=](bool toggled){if(toggled) calculator->setInputBase(InputBase::Bin);});
		toggleButtonRow->layout()->addWidget(groupBox);
	}

	//signed unsigned
	{
		intModeToggleGroup = new HorizontalButtonGroup();
		QPushButton *radio1 = new QPushButton(tr("unsigned"));
		QPushButton *radio2 = new QPushButton(tr("signed"));
		radio1->setSizePolicy(toggleSizePolicy);
		radio2->setSizePolicy(toggleSizePolicy);
		intModeToggleGroup->addButton(radio1);
		intModeToggleGroup->addButton(radio2);
		//groupBox->layout()->setAlignment(radio1, Qt::AlignLeft);

		intModeToggleGroup->layout()->setAlignment(radio2, Qt::AlignLeft);


		radio2->setChecked(true);
		//groupBox->setSizePolicy(toggleSizePolicy); //comment this so last one expands to fill remaining space
		connect(radio1, &QRadioButton::toggled, [=](bool toggled) { if(toggled) {
				lastIntMode = Mode::Unsigned;
				calculator->setMode(Mode::Unsigned);
				for(const auto inp : inputs) {
					inp.second->setMode(Mode::Unsigned);
				}
		}});

		connect(radio2, &QRadioButton::toggled, [=](bool toggled){ if(toggled) {
				lastIntMode = Mode::Signed;
				calculator->setMode(Mode::Signed);
				for(const auto inp : inputs) {
					inp.second->setMode(Mode::Signed);
				}
		}});

		toggleButtonRow->layout()->addWidget(intModeToggleGroup);
	}

	//float: actual or binary representation
	{
		floatModeToggleGroup = new HorizontalButtonGroup();
		QPushButton *radio1 = new QPushButton(tr("human repr"));
		QPushButton *radio2 = new QPushButton(tr("machine repr"));
		radio1->setSizePolicy(toggleSizePolicy);
		radio2->setSizePolicy(toggleSizePolicy);
		floatModeToggleGroup->addButton(radio1);
		floatModeToggleGroup->addButton(radio2);

		floatModeToggleGroup->layout()->setAlignment(radio2, Qt::AlignLeft);
		floatModeToggleGroup->setEnabled(false);

		radio1->setChecked(true);
		//groupBox->setSizePolicy(toggleSizePolicy); //comment this so last one expands to fill remaining space
		connect(radio1, &QRadioButton::toggled, [=](bool toggled) { if(toggled) {
				lastReprMode = ReprMode::Human;
				//calculator->setMode(Mode::Unsigned);
				for(const auto inp : inputs) {
					inp.second->setFloatMode(ReprMode::Human);
				}
		}});

		connect(radio2, &QRadioButton::toggled, [=](bool toggled){ if(toggled) {
				lastReprMode = ReprMode::Machine;
				//calculator->setMode(Mode::Signed);
				for(const auto inp : inputs) {
					inp.second->setFloatMode(ReprMode::Machine);
				}
		}});

		toggleButtonRow->layout()->addWidget(floatModeToggleGroup);
	}

	//deg/rad bit toggle
	{
		HorizontalButtonGroup *groupBox = new HorizontalButtonGroup();
		QPushButton *radio1 = new QPushButton(tr("degrees"));
		QPushButton *radio2 = new QPushButton(tr("radians"));
		radio1->setSizePolicy(toggleSizePolicy);
		radio2->setSizePolicy(toggleSizePolicy);

		groupBox->addButton(radio1);
		groupBox->addButton(radio2);
		//groupBox->layout()->setAlignment(radio1, Qt::AlignLeft);
		groupBox->layout()->setAlignment(radio2, Qt::AlignLeft);

		radio1->setChecked(true);
		groupBox->setSizePolicy(toggleSizePolicy);

		connect(radio1, &QPushButton::toggled, [=](bool toggled){if(toggled) calculator->setAngleMode(AngleMode::Degrees);});
		connect(radio2, &QPushButton::toggled, [=](bool toggled){if(toggled) calculator->setAngleMode(AngleMode::Radians);});
		toggleButtonRow->layout()->addWidget(groupBox);
	}

	//toggle button row end padding
	{
		QWidget* pad = new QWidget;
		pad->setLayout(new QHBoxLayout);
		pad->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
		toggleButtonRow->layout()->addWidget(pad);
	}

	///Start grid layout
	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
	gridLayout->setMargin(0);
	gridLayout->setSpacing(0);

	//Left Buttons
	{
		int row = rowCount;
		gridLayout->addWidget(createButton<AndOperator>(tr("AND")), row, 0);
		gridLayout->addWidget(createButton<OrOperator>(tr("OR")), row++, 1);
		gridLayout->addWidget(createButton<NorOperator>(tr("NOR")), row, 0);
		gridLayout->addWidget(createButton<XorOperator>(tr("XOR")), row++, 1);

		gridLayout->addWidget(createButton<ShiftLeftOperator>(tr("<<")), row, 0);
		gridLayout->addWidget(createButton<ShiftRightOperator>(tr(">>")), row++, 1);
		gridLayout->addWidget(createButton<ShiftLeftByOperator>(tr("<< x")), row, 0);
		gridLayout->addWidget(createButton<ShiftRightByOperator>(tr(">> x")), row++, 1);

		gridLayout->addWidget(createButton<NotOperator>(tr("NOT")), row, 0);
		gridLayout->addWidget(createButton<NandOperator>(tr("NAND")), row++, 1);

		gridLayout->addWidget(createButton<ByteReverseOperator>(tr("rev")), row, 0);
		gridLayout->addWidget(createButton<SwapHalvesOperator>(tr("swap")), row++, 1);
	}

	//Center Buttons
	{
		int row {rowCount};
		int col {5};

		gridLayout->addWidget(createButton<NegationOperator>		(tr("+/−")), row, col++);
		gridLayout->addWidget(createButton<ModulusOperator>			(tr("mod")), row, col++);
		gridLayout->addWidget(createButton							(tr("("), [=] {calculator->inputOpenBracket();}), row, col++);
		gridLayout->addWidget(createButton							(tr(")"), [=] {calculator->inputCloseBracket();}), row, col++);
		gridLayout->addWidget(createButton<SquaredOperator>			(tr("x²")), row, col++);
		gridLayout->addWidget(createButton<PowerOperator>			(tr("xⁿ")), row, col++);
		gridLayout->addWidget(createButton<ASinOperator>			(tr("sin⁻¹")), row++, col++);

		col = 5;
		gridLayout->addWidget(createButton<RotateLeftOperator>		(tr("rot<<")), row, col++);
		gridLayout->addWidget(createButton<RotateRightOperator>		(tr("rot>>")), row, col++);
		gridLayout->addWidget(createButton<OneOverOperator>			(tr("1/x")), row, col++);
		gridLayout->addWidget(createButton<RootOperator>			(tr("√x")), row, col++);
		gridLayout->addWidget(createButton<CubeRootOperator>		(tr("3√x")), row, col++);
		gridLayout->addWidget(createButton<NRootOperator>			(tr("n√x")), row, col++);
		gridLayout->addWidget(createButton<ACosOperator>			(tr("cos⁻¹")), row++, col++);

		col = 5;
		gridLayout->addWidget(createButton(							tr("AC"), [=] {calculator->inputC(); calculator->inputC();}, "Action"), row, col++);
		gridLayout->addWidget(createButton(							tr("C"), [=] {calculator->inputC();}, "Action"), row, col++);
		gridLayout->addWidget(createButton<FactorialOperator>		(tr("x!")), row, col++);
		gridLayout->addWidget(createButton<SinOperator>				(tr("sin")), row, col++);
		gridLayout->addWidget(createButton<CosOperator>				(tr("cos")), row, col++);
		gridLayout->addWidget(createButton<TanOperator>				(tr("tan")), row, col++);
		gridLayout->addWidget(createButton<ATanOperator>			(tr("tan⁻¹")), row++, col++);

		col = 5;
		gridLayout->addWidget(createButton<DivisionOperator>		(tr("÷"), "Action"), row, col++);
		gridLayout->addWidget(createButton<SubtractionOperator>		(tr("-"), "Action"), row, col++);
		gridLayout->addWidget(createButton<SinhOperator>			(tr("sinh")), row, col++);
		gridLayout->addWidget(createButton<CoshOperator>			(tr("cosh")), row, col++);
		gridLayout->addWidget(createButton<TanhOperator>			(tr("tanh")), row, col++);
		gridLayout->addWidget(createButton<LogNOperator>			(tr("log\u2099x")), row, col++);
		gridLayout->addWidget(createButton<ASinhOperator>			(tr("sinh⁻¹")), row++, col++);

		col = 5;
		gridLayout->addWidget(createButton<MultiplicationOperator>	(tr("x"), "Action"), row, col++);
		gridLayout->addWidget(createButton<AdditionOperator>		(tr("+"), "Action"), row, col++);
		gridLayout->addWidget(createButton(							tr("rand"), [=] {calculator->inputRandomNumber();}), row, col++);
		gridLayout->addWidget(createButton(							tr("EE"),	[=] {findInputForBase(10)->inputDigits("e");}), row, col++);
		gridLayout->addWidget(createButton<Log10Operator>			(tr("log₁₀")), row, col++);
		gridLayout->addWidget(createButton<TenPowOperator>			(tr("10ˣ")), row, col++);
		gridLayout->addWidget(createButton<ACoshOperator>			(tr("cosh⁻¹")), row++, col++);

		col = 5;
		gridLayout->addWidget(createButton(							tr("="), [=] { calculator->inputEquals(); }, "Action"), row, col++, 1, 2); col++;
		gridLayout->addWidget(createButton<PiOperator>				(tr("π")), row, col++);
		gridLayout->addWidget(createButton<eOperator>				(tr("e")), row, col++);
		gridLayout->addWidget(createButton<lnOperator>				(tr("ln")), row, col++);
		gridLayout->addWidget(createButton<ePowXOperator>			(tr("eˣ")), row, col++);
		gridLayout->addWidget(createButton<ATanhOperator>			(tr("tanh⁻¹")), row++, col++);
	}

	//Hex digits
	{
		const QString style { "Hex" };
		const int startColumn {2};
		const int startRow {rowCount};
		for (int i = 1; i < 16; ++i) {
			int row = ((15 - i) / 3) + startRow;
			int column = ((i - 1) % 3) + startColumn;
			const auto str = QString::number(i, 16).toUpper();
			auto button = createButton(str, [=] { inputDigit(str); }, style);
		//	button->setFont(sourceCodeLight);
			gridLayout->addWidget(
				button,
				row,
				column
			);
		}

		gridLayout->addWidget(
					createButton("FF", [=] { inputDigit("FF"); }, style),
					startRow+5,
					startColumn
					);
		gridLayout->addWidget(
					createButton("0", [=] { inputDigit("0"); }, style),
					startRow+5,
					startColumn+1
					);
		gridLayout->addWidget(
					createButton("00", [=] { inputDigit("00"); }, style),
					startRow+5,
					startColumn+2
					);
	}

	QWidget* gridWidg = new QWidget;
	gridWidg->setObjectName("MainGrid");
	gridWidg->setLayout(gridLayout);
	mainCalculatorLayout->addWidget(gridWidg);

	installEventFilter(this);
}

QString GUI::inputBaseToString(int base) {
	switch(base) {
		case 2:		return tr("binary");
		case 8:		return tr("octal");
		case 10:	return tr("decimal");
		case 16:	return tr("hex");
		default:	return tr("base ") + QString::number(base);
	}
}

void GUI::inputDigit(const QString &str) {
	auto* const focusedInput = findFocusedInput();

	if(focusedInput == nullptr) {
		qWarning("No input in focus, cannot input digit");
		return;
	}

	focusedInput->inputDigits(str);
}

void GUI::addNewInputBase(Input* input, bool canDisable, bool enabled)
{
	connect(calculator, &Calculator::displayValueChanged, input, &Input::displayValueChanged);
	connect(calculator, &Calculator::nextInputClearsChanged, input, &Input::setNextInputClears);
	connect(input, &Input::inputChanged, calculator, &Calculator::setInput);
	connect(input, &Input::inputChanged, calculationHistoryView, &CalculationHistoryView::setInput);
	input->setObjectName("Input");
	inputs[input->base] = input;

	if(enabled) {
		const int index = mainCalculatorLayout->count() - 3;
		mainCalculatorLayout->insertWidget(index, input);
	}

	//menu item
	QString menuLabel = input->labelText;
	menuLabel[0] = menuLabel[0].toUpper();
	auto dec = new QAction(menuLabel, nullptr);
	dec->setCheckable(true);
	dec->setChecked(enabled);
	dec->setEnabled(canDisable);

	connect(dec, &QAction::toggled, [=](bool toggled) {
		setInputBaseEnabled(input->base, toggled);
		saveInputBaseEnabled(input->base, toggled);
	});

	settingsMenu->insertAction(settingsMenuSeparator, dec);
	settingsMenuBases[input->base] = dec;
}

void GUI::setInputBaseEnabled(int base, bool enabled)
{
	Input* inp = findInputForBase(base);
	if(inp) {
		if(enabled) {
			const int index = mainCalculatorLayout->count() - 3;
			mainCalculatorLayout->insertWidget(index, inp);
		}
		else {
			mainCalculatorLayout->removeWidget(inp);
			inp->setParent(nullptr);
		}
	}
}

void GUI::clearCustomBaseList()
{
	for(auto it = settingsMenuBases.begin(); it != settingsMenuBases.end(); ) {
		if (it->second->isChecked() == false) {
			//remove input
			setInputBaseEnabled(it->first, false);

			//delete input
			auto inputIt = inputs.find(it->first);
			delete inputIt->second;
			inputs.erase(inputIt);

			//remove save key
			const QString key = QString("base-%1").arg(it->first); //dont translate settings keys
			settings.remove(key);

			//remove menu item
			settingsMenu->removeAction(it->second);
			delete it->second;
			it = settingsMenuBases.erase(it);
		}
		else {
			++it;
		}
	}
}

void GUI::saveInputBaseEnabled(int base, bool enabled)
{
	const QString key = QString("base-%1").arg(base); //dont translate settings keys
	settings.setValue(key, enabled);
}

bool GUI::eventFilter(QObject *obj, QEvent *event) {
	if (event->type()==QEvent::KeyPress) {
		QKeyEvent* key = static_cast<QKeyEvent*>(event);
		switch(key->key()) {
			case Qt::Key_Enter:
			case Qt::Key_Return:
				calculator->inputEquals();
				break;

			case Qt::Key_Minus:
				calculator->inputOperator(std::make_unique<SubtractionOperator>());
				break;

			case Qt::Key_Plus:
				calculator->inputOperator(std::make_unique<AdditionOperator>());
				break;

			case Qt::Key_Asterisk:
			case Qt::Key_multiply:
				calculator->inputOperator(std::make_unique<MultiplicationOperator>());
				break;

			case Qt::Key_Slash:
			case Qt::Key_division:
				calculator->inputOperator(std::make_unique<DivisionOperator>());
				break;

			case Qt::Key_Delete:
				calculator->inputC();
				break;

			case Qt::Key_ParenLeft:
				calculator->inputOpenBracket();
				break;

			case Qt::Key_ParenRight:
				calculator->inputCloseBracket();
				break;

			default:
				return QObject::eventFilter(obj, event);
		}
		return true;
	}

	return QObject::eventFilter(obj, event);
}

void GUI::openCustomBaseDialog()
{
	bool ok = false;
	int base = QInputDialog::getInt(this, tr("Add a new input base"),
									tr("Base [2 - 26]:"), 2, 2, 26, 1, &ok);
	if (ok && !findInputForBase(base)) {

		addNewInputBase(new BasicInput(base, lastIntMode, inputBaseToString(base)), true, true);
		saveInputBaseEnabled(base, true);
	}
//		integerLabel->setText(tr("%1%").arg(i));
}

Input* GUI::findFocusedInput() {
	auto binInput = std::find_if(inputs.begin(), inputs.end(), [=] (auto x) { return x.second->hasFocus();});
	if(binInput != inputs.end()) {
		return binInput->second;
	}
	else {
		return nullptr;
	}
}

Input* GUI::findInputForBase(int base) {
	//auto binInput = std::find_if(inputs.begin(), inputs.end(), [=] (Input* x) { return x->base == base;});
	const auto it = inputs.find(base);
	if(it != inputs.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}


void GUI::unfocusAllInputs()
{
	for(const auto inp : inputs) {
		inp.second->unfocus();
	}
}

template <class T>
Button* GUI::createButton(const QString &text, T callback, const QString& name) {
	Button *button = new Button(text);
	connect(button, &Button::clicked, callback);
	button->setObjectName(name);
	button->setProperty("right", true);
	button->setProperty("bottom", true);
    button->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	return button;
}

template <class T>
Button* GUI::createButton(const QString &text, const QString& name)
{
	Button *button = new Button(text);
	connect(button, &Button::clicked, [=]() {
		calculator->inputOperator(std::make_unique<T>());
	});
	button->setObjectName(name);
	button->setProperty("right", true);
	button->setProperty("bottom", true);
    button->setFocusPolicy(Qt::FocusPolicy::NoFocus);

	button->style()->unpolish(button);
	button->style()->polish(button);
	return button;
}
