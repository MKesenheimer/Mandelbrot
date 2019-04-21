#pragma once
#include <cmath>
#include <complex>

class SetCalculator
{
public:
	static int Mandelbrot(const std::complex<double>& point, const std::complex<double>& constant = std::complex<double>(0.1, 0.4), const int maxit = 1000);
	
	static int Julia(const std::complex<double>& point, const std::complex<double>& constant = std::complex<double>(0.1, 0.4), const int maxit = 1000);
	
private:
	static int calculateDiv(std::complex<double> zn, std::complex<double> c, const int maxit);
};