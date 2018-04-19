#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "individual.h"

class Crossover {
public:
  Crossover(double pc);
  void compute(Individual &p1, Individual &p2, Individual &child1,
               Individual &child2);

  double pc;
};

#endif // CROSSOVER_H
