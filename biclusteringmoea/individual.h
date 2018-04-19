#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "global.h"
#include <iostream>
#include <random>
#include <limits>
#include <chrono>
#include <math.h>
#include <vector>

using namespace std;

class Individual {
public:
  Individual();
  virtual ~Individual();
  std::vector<int> Genes;
  std::vector<int> Conditions;
  std::vector<std::vector<double>> bicl;
  int r, c;
  double f1_MSR;
  int f2_size;
  double delta;
  bool dominated;
  int intentos;

  void rnd_init();
  void generateRandom();
  void shuffle(std::vector<int> &vec, const int limit);
  void evaluate();
  double MSR();
  int size();
  bool isFeasible();
  Individual *makeCopy();
  void update();

  // dominance
  bool operator<(const Individual &y);


  void operator=(const Individual &ind2);

  bool operator==(const Individual &ind2);
};

#endif // INDIVIDUAL_H
