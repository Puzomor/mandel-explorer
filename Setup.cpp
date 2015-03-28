#include "Setup.h"

Setup::Setup()
{
	reset();
}

Setup::Setup(double z, double x, double y, double w, double h, int miter, int s1, int s2)
{
	zoom = z;
	offsetx = x;
	offsety = y;
	width = w;
	height = h;
	maxIter = miter;
	style1 = s1;
	style2 = s2;
}

void Setup::setStyle(int s1, int s2)
{
	style1 = s1;
	style2 = s2;
}

void Setup::reset()
{
	zoom = 2;
	offsetx = 0;
	offsety = 0;
	width = 80;
	height = 79;
	maxIter = 50;
	style1 = style2 = 3;
}