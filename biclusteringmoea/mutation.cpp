#include "mutation.h"
#include "global.h"
#include <algorithm>

Mutation::Mutation(double pm) {
    this->pm = pm;
}

void Mutation::compute(Individual &child) {
    double rnd;
    std::uniform_real_distribution<double> distribution(0,1);
    rnd = distribution(generator);
  if (rnd <= pm) {
      double rnd2;
      std::uniform_real_distribution<double> distributionSeg(0,1);
      rnd2 = distributionSeg(generator);
    // muta genes
    if (rnd2 <= MUTATE_GENES_PROB) {
      int gen;
      std::uniform_int_distribution<int> distributionInt1(0,ROWS - 1);
      gen = distributionInt1(generator);

      std::vector<int>::iterator it;
      it = std::find(child.Genes.begin(), child.Genes.end(), gen);

      if (it != child.Genes.end()) {
        // std::cout << gen << " already in child\n";
        child.Genes.erase(it);

        // si se queda vacio se agrega uno aleatorio
        if (child.Genes.empty()) {
          gen = distributionInt1(generator);
          child.Genes.push_back(gen);
        }
      } else {
        child.Genes.push_back(gen);
      }
    }

    // muta condiciones
    else {
      int cond;
      std::uniform_int_distribution<int> distributionInt2(0,COLS - 1);
      cond = distributionInt2(generator);

      std::vector<int>::iterator it;
      it = std::find(child.Conditions.begin(), child.Conditions.end(), cond);

      if (it != child.Conditions.end()) {
        child.Conditions.erase(it);

        // si se queda vacio agrega uno aleatorio
        if (child.Conditions.empty()) {
          cond = distributionInt2(generator);
          child.Conditions.push_back(cond);
        }
      } else {
        child.Conditions.push_back(cond);
      }
    }
  }
}
