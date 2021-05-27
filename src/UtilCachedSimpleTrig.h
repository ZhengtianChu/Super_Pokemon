#pragma once

#include "math.h"


class CachedSimpleTrig
{
	double cosCache[360];

	CachedSimpleTrig()
	{
		for (int i = 0; i < 360; i++)
			cosCache[i] = ::cos(i);
	}

	// Get cosine for integer angle to avoid the continuous function call to the sin and cos methods
	double cos(int angle) { return cosCache[(angle + 720) % 360]; }
	double sin(int angle) { return cosCache[(angle + 630) % 360]; } // cos of 90 degrees less
};
