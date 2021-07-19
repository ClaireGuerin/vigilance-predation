#include <iostream>
#include <vector>
#include <random>
#include "src/vigi.h"
#include "src/Individual.h"
#include "src/Grid.h"


int main() 
{
  auto param = vigi::Parameter{};
  auto grid = vigi::Grid{param};
  
  std::vector<vigi::Individual> pop{ param.popSize, vigi::Individual{param} };
  auto reng = std::default_random_engine{};
  for (auto& ind : pop) {
    ind.set_random_coord(grid.dim(), reng);
  }

  return 0;
}
