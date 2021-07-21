#include <iostream>
#include <vector>
#include <random>
#include <fstream>
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
  auto resources = grd::Grid<double>{param.edgeSize * param.edgeSize, param.initResources};

  std::cout << "Placing individuals on grid...\n";
  for (auto& ind : pop.individuals()) {
    ind.set_random_coord(param.edgeSize * param.edgeSize, reng);
  }

/*  std::ofstream ofs("vigilance.txt"); 
  if(!ofs.is_open()) {
    std::cerr << "error: unable to open vigilance file\n";
    exit(EXIT_FAILURE);
  }*/

  for (size_t gen = 0; gen < param.nGen; ++gen) {

    if (pop.size() == 0) {
      std::cout << "Population extinct. Exiting...\n";
      break;
    } else {

      // ECOLOGICAL TIME STEPS
      for (size_t step = 0; step < param.ecoTime; ++step) {

        std::cout << "eco time step " << step << "\n";
        auto shares = pop.ecologicalStep(param, reng, resources);

        // DEPLETE AND GROW RESOURCES
        // resourceConsumption = 1 - self.efficiency * shares
        // resourceGrowth = self.grid.resources * self.growth
        // limit growth of resources arbitrarily set to 200 / fecundity
        for (size_t cell = 0; cell < (param.edgeSize * param.edgeSize); ++cell) {
          double tmpRes = resources.read(cell) * param.rGrowth * (1 - param.eff * shares.read(cell));
          resources.write(cell, 
                          (tmpRes > (200 / param.fecundity)) ? param.initResources : tmpRes);

        }

      }

      // EVOLUTIONARY TIME STEP

      std::cout << "Reproduction...\n";
      pop.evolutionaryStep(param, reng);

      //ofs << pop.vigilance << "\n";

    }

  }
  
  return 0;
}
