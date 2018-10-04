// GAGS  
// Developed by Andrew R Wood
// a.r.wood@exeter.ac.uk

#include "Calculator.h"
#include <math.h>
#include <vector>
#include <cmath>

using namespace std;


double CalcVariance(vector<double>& v) {
    double s;
    double m;
    int    i;
    s=0;
    for (i=0;i<v.size();++i) {
        s+=v[i];
    }
    m=s/(double) v.size();
    s=0;
    for (i=0;i<v.size();++i) {
        s+=(v[i]-m)*(v[i]-m);
    }
    return (s/(v.size()-1)) ;
}


double CalcMean(vector<double>& v) {
    double s;
    s=0;
    for (int i=0;i<v.size();++i) {
        s+=v[i];
    }
    return s/v.size();
}


double CalcSum(vector<double>& v) {
    double s;
    s=0;
    for (int i=0;i<v.size();++i) {
        s+=v[i];
    }
    return s;
}


bool doubleEqual(double l, double r, double e) {
    return (fabs(l-r)<e);
}


bool doubleLess(double l, double r, double e, bool orequal) {
    if (fabs(l-r)<e) {
        // within epsilon so considered equal
        return orequal;
    }
    return (l<r);
}


bool doubleGreater(double l, double r, double e, bool orequal) {
    if (fabs(l-r)<e) {
       	// within epsilon so considered equal
       	return orequal;
    }
    return (l>r);
}
