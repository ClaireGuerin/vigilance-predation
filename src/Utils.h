#ifndef VIGI_UTILS_H_INCLUDED
#define VIGI_UTILS_H_INCLUDED

#include <random>
#include <algorithm>
#include "Grid.h"

namespace vigi {

  // selective using directives
  using grd::Coord;
  using grd::Grid;

  struct Parameter
  {
    size_t edgeSize = 10;              // grid size
    double initResources = 2.0;         // initial resources

    size_t popSize = 80;               // population size
    size_t nGen = 100;                    // number of generations
    size_t ecoTime = 5;                 // number of ecological steps per generation

    double v = 0.5;                     // initial vigilance level
    double mutRate = 0.1;              // mutation rate
    double mutStep = 0.02;              // mutation step (deviation)
    bool bounded = true;                // is the phenotype bounded between 0 and 1?

    double p = 0.3;                     // basal predation risk

    double eff = 0.7;                   // efficiency in resources extraction
    double competition = 20.0;           // gamma, intra-cell competition for resources

    double residualFertility = 0.0001;     // fertility cannot be zero (poisson distri), so when ind has no resources, its fertility is residual
    double fecundity = 1.5;             // basal fecundity

    double rGrowth = 1.1;               // resources growth rate

  };

  template <typename RENG>
  Coord randomCoord(size_t edgeSize, RENG& reng)
  {
    auto pdist = std::uniform_int_distribution<>(0, static_cast<int>(edgeSize) - 1);
    return { pdist(reng), pdist(reng) };
  }

  template <typename RENG>
  Coord randomMove(const Coord& current, RENG& reng)
  {
    auto pdist = std::uniform_int_distribution<>(-1, 1);
    return { current.x + pdist(reng), current.y + pdist(reng) };
  }

  template <typename RENG>
  bool randomBool(double pTrue, RENG& reng)
  {
    auto pdist = std::bernoulli_distribution(pTrue);
    return pdist(reng);
  }

  template <typename RENG>
  double randomDev(double mutationStep, RENG& reng)
  {
    auto pdist = std::normal_distribution<>(0, mutationStep);
    return pdist(reng);
  }

  template <typename RENG>
  size_t randomRepro(double fertility, RENG& reng)
  {
    auto pdist = std::poisson_distribution<>(fertility);
    return pdist(reng);
  }
}

#endif
