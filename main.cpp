#include <iostream>
#include <vector>
#include <random>
#include "src/vigi.h"
#include "src/Individual.h"
#include "src/Grid.h"


int main() 
{
  auto param = vigi::Parameter{};
  auto resourcesGrid = grid::Grid<double>{param.edgeSize, param.initResources};
  std::cout << "First cell: " << resourcesGrid.read(0,0) << "\n";
  resourcesGrid.write(0, 0, 12.3);
  std::cout << "First cell new: " << resourcesGrid.read(0,0) << "\n";
  
  std::vector<vigi::Individual> pop{ param.popSize, vigi::Individual{param} };
  auto reng = std::default_random_engine{};
  for (auto& ind : pop) {
    ind.set_random_coord(resourcesGrid.dim(), reng);
  }

  return 0;
}
