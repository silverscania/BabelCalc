#ifndef BASE10INPUT_H
#define BASE10INPUT_H

#include "basicinput.h"

/**
 * The Base10Input class is a special case of input class. It differs from
 * the BasicInput class in the following ways:
 *
 * * Allows input of floating point numbers. TODO: this should change once human readable floating points are implemented
 * * Ignores the change to machine repr. Base 10 should always be shown as machine readable.
 * * Supports e notation...? TODO: also remove?
 */
class Base10Input : public BasicInput
{
	public:
		Base10Input();
		void setReprMode(ReprMode mode) override;
};

#endif // BASE10INPUT_H
