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
  auto resources = grd::Grid<double>{param.edgeSize, param.initResources};
  auto shares = grd::Grid<double>{param.edgeSize, 0.0};
  auto vigilance = grd::Grid<double>{param.edgeSize, 0.0};
  auto abundance = grd::Grid<int>{param.edgeSize, 0};

  std::cout << "Placing and counting individuals...\n";
  for (auto& ind : pop.individuals()) {
    ind.set_random_coord(abundance.dim(), reng);
    vigi::Coord c = ind.coordinates();
    abundance.write(c.x, c.y, abundance.read(c.x, c.y) + 1);
  }

  std::cout << "Individuals mutate...\n";
  for (auto& ind : pop.individuals()) {
    vigi::Coord c = ind.coordinates();
    ind.mutate(param, reng);
    vigilance.write(c.x, c.y, vigilance.read(c.x, c.y) + ind.vigilance());
  }

  std::cout << "Individuals avoid predation or not...\n";
  for (auto& ind : pop.individuals()) {
    ind.survive(param, reng);
  }

  std::cout << "Calculating shares of resources on grid...\n";
  for (size_t cell = 0; cell < (abundance.dim() * abundance.dim()); ++cell) {
    // share = SUM(1-v_i)/(gamma*n), gamma = competition parameter
    double shareCell = (abundance.read(cell) - vigilance.read(cell)) / (param.competition * abundance.read(cell));
    shares.write(cell, shareCell);
  }

  std::cout << "Individuals gather resources...\n";
  for (auto& ind : pop.individuals()) {
    vigi::Coord c = ind.coordinates();
    ind.gather(param, resources.read(c.x, c.y), shares.read(c.x, c.y));
  }

  std::cout << "Individuals reproduce...\n";
  for (auto& ind : pop.individuals()) {
    ind.reproduce(param, reng);
  }

  return 0;
}
