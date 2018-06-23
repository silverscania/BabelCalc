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
#include <iostream>

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
			stack.back().type == CalcStackType::OpenBracket)
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
			stack.back().type == CalcStackType::OpenBracket)
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
		std::uniform_real_distribution<CalcDouble> distribution;
		RHS.doubleVal = distribution(randgen);
		RHS.intVal = static_cast<CalcInt>(RHS.doubleVal);
		RHS.uIntVal = static_cast<CalcUInt>(RHS.intVal);
	}
	else {
		std::uniform_int_distribution<CalcInt> distribution;
		RHS.intVal = distribution(randgen);
		RHS.doubleVal = static_cast<CalcDouble>(RHS.intVal);
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
	//only allow brackets when there is an operator
	if(!stack.empty() && stack.back().type == CalcStackType::Operator) {
		stack.push_back(CalcStackItem::createOpenBracket());
		emit nextInputClearsChanged(true);
	}
}

void Calculator::inputCloseBracket()
{
	evaluateLastBracket();
	//fire upated text event
	emit displayValueChanged(stack.back().val, false);
}

void Calculator::evaluateLastBracket()
{
	auto innermostBracket = std::find_if(stack.rbegin(), stack.rend(),
										 [](auto x) { return x.type == CalcStackType::OpenBracket; }
							);

	if(innermostBracket == stack.rend()) {
		return;
	}

	auto start = innermostBracket.base() - 1;

	//copy range between brackets
	auto bracketSection = std::vector<CalcStackItem>(start, stack.end());

	//evaluate
	Value val = evaluate(bracketSection);

	//remove range and replace with result value
	auto it = stack.erase(start, stack.end());
	stack.insert(it, CalcStackItem(val));
}

void Calculator::emitDisplayValueChanged(Value val, bool userInput)
{
	if(bitWidth == BitWidth::SixtyFour)
		emit displayValueChanged(val, userInput);
	else
		emit displayValueChanged(static_cast<val, userInput);
}

void Calculator::evaluateAll()
{
	std::cout << "evaluate all:" << std::endl;
	for(const auto& x : stack) {
		std::cout << x;
	}
	std::cout << std::endl;

	while(stack.size() > 1) {
		//first find innermost bracket
		auto lastBracket = std::find_if(stack.rbegin(), stack.rend(),
								  [](auto x) { return x.type == CalcStackType::OpenBracket; }
					);

		//no brackets, just use whole array
		auto begin = lastBracket == stack.rend() ? stack.begin() : lastBracket.base() - 1;

		//copy range between brackets
		auto bracketSection = std::vector<CalcStackItem>(begin, stack.end());

		//evaluate
		Value val = evaluate(bracketSection);

		//remove range and replace with result value
		auto it = stack.erase(begin, stack.end());
		stack.insert(it, CalcStackItem(val));
	}
}

Value Calculator::evaluate(std::vector<CalcStackItem>& section)
{
	std::cout << "evaluate:" << std::endl;
	for(const auto& x : stack) {
		std::cout << x;
	}
	std::cout << std::endl;

	//BODMAS, evaluate in order or precedence 0 - 2
	for(int precedence = 0; precedence <= 2; ++precedence) {
		//evaluate left to right
		for(auto it = section.cbegin(); it != section.cend(); ++it) {
			//strip brackets
			if((*it).type == CalcStackType::OpenBracket) {
				it = section.erase(it);
			}

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

CalcStackItem CalcStackItem::createOpenBracket()
{
	return CalcStackItem();
}

CalcStackItem::CalcStackItem() :
	type(CalcStackType::OpenBracket)
{
}

std::ostream & operator<<(std::ostream &os, const CalcStackItem &x)
{
	if(x.type == CalcStackType::OpenBracket) {
		os << " ( ";
	}
	else if(x.type == CalcStackType::Operator) {
		os << " " << x.operater->getOperatorString() << " ";
	}
	else if(x.type == CalcStackType::Value) {
		os << x.val.doubleVal;
	}

	return os;
}
