#ifndef VIGI_PARAMETERS_H_INCLUDED
#define VIGI_PARAMETERS_H_INCLUDED

#include <random>
#include <algorithm>
#include <cassert>
#include <fstream>
#include "Grid.h"

namespace vigi {

  // selective using directives
  using grd::Coord;
  using grd::Grid;

  struct Parameter
  {
    size_t edgeSize;          // grid size
    double initResources;     // initial resources

    size_t popSize;           // population size
    size_t nGen;              // number of generations
    size_t ecoTime;           // number of ecological steps per generation

    double v;                 // initial vigilance level
    double mutRate;           // mutation rate
    double mutStep;           // mutation step (deviation)

    double p;                 // basal predation risk

    double eff;               // efficiency in resources extraction
    double competition;       // gamma, intra-cell competition for resources

    double residualFertility; // fertility cannot be zero (poisson distri), so when ind has no resources, its fertility is residual
    double fecundity;         // basal fecundity

    double rGrowth;           // resources growth rate
    double maxGrowth;         // resources max growth

    unsigned rdSeed;          // random seed for the simulation

    explicit Parameter(std::string);

  };

  Parameter::Parameter(std::string pathToParFile) 
  {
    try
    {
      std::ifstream ifs(pathToParFile + "parameters.txt");
      if (!ifs.is_open())
      {
        throw std::runtime_error("unable to open parameters file");
      }

      std::string input;

      while (ifs >> input) {
        if (input == "edgeSize") ifs >> edgeSize;
        else if (input == "initResources") ifs >> initResources;
        else if (input == "popSize") ifs >> popSize;
        else if (input == "nGen") ifs >> nGen;
        else if (input == "ecoTime") ifs >> ecoTime;
        else if (input == "v") 
        {
          ifs >> v;
          assert(v >= 0.0 && v <= 1.0);
        }
        else if (input == "mutRate") ifs >> mutRate;
        else if (input == "mutStep") ifs >> mutStep;
        else if (input == "p") ifs >> p;
        else if (input == "eff") ifs >> eff;
        else if (input == "competition") ifs >> competition;
        else if (input == "residualFertility") ifs >> residualFertility;
        else if (input == "fecundity") ifs >> fecundity;
        else if (input == "rGrowth") ifs >> rGrowth;
        else if (input == "maxGrowth") ifs >> maxGrowth;
        else if (input == "rdSeed") ifs >> rdSeed;
        else throw std::runtime_error(input + " is not a valid parameter.\n");
      }

    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
    
  }

}

#endif
