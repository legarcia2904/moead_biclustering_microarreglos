#ifndef SCALARFUNC_H
#define SCALARFUNC_H

#include "global.h"
#include "individual.h"
#include <vector>

// scalarizing functions for decomposition methods
double scalar_func(std::vector<double> &y_obj, std::vector<double> &namda) {

  double fvalue = 0;

  // Tchebycheff approach

  double max_fun = -1.0e+30;

  for (int n = 0; n < 2; n++) {
    double diff = fabs(y_obj[n] - idealpoint[n]);
    double feval;
    if (namda[n] == 0)
      feval = 0.00001 * diff;
    else
      feval = diff * namda[n];
    if (feval > max_fun)
      max_fun = feval;
  }

  fvalue = max_fun;

  return fvalue;
}

#endif // SCALARFUNC_H
