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

#include "calculator.h"
#include <QTime>

Calculator::Calculator()
	: randgen(QDateTime::currentMSecsSinceEpoch())
{
}

void Calculator::setMode(Mode mode)
{
	this->mode = mode;
}

void Calculator::setAngleMode(AngleMode mode)
{
	angleMode = mode;
}

void Calculator::setInput(const Value& input)
{
	if(stack.empty()) {
		//empty, so value goes straight in
		stack.push_back(CalcStackItem(input));
	}
	else {
		if(stack.back().type == CalcStackType::Operator ||
			stack.back().type == CalcStackType::Bracket)
		{
			//if there is an operator or bracket stacked, then add a new value
			stack.push_back(CalcStackItem(input));
		}
		else if(stack.back().type == CalcStackType::Value)
		{
			//if there was a value last, then replace it
			stack.back() = CalcStackItem(input);
		}
	}

	emit nextInputClearsChanged(false);
	emit displayValueChanged(stack.back().val, true);
}

void Calculator::inputEquals()
{
	if(!stack.empty()) {
		//if its a bracket or operator on the end, remove it
		if(stack.back().type == CalcStackType::Operator ||
			stack.back().type == CalcStackType::Bracket)
		{
			stack.pop_back();
		}
	}

	//evaluate if still not empty
	if(!stack.empty()) {
		evaluateAll();

		emit nextInputClearsChanged(true);
		emit displayValueChanged(stack.back().val, false);
	}
}


void Calculator::inputOperator(std::unique_ptr<Operator> operater)
{
	//only allow new operators when there is something in the stack
	if(!stack.empty()) {
		//if there is already an operator stacked, then replace it
		if(stack.back().type == CalcStackType::Operator) {
			stack.back() = CalcStackItem(std::move(operater));
		}
		//if there was a value last, then add new operator
		else if(stack.back().type == CalcStackType::Value) {
			//if it's unary pretend equals button was pressed immediately
			if(auto op = dynamic_cast<UnaryOperator*>(operater.get())) {
				//replace last value with new value
				stack.back().val = op->evaluate(stack.back().val);

				//fire upated text event
				emit displayValueChanged(stack.back().val, false);
			}
			else {
				//not unary so add it as a new operator
				stack.push_back(CalcStackItem(std::move(operater)));
			}
		}
		//if there was a bracket last, do nothing
	}

	emit nextInputClearsChanged(true);
}

void Calculator::inputRandomNumber()
{
	Value RHS;
	if(mode == Mode::Float) {
		std::uniform_real_distribution<CalcFloat> distribution;
		RHS.floatVal = distribution(randgen);
		RHS.intVal = static_cast<CalcInt>(RHS.floatVal);
		RHS.uIntVal = static_cast<CalcUInt>(RHS.intVal);
	}
	else {
		std::uniform_int_distribution<CalcInt> distribution;
		RHS.intVal = distribution(randgen);
		RHS.floatVal = static_cast<CalcFloat>(RHS.intVal);
		RHS.uIntVal = static_cast<CalcUInt>(RHS.intVal);
	}

	//fire upated text event
	setInput(RHS);
}

void Calculator::inputC()
{
	//already cleared?
	if(stack.empty()) {
		return;
	}

	//if theres a value, remove it
	if(stack.back().type == CalcStackType::Value) {
		stack.pop_back();
	}
	//otherwise clear everything
	else {
		stack.clear();
	}

	emit nextInputClearsChanged(false);
	emit displayValueChanged(Value(), false);
}

void Calculator::inputOpenBracket()
{
	//stack.push_back(CalcStackItem());
}

void Calculator::inputCloseBracket()
{

}

void Calculator::evaluateAll()
{
	//todo
	//first find innermost bracket
	//evaluate
	//delete range of bracket
	//loop until only one item left
	auto begin = stack.begin();
	auto end = stack.end();

	//copy range between brackets
	auto bracketSection = std::vector<CalcStackItem>(begin, end);

	//evaluate
	Value val = evaluate(bracketSection);

	//remove range and replace with result value
	auto it = stack.erase(begin, end);
	stack.insert(it, CalcStackItem(val));
}

Value Calculator::evaluate(std::vector<CalcStackItem>& section)
{
	//BODMAS, evaluate in order or precedence 0 - 2
	for(int precedence = 0; precedence <= 2; ++precedence) {
		//evaluate left to right
		for(auto it = section.cbegin(); it != section.cend(); ++it) {
			//only interested in binary operators
			auto op = dynamic_cast<BinaryOperator*>((*it).operater.get());
			if(op == nullptr)
				continue;

			if(op->precedence == precedence) {
				Value val = op->evaluate((*(it-1)).val, (*(it+1)).val);
				//remove value-operator-value tuple
				it = section.erase(it-1, it+2);

				//insert new value
				it = section.insert(it, CalcStackItem(val));
			}
		}
	}

	return section.front().val;
}

CalcStackItem::CalcStackItem(const Value &val) :
	val(val),
	type(CalcStackType::Value)
{
}

CalcStackItem::CalcStackItem(std::shared_ptr<Operator> operater) :
	operater(std::move(operater)),
	type(CalcStackType::Operator)
{
}

CalcStackItem::CalcStackItem() :
	type(CalcStackType::Bracket)
{
}
