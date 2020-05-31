#include <iostream>
#include <thread>
#include <math.h>

using namespace std;

double eq1(double x) {
	return x * x * x - 3 * x * x - 3 * x - 4;
};

double eq1_deriv(double x) {
	return 3 * x * x - 6 * x - 3;
}

double eq2(double x) {
	return sin(x);
}
double eq2_deriv(double x) {
	return cos(x);
}

// Newton-Raphson Method for solving f(x) = 0 equations:
// - start with initial guess x1
// - draw the tangent to the curve of f at x1, and take our next estimate x2
//   to be the point where this tangent cuts the x-axis
// In other words, x{n+1} = x{n} - f(x{n}) / f'(x{n})
void newton(double (*fn)(double), double (*fn_deriv)(double), double x1, double dx = 1e-6)
{
	double x = x1 - fn(x1) / fn_deriv(x1);
	cout << "x: " << x << endl;
	if (abs(x1 - x) <= dx) {
		return;
	}
	this_thread::sleep_for(chrono::milliseconds(500));
	newton(fn, fn_deriv, x);
}

int main()
{
	//newton(eq1, eq1_deriv, 2.5);
	//newton(eq1, eq1_deriv, 0);

	newton(eq2, eq2_deriv, 0.5);

	return 0;
}
