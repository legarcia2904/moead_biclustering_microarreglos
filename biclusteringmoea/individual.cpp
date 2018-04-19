#include "individual.h"
#include <algorithm>

Individual::Individual() {
  delta = 2500;
  dominated = false;
  intentos = 0;
}

Individual::~Individual() {}

void Individual::rnd_init() {
  do {
//	std::cout << "Generating random: " << intentos++ << std::endl;
    generateRandom();
  } while (!isFeasible());
  std::cout << "MSR = " << f1_MSR << " Size = " << f2_size << std::endl;
}

void Individual::generateRandom() {
	Conditions.clear();
    Genes.clear();
    
   std::uniform_int_distribution<int> distribution(2,ROWS);
   r = distribution(generator);
   Genes.resize(r);

   std::uniform_int_distribution<int> distributionSeg(1,COLS);
   c = distributionSeg(generator);
  Conditions.resize(c, -1);

  shuffle(Genes, ROWS);
  shuffle(Conditions, COLS);

  bicl.resize(r);
  for (int i = 0; i < r; ++i) {
    bicl[i].resize(c);
  }

  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      bicl[i][j] = data[Genes[i]][Conditions[j]];
    }
  }

  evaluate();
}

void Individual::shuffle(std::vector<int> &vec, const int limit) {
  std::uniform_int_distribution<int> distribution(0,limit - 1);
  int i = 0;
  int index;

  while (i < vec.size()) {
    index = distribution(generator);
    std::vector<int>::iterator it;
    it = std::find(vec.begin(), vec.end(), index);
    if (it != vec.end()) {
      // std::cout << index << " already in vector\n";
    } else {
      vec[i] = index;
      i++;
    }
  }
}

void Individual::evaluate() {
  f1_MSR = MSR();
  f2_size = size() * -1;
}


double Individual::MSR() {
  double G = 0.0;

  std::vector<double> e_ic(r); // promedio i-esima fila
  std::vector<double> e_gj(c); // promedio j-esima columna
  double e_gc = 0.0;           // promedio de todo el bicluster

  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      e_ic[i] += ((double)bicl[i][j] / c);
    }
  }

  for (int i = 0; i < c; ++i) {
    for (int j = 0; j < r; ++j) {
      e_gj[i] += ((double)bicl[j][i] / r);
    }
  }

  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      e_gc += ((double)bicl[i][j] / (r * c));
    }
  }

  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      G += pow((double)bicl[i][j] - e_ic[i] - e_gj[j] + e_gc, 2);
    }
  }

  G = G / (r * c);
  // std::cout << "MSR = " << G << std::endl;

  return G;
}

int Individual::size() {
  // std::cout << "Tamaño = " << r * c << std::endl;
  return r * c;
}

bool Individual::isFeasible() {
//    if (f1_MSR <= delta)
//      return true;
//    else
//      return false;
    return true;
}

Individual *Individual::makeCopy() {
  Individual *copy = new Individual();

  copy->f1_MSR = f1_MSR;
  copy->f2_size = f2_size;
  copy->r = r;
  copy->c = c;
  copy->Genes = Genes;
  copy->Conditions = Conditions;
  copy->bicl = bicl;
  copy->dominated = dominated;
}

void Individual::update() {
  r = Genes.size();
  c = Conditions.size();

  bicl.resize(r);
  for (int i = 0; i < r; ++i) {
    bicl[i].resize(c);
  }

  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      bicl[i][j] = data[Genes[i]][Conditions[j]];
    }
  }
}

bool Individual::operator<(const Individual &y) {
  //  if (f1_MSR <= y.f1_MSR && f2_size > y.f2_size)
  //    return true;

  //  if (f2_size >= y.f2_size && f1_MSR < y.f1_MSR)
  //    return true;

  if (f1_MSR <= y.f1_MSR && f2_size < y.f2_size)
    return true;

  if (f2_size <= y.f2_size && f1_MSR < y.f1_MSR)
    return true;

  return false;
}

void Individual::operator=(const Individual &ind2) {
  f1_MSR = ind2.f1_MSR;
  f2_size = ind2.f2_size;
  r = ind2.r;
  c = ind2.c;
  Genes = ind2.Genes;
  Conditions = ind2.Conditions;
  bicl = ind2.bicl;
}

bool Individual::operator==(const Individual &ind2) {
  if (f1_MSR == ind2.f1_MSR && f2_size == ind2.f2_size)
    return true;
  else
    return false;
}
