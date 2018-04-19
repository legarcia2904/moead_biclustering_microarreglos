#include "moead.h"
#include "common.h"
#include "crossover.h"
#include "mutation.h"
#include "scalarfunc.h"
#include <fstream>
#include <iomanip>

MOEAD::MOEAD() {
  idealpoint = new double[2];

  idealpoint[0] = 1.0e+30; // positive value for minimization MSR
  idealpoint[1] = 1.0e+30; // positive value for minimization size
  maxRepeadChild =0; //Para controlar que si pasa de 50 intentos por generar un hijo salte.
}

MOEAD::~MOEAD() {
  delete[] idealpoint;

  for (std::vector<Subproblem *>::iterator it = population.begin();
       it != population.end(); ++it) {
    delete (*it);
  }
  population.clear();

  for (std::vector<Individual *>::iterator it = nondominatedSet.begin();
       it != nondominatedSet.end(); it++) {
    delete (*it);
  }
  nondominatedSet.clear();
}

void MOEAD::init_uniformweight(int sd) {
  for (int i = 0; i <= sd; ++i) {
    Subproblem *sub = new Subproblem;
    sub->array.push_back(i);
    sub->array.push_back(sd - i);

    for (int j = 0; j < sub->array.size(); j++) {
      sub->namda.push_back(1.0 * sub->array[j] / sd);
    }
//    std::cout << sub->namda[0] << " " << sub->namda[1] << "\n";

    population.push_back(sub);
  }

  pops = population.size();
}

void MOEAD::init_neighbourhood() {
  double *x = new double[pops];
  int *idx = new int[pops];

  for (int i = 0; i < pops; ++i) {
    for (int j = 0; j < pops; ++j) {
      x[j] = distanceVector(population[i]->namda, population[j]->namda);
      idx[j] = j;
    }

    minfastsort(x, idx, pops, niche);

    for (int k = 0; k < niche; k++)
      population[i]->table.push_back(idx[k]);
  }

  delete[] x;
  delete[] idx;
}

void MOEAD::init_population() {
  for (int i = 0; i < pops; ++i) {
    population[i]->indiv.rnd_init();
    update_reference(population[i]->indiv);
  }
}

void MOEAD::update_reference(Individual &ind) {

  if (ind.f1_MSR < idealpoint[0]) {
    idealpoint[0] = ind.f1_MSR;
    std::cout << "******** Best MSR updated: " << idealpoint[0]
              << " ********\n";
  }

  if (ind.f2_size < idealpoint[1]) {
    idealpoint[1] = ind.f2_size;
    std::cout << "******** Best size updated: " << idealpoint[1]
              << " ********\n";
  }
}

void MOEAD::evolution() {
  maxRepeadChild = 0;
  pops = population.size();
  int *perm = new int[pops];
  random_permutation(perm, pops);

  for (int i = 0; i < population.size(); ++i) {
    maxRepeadChild = 0;
    int n = perm[i];
    int type;

    double rnd;
    std::uniform_real_distribution<double> distribution(0,1);
    rnd = distribution(generator);
    if (rnd <= 0.9)
      type = 1; // neighborhood
    else
      type = 2; // whole population

    vector<int> p;
    matingselection(p, n, 2, type); // neighborhood selection

    Individual child_win;

    do {
      Individual child1, child2;

      Crossover cros(CROSSOVER_PROB);
      cros.compute(population[p[0]]->indiv, population[p[1]]->indiv, child1,
                   child2);

      Mutation mut(MUTATION_PROB);
      mut.compute(child1);
      mut.compute(child2);

      child1.update();
      child1.evaluate();

      child2.update();
      child2.evaluate();

      if (child1 < child2) {
        child_win = child1;
      } else if (child2 < child1) {
        child_win = child2;
      } else {

      double tourn;
      std::uniform_real_distribution<double> distribution(0,1);
      tourn = distribution(generator);

        if (tourn <= RND_CHILD_1_SELECT_PROB)
          child_win = child1;
        else
          child_win = child2;
      }
      maxRepeadChild ++;
    std::cout << "Child try: "<<maxRepeadChild << " Pop: "<<i<<std::endl;
      if(maxRepeadChild > 10)
      {
          std::cout<< "No se pudo generar un hijo factible, se copio al padre"<<endl;
          child_win = population[p[0]]->indiv;
          break;
      }
    } while (!child_win.isFeasible());

    update_reference(child_win);
    update_problem(child_win, n, type);

    p.clear();

    updateNondominatedSet(child_win);
  }

  delete[] perm;
}

void MOEAD::run(int sd, int nc, int mg, int rn) {
  // sd: integer number for generating weight vectors
  // nc: size of neighborhood
  // mg: maximal number of generations
  niche = nc;
  init_uniformweight(sd);
  init_neighbourhood();
  init_population();

  for (int gen = 2; gen < mg; ++gen) {
    std::cout << "gen: " << gen << "\n";
    evolution();
  }

  save_front(rn);
  population.clear();
}

void MOEAD::update_problem(Individual &child, int id, int type) {
  // indiv: child solution
  // id:   the id of current subproblem
  // type: update solutions in - neighborhood (1) or whole population
  // (otherwise)

  int size, time = 0;

  if (type == 1)
    size = population[id]->table.size();
  else
    size = population.size();

  int *perm = new int[size];
  random_permutation(perm, size);

  for (int i = 0; i < size; ++i) {
    int k;
    if (type == 1)
      k = population[id]->table[perm[i]];
    else
      k = perm[i];

    std::vector<double> objs(2);
    objs[0] = population[k]->indiv.f1_MSR;
    objs[1] = population[k]->indiv.f2_size;

    std::vector<double> childObjs(2);
    childObjs[0] = child.f1_MSR;
    childObjs[1] = child.f2_size;

    double f1, f2;
    f1 = scalar_func(objs, population[k]->namda);
    f2 = scalar_func(childObjs, population[k]->namda);

    if (f2 < f1) {
      population[k]->indiv = child;
      time++;
    }

    if (time >= LIMIT)
      break;
  }

  delete[] perm;
}

void MOEAD::save_front(int rn) {
  std::string name = "PF/PF";

  std::fstream fout;
  name.append("_Pop_").append(std::to_string(rn)).append(".dat");
  fout.open(name, std::ios::out);

  for (int i = 0; i < population.size(); ++i) {
    fout << population[i]->indiv.f1_MSR << " "
         << population[i]->indiv.f2_size * -1 << "\n";
  }
  fout.close();

  std::fstream nonds;
  name.clear();
  name = "PF/PF";
  name.append("_NDS_").append(std::to_string(rn)).append(".dat");
  fout.open(name, std::ios::out);
  for (int i = 0; i < nondominatedSet.size(); ++i) {
    fout << nondominatedSet[i]->f1_MSR << " "
         << nondominatedSet[i]->f2_size * -1 << "\n";
  }
  fout.close();

  for (int i = 0; i < population.size(); ++i) {
    std::fstream fpop;
    std::string namefile = "Biclusters/pop_";
    namefile.append(std::to_string(i))
        .append("_")
        .append(std::to_string(rn))
        .append(".dat");
    fpop.open(namefile, std::ios::out);

    int r, c;
    r = population[i]->indiv.r;
    c = population[i]->indiv.c;

    //    for (int h = 0; h < c; ++h) {
    //      fpop << population[i]->indiv.Conditions[h] << ", ";
    //    }
    //    fpop << "\n";


    for (int k = 0; k < r; ++k) {
      // fpop << population[i]->indiv.Genes[k] << ",";
      for (int j = 0; j < c; ++j) {
        fpop << population[i]->indiv.bicl[k][j] << ",";
      }
      fpop << "\n";
    }

    fpop.close();
  }
}


void MOEAD::random_permutation(int *perm, int size) {

  for (int i = 0; i < size; ++i) {
    perm[i] = i;
  }

  for (int i = size - 1; i > 0; i--) {
    std::uniform_int_distribution<int> distribution(0,i);
    int index;
    index = distribution(generator);

    int temp = perm[index];
    perm[index] = perm[i];
    perm[i] = temp;
  }
}

void MOEAD::matingselection(std::vector<int> &list, int cid, int size,
                            int type) {
  // list : the set of the indexes of selected mating parents
  // cid  : the id of current subproblem
  // size : the number of selected mating parents
  // type : 1 - neighborhood; otherwise - whole population

  int ss = population[cid]->table.size(), r, p;

  while (list.size() < size) {
    if (type == 1) {
        std::uniform_int_distribution<int> distribution(0, ss - 1);
        r = distribution(generator);
      p = population[cid]->table[r];
    } else {
        std::uniform_int_distribution<int> distribution(0, population.size() - 1);
        p = distribution(generator);
    }

    bool flag = true;

    for (int i = 0; i < list.size(); ++i) {
      if (list[i] == p) {
        flag = false;
        break;
      }
    }
    if (flag)
      list.push_back(p);
  }
}

void MOEAD::updateNondominatedSet(Individual &ind) {

  int i;

  for (i = 0; i < nondominatedSet.size(); i++) {
    if (ind == (*nondominatedSet[i]))
      return;

    if ((*nondominatedSet[i]) < ind)
      return;

    if (ind < (*nondominatedSet[i])) {
      nondominatedSet[i]->dominated = true;
    } else {
      nondominatedSet[i]->dominated = false;
    }
  }

  for (i = 0; i < nondominatedSet.size(); i++) {
    if (nondominatedSet[i]->dominated) {
      delete nondominatedSet[i];
      nondominatedSet.erase(nondominatedSet.begin() + i);
      i--;
    }
  }

  nondominatedSet.push_back(ind.makeCopy());
}
