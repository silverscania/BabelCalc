#include "calculationhistoryview.h"
#include "value.h"

CalculationHistoryView::CalculationHistoryView()
{
	setReadOnly(true);
	setText("tftfyjtfyjtf\nydytdytdytd\tftyf\nyttyd");
}

void CalculationHistoryView::setInput(const Value &input)
{
	setText(QString::number(input.intVal));
}
