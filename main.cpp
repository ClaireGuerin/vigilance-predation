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

  std::vector y {param.edgeSize * param.edgeSize};
  std::vector x {param.edgeSize * param.edgeSize};

  size_t coord = 0;
  for (int i = 0; i < y.size(); ++i) {
    y[i] = coord;
    if (i % param.edgeSize == 0) ++coord;
  }

  coord = 0;
  for (int i = 0; i < y.size(); ++i) {
    if (coord < param.edgeSize) ++coord;
    else coord = 0;

    y[i] = coord;
  }
  
  auto pop = vigi::Population{param};
  std::cout << "Pop size: " << pop.size() << "\n";
  auto resources = grd::Grid<double>{param.edgeSize, param.initResources};

  std::cout << "Placing individuals on grid...\n";
  pop.place(param, reng);

  std::ofstream ofsR("resources_out.txt"); 
  if(!ofsR.is_open()) 
  {
    std::cerr << "error: unastd::vector y {param.edgeSize * param.edgeSize};ble to open resources file\n";
    exit(EXIT_FAILURE);
  }

  std::ofstream ofsV("vigilance_out.txt"); 
  if(!ofsV.is_open()) 
  {
    std::cerr << "error: unable to open vigilance file\n";
    exit(EXIT_FAILURE);
  }

  std::ofstream ofsE("exploration_out.txt");
  if(!ofsE.is_open()) 
  {
    std::cerr << "error: unable to open vigilance file\n";
    exit(EXIT_FAILURE);
  }

  // SIM LOOP
  size_t timer = 0;

  for (size_t gen = 0; gen < param.nGen; ++gen) {

    if (pop.size() == 0) {
      std::cout << "Population extinct. Exiting...\n";
      break;
    } else {

      std::cout << "Gen " << gen << "\n";

      // ECOLOGICAL TIME STEPS
      for (size_t step = 0; step < param.ecoTime; ++step) {

        std::cout << "eco time step " << step << "\n";
        auto shares = pop.ecologicalStep(param, reng, resources, ofsE, timer);

        // DEPLETE AND GROW RESOURCES
        // resourceConsumption = 1 - self.efficiency * shares
        // resourceGrowth = self.grid.resources * self.growth
        // limit growth of resources arbitrarily set to 200 / fecundity
        for (size_t cell = 0; cell < resources.size(); ++cell) {
          if (resources[cell] > (200 / param.fecundity) ) 
          {
            resources[cell] = param.initResources;
          } else 
          {
            resources[cell] *= param.rGrowth * (1.0 - param.eff * shares[cell]);
            //if (cell % 5 == 0) std::cout << resources[cell] << "\n";
          }

          // write out resources
          ofsR << timer << " " << x[cell] << " " << y[cell] << " " << resources[cell] << "\n";
        }

        ++timer;
      }

      // EVOLUTIONARY TIME STEP

      std::cout << "Reproduction...\n";
      pop.evolutionaryStep(param, reng);
      ofsV << gen << " " << pop.meanVigilance() << "\n";
    }

  }
  
  return 0;
}
