#include "Transformer.h"

void Transformer::Rotate(const double deltaX, const double deltaY, const double alpha, double& x, double& y)
{
	double tempX = std::cos(alpha) * x - std::sin(alpha) * y + deltaX;
	y            = std::sin(alpha) * x + std::cos(alpha) * y + deltaY;
	x = tempX;
}

void Transformer::Linear(const double x0, const double y0, const double factorX, const double factorY, double& x, double& y)
{
	x = (x - x0) * factorX;
	y = (y - y0) * factorY;
}