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
      Population(const Parameter &);
      size_t size() const {return individuals_.size(); };

      template <typename RENG>
      void place(const Parameter & par, RENG& reng) 
      {
        for (auto& ind : individuals_) 
        {
          ind.set_random_coord(par.edgeSize, reng);
        }
      }

      template <typename RENG>
      Grid<double>& ecologicalStep(const Parameter& param, RENG& reng, const Grid<double>& resources) 
      {
        individualsExplore(param, reng);
        individualsGatherAndSurvive(param, resources, reng);
        return shares_;
      }

      template <typename RENG>
      void evolutionaryStep(const Parameter& param, RENG& reng) 
      {

        std::vector<Individual> offspring;
        totalVigilance_ = 0.0;

        for (auto& ind : individuals_) 
        {
          ind.reproduce(param, reng);

          for (int offs = 0; offs < ind.nOffspring_; ++offs) 
          {
            Individual newIndiv = ind;
            newIndiv.cleanSlate();
            newIndiv.mutate(param, reng);
            totalVigilance_ += newIndiv.vigilance_;
            offspring.push_back(newIndiv);
          }
        }

        individuals_ = offspring;
      }

      double meanVigilance() const { return totalVigilance_ / individuals_.size(); }

    private:

      struct vigil_abund_t {
        double vigil;
        size_t abund;
      };

      std::vector<Individual> individuals_;
      Grid<vigil_abund_t> vigidance_;
      Grid<double> shares_;
      double totalVigilance_;
      

      template <typename RENG>
      void individualsExplore(const Parameter& param, RENG& reng) 
      {

        // set abundance & vigilance to zero before each exploration
        vigil_abund_t reset {0.0, 0};
        vigidance_.assign(reset);

        for (auto& ind : individuals_) {
          if (ind.isAlive_)
          {
            ind.explore(param, reng);
            vigidance_(ind.coordinates_).vigil += ind.vigilance_;
            ++vigidance_(ind.coordinates_).abund;
          }
        }
      }
            
      template <typename RENG>
      void individualsGatherAndSurvive(const Parameter& param, 
                                                    const grd::Grid<double>& resources,
                                                    RENG& reng) 
      {
        // calculate share of resources
        for (size_t cell = 0; cell < shares_.size(); ++cell) {
          // share = SUM(1-v_i)/(gamma*n), gamma: competition parameter
          const auto& vd = vigidance_[cell];
          shares_[cell] = (vd.vigil - vd.abund) / (param.competition * vd.abund);
        }
     
        for (auto& ind : individuals_) {
          if (ind.isAlive_) 
          {
            // individuals gather resources
            ind.gather( param, resources(ind.coordinates_), shares_(ind.coordinates_) );
            // individuals survive or get predated upon
            ind.survive(param, reng);
          }
        }
      }
    };

    inline Population::Population(const Parameter& param) :
      individuals_(param.popSize, Individual{param}),
      vigidance_(param.edgeSize),
      shares_(param.edgeSize)
    {
    }
}

#endif