#include "SetCalculator.h"

int SetCalculator::Mandelbrot(const std::complex<double>& point, const std::complex<double>& constant, const int maxit)
{
	// Mandelbrot set
	std::complex<double> zn = 0;
	std::complex<double> c = point;
	
	return calculateDiv(zn, c, maxit);
}

int SetCalculator::Julia(const std::complex<double>& point, const std::complex<double>& constant, const int maxit)
{
	// Julia set
	std::complex<double> zn = point;
	std::complex<double> c = constant;
	
	return calculateDiv(zn, c, maxit);
}

int SetCalculator::calculateDiv(std::complex<double> zn, std::complex<double> c, const int maxit)
{
	double div = 0;
	for (int i = 0; i < maxit; ++i)
	{
		//zn = std::pow(zn, 2.0) + c * std::pow(-1, i);
		zn = std::pow(zn, 2.0) + c;
		double norm = std::norm(zn);
		if (norm >= 4.0)
		{
			div = i;
			break;
		}
		else if (norm < 0.01)
		{
			div = 0;
			break;
		}
		 
	}
	return div;
}