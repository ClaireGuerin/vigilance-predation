#include <iostream>
#include <vector>
#include <random>
#include "src/Utils.h"
#include "src/Individual.h"
#include "src/Grid.h"
#include "src/Population.h"

int main() 
{
  auto param = vigi::Parameter{};
  auto reng = std::default_random_engine{};
  
  auto pop = vigi::Population{param};
  std::cout << "Pop size: " << pop.size() << "\n";
  auto resources = vigi::Grid<double>{param.edgeSize, param.initResources};

  std::cout << "Placing individuals on grid...\n";
  for (auto& ind : pop.individuals()) {
    ind.set_random_coord(param.edgeSize, reng);
  }

  // ECOLOGICAL TIME STEPS
  for (size_t step = 0; step < param.ecoTime; ++step) {
    std::cout << "eco time step " << step << "\n";
    auto shares = pop.ecologicalStep(param, reng, resources);
    // deplete and grow resources accordingly
    for (size_t cell = 0; cell < resources.size(); ++cell) {
      resources[cell] *= param.rGrowth * (1.0 - param.eff * shares[cell]);
    }
  }

  // EVOLUTIONARY TIME STEP

  return 0;
}
