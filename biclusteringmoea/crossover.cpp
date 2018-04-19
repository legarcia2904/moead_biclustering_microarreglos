#include "crossover.h"
#include <algorithm>

Crossover::Crossover(double pc) {
    this->pc = pc;
 }

void Crossover::compute(Individual &p1, Individual &p2, Individual &child1,
                        Individual &child2) {
   double rnd;
   std::uniform_real_distribution<double> distribution(0,1);
   rnd = distribution(generator);

  int pivG, pivC;
  int PG, PC;

  if (rnd <= pc) {
      std::uniform_int_distribution<int> distributionPri(0,p1.r - 1);
      pivG = distributionPri(generator);

      std::uniform_int_distribution<int> distributionSeg(0,p1.c - 1);
      pivC = distributionSeg(generator);

    PG = p1.Genes[pivG];
    PC = p1.Conditions[pivC];

    for (int i = 0; i < p1.Genes.size(); ++i) {
      if (p1.Genes[i] <= PG) {
        child1.Genes.push_back(p1.Genes[i]);
      }

      else {
        child2.Genes.push_back(p1.Genes[i]);
      }
    }

    for (int j = 0; j < p2.Genes.size(); ++j) {
      if (p2.Genes[j] > PG) {
        std::vector<int>::iterator it;
        it = std::find(child1.Genes.begin(), child1.Genes.end(), p2.Genes[j]);

        if (it != child1.Genes.end()) {
          // std::cout << p2.Genes[j] << " already in child1\n";
        } else {
          child1.Genes.push_back(p2.Genes[j]);
        }
      }

      else {
        std::vector<int>::iterator it;
        it = std::find(child2.Genes.begin(), child2.Genes.end(), p2.Genes[j]);

        if (it != child2.Genes.end()) {
          // std::cout << p2.Genes[j] << " already in child2\n";
        } else {
          child2.Genes.push_back(p2.Genes[j]);
        }
      }
    }

    for (int i = 0; i < p1.Conditions.size(); ++i) {
      if (p1.Conditions[i] <= PC) {
        child1.Conditions.push_back(p1.Conditions[i]);
      }

      else {
        child2.Conditions.push_back(p1.Conditions[i]);
      }
    }

    for (int i = 0; i < p2.Conditions.size(); ++i) {
      if (p2.Conditions[i] > PC) {
        std::vector<int>::iterator it;
        it = std::find(child1.Conditions.begin(), child1.Conditions.end(),
                       p2.Conditions[i]);

        if (it != child1.Conditions.end()) {
          // std::cout << p2.Conditions[i] << " already in child1\n";
        } else {
          child1.Conditions.push_back(p2.Conditions[i]);
        }
      }

      else {
        std::vector<int>::iterator it;
        it = std::find(child2.Conditions.begin(), child2.Conditions.end(),
                       p2.Conditions[i]);

        if (it != child2.Conditions.end()) {
          // std::cout << p2.Conditions[i] << " already in child2\n";
        } else {
          child2.Conditions.push_back(p2.Conditions[i]);
        }
      }
    }
  }

  // evita que se queden vacios los vectores del hijo 1
  if (child1.Genes.empty()) {
    child1.Genes.push_back(PG);
  }

  if (child1.Conditions.empty()) {
    child1.Conditions.push_back(PC);
  }

  // evita que se queden vacios los vectores del hijo 2
  if (child2.Genes.empty()) {
    child2.Genes.push_back(PG);
  }

  if (child2.Conditions.empty()) {
    child2.Conditions.push_back(PC);
  }
}
