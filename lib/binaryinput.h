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

#include "input.h"

class QLabel;

class BinaryInput : public Input
{
	public:
		BinaryInput(Mode mode, ReprMode reprMode);
		void setReprMode(ReprMode mode) override;

	public slots:
		void displayValueChanged(const Value& value, bool userInput) override;

	protected:
		void updateValidator() override;

	private:
		QLabel *binaryMarker;
};

