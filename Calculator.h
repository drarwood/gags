#ifndef __CALCULATOR__
#define __CALCULATOR__

#include <vector>
#include <string>

double CalcVariance(std::vector<double>&);
double CalcMean(std::vector<double>&);
bool   doubleEqual(double, double, double);
bool   doubleLess(double, double, double, bool);
bool   doubleGreater(double, double, double, bool);

#endif
