#ifndef VIGI_POPULATION_H_INCLUDED
#define VIGI_POPULATION_H_INCLUDED

#include <utility>
#include <vector>
#include <random>
#include "Individual.h"
#include "Utils.h"
#include "Grid.h"

namespace vigi {

  class Population 
  {
  public:
    Population(const Parameter& param);
    size_t size() const {return individuals_.size(); };

    template <typename RENG>
    void place(const Parameter & par, RENG& reng) 
    {
        for (auto& ind : individuals_) {
            ind.set_random_coord(par.edgeSize, reng);
        }
    }

    const std::vector<vigi::Individual>& individuals() const noexcept { return individuals_; }
    std::vector<vigi::Individual>& individuals() noexcept { return individuals_; }    // not the best idea

    template <typename RENG>
    Grid<double>& ecologicalStep(const Parameter& param, RENG& reng, const Grid<double>& resources) 
    {
      individualsExplore(param, reng);
      individualsGatherAndSurvive(param, resources, reng);
      return shares_;
    }

  private:
    struct vigil_abund_t
    {
      double vigil;
      size_t abund;
    };

    std::vector<vigi::Individual> individuals_;
    Grid<vigil_abund_t> vigidance_;
    Grid<double> shares_;

    template <typename RENG>
    void individualsExplore(const Parameter& param, RENG& reng) 
    {
      for (auto& ind : individuals_) {
        if (ind.isAlive()) {
          ind.explore(param, reng);
          Coord c = ind.coordinates();
          vigidance_(c).vigil += ind.vigilance();
          ++vigidance_(c).abund;
        }
      }
    }
            
    template <typename RENG>
    void individualsGatherAndSurvive(const Parameter& param, 
                                     const grd::Grid<double>& resources,
                                     RENG& reng) 
    {
      for (size_t cell = 0; cell < shares_.size(); ++cell) {
        // share = SUM(1-v_i)/(gamma*n), gamma: competition parameter
        const auto& vd = vigidance_[cell];
        shares_[cell] = (vd.vigil - vd.abund) / (param.competition * vd.abund);
      }
      for (auto& ind : individuals_) {
        if (ind.isAlive()) {
          Coord c = ind.coordinates();
          ind.gather(param, resources(c), shares_(c));
          // individuals survive or get predated upon
          ind.survive(param, reng);
        }
      }
    }
  };

    
  inline Population::Population(const Parameter& param) :
    individuals_(param.popSize, vigi::Individual{ param }),
    vigidance_(param.edgeSize),
    shares_(param.edgeSize)
  {
  }

}

#endif