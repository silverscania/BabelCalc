#ifndef CALCULATIONHISTORYVIEW_H
#define CALCULATIONHISTORYVIEW_H
#include <QTextEdit>

class Value;

class CalculationHistoryView : public QTextEdit
{
	public:
		CalculationHistoryView();
		void setInput(const Value& input);
};

#endif // CALCULATIONHISTORYVIEW_H
