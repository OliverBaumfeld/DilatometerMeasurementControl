#ifndef AKIMA_H
#define AKIMA_H

#include <vector>

class AkimaInterp
// Cubic spline interpolation after:
// H. Akima, "A New Method of Interpolation and Smooth Curve Fitting Based on Local Procedures"
//            J.ACM, Vol. 17, No. 4, pp. 589-602, 1970.
{
public:
	bool init(std::string file);
	double get_temperature(double resitance);
private:
	std::vector<double> a0, a1, a2, a3, vR;
};

#endif  //  AKIMA_H