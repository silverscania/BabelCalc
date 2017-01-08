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

#include "horizontalbuttongroup.h"

#include <QAbstractButton>
#include <QStyle>
#include <QButtonGroup>
#include <QHBoxLayout>

HorizontalButtonGroup::HorizontalButtonGroup(QWidget *parent) :
	QWidget(parent),
	groupBox(new QButtonGroup),
	hBoxLayout(new QHBoxLayout)
{
	setLayout(hBoxLayout);
	groupBox->setExclusive(true);
	hBoxLayout->setSpacing(0);
	setStyleSheet("background-color: #33FFFFFF; ");
}

void HorizontalButtonGroup::addButton(QAbstractButton *button)
{
	const auto styleEnd { QString("background-color: #CCC; \
								  color: #555; \
								  font-family: \"Source Code Pro\"; \
								   font-weight: 400; \
								   font-size: 10px; \
									margin: 0; \
									padding: 5; \
									spacing: 0; \
									} \
								QPushButton:checked { \
									background-color: #333; \
									color: white; \
									} \
								QPushButton[enabled=\"false\"] { \
									background-color: #777; \
									color: #999; \
								}"
						   )};

	const auto leftStyle { QString("QPushButton { \
					border-top-left-radius: 8px; \
					border-bottom-left-radius: 8px; \
					") + styleEnd };

	const auto centerStyle { QString("QPushButton { \
				   ") + styleEnd };

	const auto rightStyle { QString("QPushButton { \
				  border-top-right-radius: 8px; \
				  border-bottom-right-radius: 8px; \
				  ") + styleEnd };

	button->setCheckable(true);

	if(groupBox->buttons().count() == 0) {
		button->setStyleSheet(leftStyle);
	}
	else if(groupBox->buttons().count() == 1){
		button->setStyleSheet(rightStyle);
	}
	else {
		//set previous button to center type
		groupBox->buttons().back()->setStyleSheet(centerStyle);
		button->setStyleSheet(rightStyle);
	}


	hBoxLayout->addWidget(button);
	groupBox->addButton(button);
}

void HorizontalButtonGroup::setEnabled(bool enabled)
{
	//QWidget::setEnabled(enabled);
	for(auto button : groupBox->buttons()) {
		button->setEnabled(enabled);

		button->style()->unpolish(button);
		button->style()->polish(button);
		//button->setStyleSheet(button->styleSheet()); //force style update
	}
}
