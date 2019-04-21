#pragma once
#include <cmath>

class Transformer
{
public:
	static void Rotate(const double deltaX, const double deltaY, const double alpha, double& x, double& y);
	
	static void Linear(const double x0, const double y0, const double factorX, const double factorY, double& x, double& y);
};