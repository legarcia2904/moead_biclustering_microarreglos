#ifndef MOEAD_H
#define MOEAD_H

#include "global.h"
#include "individual.h"
#include "subproblem.h"
#include <string>

class MOEAD {
public:
  MOEAD();
  ~MOEAD();

  void init_uniformweight(int sd);
  void init_neighbourhood();
  void init_population();
  void update_reference(Individual &ind);
  void evolution();
  void run(int sd, int nc, int mg, int rn);
  void update_problem(Individual &child, int id, int type);
  void save_front(int rn);
  void random_permutation(int *perm, int size);
  void matingselection(std::vector<int> &list, int cid, int size, int type);
  void updateNondominatedSet(Individual &child);

  std::vector<Subproblem *> population;
  std::vector<Individual *> nondominatedSet;
  int niche;
  int pops;
  int maxRepeadChild;
};

#endif // MOEAD_H
