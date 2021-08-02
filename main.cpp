#include <stdexcept>
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "src/Utils.h"
#include "src/Individual.h"
#include "src/Grid.h"
#include "src/Population.h"
#include "src/Random.h"

int main() 
{
  try {

    std::cout << "Placing individuals on grid...\n";
    pop.place(reng, rd);

    std::ofstream ofsR("resources_out.txt");
    if (!ofsR.is_open())
    {
      throw std::runtime_error("unable to open resources file");
    }

    std::ofstream ofsV("vigilance_out.txt");
    if (!ofsV.is_open())
    {
      std::cerr << "error: unable to open vigilance file\n";
      exit(EXIT_FAILURE);
    }

    std::ofstream ofsE("exploration_out.txt");
    if (!ofsE.is_open())
    {
      std::cerr << "error: unable to open vigilance file\n";
      exit(EXIT_FAILURE);
    }

    // SIM LOOP
    size_t timer = 0;

    for (size_t gen = 0; gen < param.nGen && pop.size(); ++gen) {
      std::cout << "Gen " << gen << "\n";

      // ECOLOGICAL TIME STEPS
      for (size_t step = 0; step < param.ecoTime; ++step, ++timer) {

        std::cout << "eco time step " << step << "\n";
        pop.ecologicalStep(param, reng, rd, resources, ofsE, timer);

        // DEPLETE AND GROW RESOURCES
        // resourceConsumption = 1 - self.efficiency * shares
        // resourceGrowth = self.grid.resources * self.growth
        // limit growth of resources arbitrarily set to 200 / fecundity
        for (size_t cell = 0; cell < resources.size(); ++cell) {
          if (resources[cell] > (param.maxGrowth / param.fecundity))
          {
            resources[cell] = param.initResources;
          }
          else
          {
            resources[cell] *= param.rGrowth * (1.0 - param.eff * pop.share(cell));
            //if (cell % 5 == 0) std::cout << resources[cell] << "\n";
          }

          // write out resources
          ofsR << timer << "," << x[cell] << "," << y[cell] << "," << resources[cell] << "\n";
        }

        // EVOLUTIONARY TIME STEP

        std::cout << "Reproduction...\n";
        pop.evolutionaryStep(param, reng, rd);
        ofsV << gen << "," << pop.meanVigilance() << "\n";
      }
    }
    if (pop.size() == 0) {
      std::cout << "Extinction, how sad\n";
    }
    return 0;
  }
  catch (const std::exception& err) {
    std::cerr << "Exception: " << err.what() << '\n';
  }
  catch (...) {
    std::cerr << "Unknown exception\n";
  }
  return -1;
}
