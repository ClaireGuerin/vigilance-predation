#ifndef VIGI_PARAMETERS_H_INCLUDED
#define VIGI_PARAMETERS_H_INCLUDED

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
    size_t ecoTime = 4;                 // number of ecological steps per generation

    double v = 0.5;                     // initial vigilance level
    double mutRate = 0.1;              // mutation rate
    double mutStep = 0.02;              // mutation step (deviation)

    double p = 0.3;                     // basal predation risk

    double eff = 0.9;                   // efficiency in resources extraction
    double competition = 1.0;           // gamma, intra-cell competition for resources

    double residualFertility = 0.0001;     // fertility cannot be zero (poisson distri), so when ind has no resources, its fertility is residual
    double fecundity = 2.0;             // basal fecundity

    double rGrowth = 1.1;               // resources growth rate
    double maxGrowth = 200.0;           // resources max growth

    unsigned rdSeed = 3;                // random seed for the simulation

  };

}

#endif
