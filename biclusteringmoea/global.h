#ifndef GLOBAL_H
#define GLOBAL_H
#include <string>
#include <random>

//const std::string FILE_TO_READ = "yeast.dat";
//const std::string FILE_TO_READ = "human.dat";
const std::string FILE_TO_READ = "14cancer.xtest";
extern double **data; // Genetic expression matrix
//const int ROWS = 2884; //yeast
//const int COLS = 17; //yeast
//const int ROWS = 4026; //human
//const int COLS = 96; //human
const int ROWS = 16063; //human
const int COLS = 144; //human

const int POPULATION_SIZE = 200;
const int NEIGHBORHOOD_SIZE =30;
const int LIMIT = 5; // maximal number of solutions updated
const int GENERATIONS = 50;
const int RUNS = 1;

const double MUTATION_PROB = 0.4;
const double CROSSOVER_PROB = 1.0;          // DON'T CHANGE THIS
const double RND_CHILD_1_SELECT_PROB = 0.5; // Child 2 prob is (this - 1)
const double MUTATE_GENES_PROB = 0.8;

extern double *idealpoint; // Ideal point used in decomposition method
extern  std::default_random_engine generator;

#endif // GLOBAL_H
