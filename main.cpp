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

  std::vector y(param.edgeSize * param.edgeSize, 0);
  std::vector x(param.edgeSize * param.edgeSize, 0);

  size_t coordY = 0;
  size_t coordX = 0;
  for (int i = 0; i < y.size(); ++i) {
    x[i] = coordX;
    ++coordX;
    if (coordX == param.edgeSize) coordX = 0;

    if (i > 0 && i % param.edgeSize == 0) ++coordY;
    y[i] = coordY;
  }
  
  auto pop = vigi::Population{param};
  std::cout << "Pop size: " << pop.size() << "\n";
  auto resources = grd::Grid<double>{param.edgeSize, param.initResources};
  
  /* auto testGrid = grd::Grid<double>(2);
  testGrid.assign(0.0);
  auto testGrid2 = grd::Grid<double>(2);
  testGrid2.assign(2.0);
  ++testGrid(grd::Coord {0, 0});
  testGrid(grd::Coord {1, 0}) += 3.5;
  auto testGridCalc = grd::Grid<double>(2);
  testGridCalc[0] = (testGrid[0] - testGrid2[0]) / (param.competition * testGrid[0]);
  testGridCalc[1] = (testGrid[1] - testGrid2[1]) / (param.competition * testGrid[1]);

  std::cout << "Test " << testGrid(grd::Coord {0, 0}) << " " << testGrid(grd::Coord {1, 0}) << " " << (testGridCalc[0] == -1) << " " << (testGridCalc[1] == 1.5/3.5) << "\n"; */ 


  std::cout << "Placing individuals on grid...\n";
  pop.place(param, reng);

  std::ofstream ofsR("resources_out.txt"); 
  if(!ofsR.is_open()) 
  {
    std::cerr << "error: unable to open resources file\n";
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
        pop.ecologicalStep(param, reng, resources, ofsE, timer);

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
            resources[cell] *= param.rGrowth * (1.0 - param.eff * pop.share(cell));
            //if (cell % 5 == 0) std::cout << resources[cell] << "\n";
          }

          // write out resources
          ofsR << timer << "," << x[cell] << "," << y[cell] << "," << resources[cell] << "\n";
        }

        ++timer;
      }

      // EVOLUTIONARY TIME STEP

      std::cout << "Reproduction...\n";
      pop.evolutionaryStep(param, reng);
      ofsV << gen << "," << pop.meanVigilance() << "\n";
    }

  }
  
  return 0;
}
