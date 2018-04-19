#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include "individual.h"
#include <vector>

class Subproblem {
public:
  Subproblem();
  virtual ~Subproblem();

  Individual indiv;          // best solution
  std::vector<double> namda; // weight vector
  std::vector<int>
      table; // the vector for the indexes of neighboring subproblems
  std::vector<int> array; // lattice point in a simplex

  void operator=(const Subproblem &sub2);
};

#endif // SUBPROBLEM_H
