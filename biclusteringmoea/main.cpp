#include "global.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include "moead.h"

using namespace std;

bool readData();

int main() {
  if (!readData()) {
    exit(EXIT_FAILURE);
  }
  long long se =
                 std::chrono::duration_cast<std::chrono::nanoseconds>
                 (std::chrono::system_clock::now().time_since_epoch()).count();
  generator.seed(se);

  for (int i = 0; i < RUNS; ++i) {
    MOEAD moead;
    moead.run(POPULATION_SIZE - 1, NEIGHBORHOOD_SIZE, GENERATIONS, i + 1);
  }

  delete [] data;
  return 0;
}

bool readData() {
  data = new double *[ROWS];
  for (int i = 0; i < ROWS; i++) {
    data[i] = new double[COLS];
  }

  ifstream file;
  file.open(FILE_TO_READ);

  if (!file.is_open()) {
    cout << "Error in creating file!!!" << endl;
    return false;
  }

  for (int i = 0; i < ROWS; ++i) {
    for (int j = 0; j < COLS; ++j) {
      file >> data[i][j];
    }
  }

//  for (int i = 0; i < ROWS; ++i) {
//    for (int j = 0; j < COLS; ++j) {
//      cout << data[i][j] << " ";
//    }
//    cout << "\n";
//  }

  file.close();
  return true;
}
