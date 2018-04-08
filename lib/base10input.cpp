#include "base10input.h"

Base10Input::Base10Input(Mode mode, const QString& labelText) :
	BasicInput(10, mode, labelText)
{
}

void Base10Input::setReprMode(ReprMode mode)
{
	//Do nothing, base 10 is always human readable
}
