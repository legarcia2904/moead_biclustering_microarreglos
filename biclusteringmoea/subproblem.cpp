#include "subproblem.h"

Subproblem::Subproblem() {}

Subproblem::~Subproblem() {}

void Subproblem::operator=(const Subproblem &sub2) {
  indiv = sub2.indiv;
  table = sub2.table;
  namda = sub2.namda;
  array = sub2.array;
}
