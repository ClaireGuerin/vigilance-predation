#ifndef VIGI_POPULATION_H_INCLUDED
#define VIGI_POPULATION_H_INCLUDED

#include <utility>
#include <vector>
#include <random>
#include <fstream>
#include <algorithm>
#include "Individual.h"
#include "Utils.h"
#include "Grid.h"

namespace vigi {

  class Population 
  {
    public:
      
      explicit Population(const Parameter &);
      
      size_t size() const {return individuals_.size(); };

      template <typename RENG>
      void place(RENG& reng, Random& rd) 
      /* Returns nothing
         Loops over individual instances (class Individual) in the population and assign them random coordinates on the grid */
      {
        for (auto& ind : individuals_) 
        {
          ind.set_random_coord(reng, rd);
        }
      }

      template <typename RENG>
      std::string ecologicalStep(const Parameter& param, RENG& reng, Random& rd, const Grid<double>& resources, const size_t& ecoTime)
      {
        /* Returns string of exploration output
           Make individuals explore the grid, gather resources and survive
           writes out exploration data to the corresponding ofstream */
        individualsExplore(param, reng, rd, ecoTime);
        individualsGatherAndSurvive(param, resources, reng, rd);

        return explore_string_;
      }

      template <typename RENG>
      void evolutionaryStep(const Parameter& param, RENG& reng, Random& rd) 
      {
        /* Returns nothing
           Makes ALIVE individual instances (class Individual) in the population reproduce
           Creates new Individual instance for each offspring of each individual instance in the population,
           by copying the parent instance.
           Resets new Individual instance storage (among other things)
           Calculates total sum of vigilance level over the whole new population of offspring
           Replaces parent individuals by offspring (non-overlapping generations)
        */

        totalVigilance_ = 0.0;
        offspring.clear();
        for (auto& ind : individuals_)
        {
          if (ind.isAlive_) 
          {
            ind.reproduce(param, reng, rd);
            //std::cout << "Offspring n = " << ind.offspring() << "\n";

            for (int offs = 0; offs < ind.nOffspring_; ++offs) 
            {
              Individual newIndiv = ind.deliver();
              newIndiv.mutate(param, reng, rd);
              totalVigilance_ += newIndiv.vigilance_;
              offspring.push_back(newIndiv);
            }
          }
        }

        std::cout << "Offspring vector: " << offspring.size() << "\n";
        individuals_.swap(offspring);
      }

      double meanVigilance() const { return totalVigilance_ / individuals_.size(); }

      double share(const size_t& linCoord) const { return shares_[linCoord]; }

    private:

      std::vector<Individual> individuals_;
      std::vector<Individual> offspring;
      Grid<double> vigilances_;
      Grid<size_t> abundances_;
      Grid<double> shares_;
      double totalVigilance_;
      std::string explore_string_ = "";
      
      template <typename RENG>
      void individualsExplore(const Parameter& param, 
                              RENG& reng, 
                              Random& rd, 
                              const size_t& ecoTime)
      {
        /* Returns nothing
           Makes the ALIVE individual instances (class Individual) move on the grid
           Updates the sum of vigilance level within each grid cell
           Updates the number of individuals within each grid cell */

        // set abundance & vigilance to zero before each exploration
        vigilances_.assign(0.0);
        abundances_.assign(0);

        for (auto& ind : individuals_) {
          if (ind.isAlive_)
          {
            ind.explore(param, reng, rd); // new individual coordinates
            explore_string_ = std::to_string(ecoTime) + "," +
                              std::to_string(ind.coordinates_.x) + "," +
                              std::to_string(ind.coordinates_.y) + "," +
                              std::to_string(ind.vigilance_);
            vigilances_(ind.coordinates_) += ind.vigilance_; // Is this a copy???
            ++abundances_(ind.coordinates_); // Is this a copy???
          }
        }
      }
            
      template <typename RENG>
      void individualsGatherAndSurvive( const Parameter& param, 
                                        const grd::Grid<double>& resources,
                                        RENG& reng,
                                        Random& rd) 
      {
        /* Returns nothing
           Calculates the per-capita share of resources in each grid cell based on updated vigilance and abundance
           Makes ALIVE individual instances (class Individual) in the population accumulate resources in their storage
           Makes ALIVE individual instances (class Individual) in the population survive to predation
        */

        // calculate share of resources
        for (size_t cell = 0; cell < shares_.size(); ++cell) {
          // share = SUM(1-v_i)/(gamma*n), gamma: competition parameter
          const auto& vigi = vigilances_[cell];
          const auto& abun = abundances_[cell];
          shares_[cell] = (abun == 0) ? 0.0 : (abun - vigi) / (param.competition * abun);
        }
     
        for (auto& ind : individuals_) {
          if (ind.isAlive_) {
            // individuals gather their share of resources from the grid cell where they currently are
            ind.gather(param, resources(ind.coordinates_), shares_(ind.coordinates_));
            // individuals survive or get predated upon
            ind.survive(param, reng, rd);
          }
        }
      }
    };

    inline Population::Population(const Parameter& param) :
      individuals_(param.popSize, Individual{param}),
      vigilances_(param.edgeSize),
      abundances_(param.edgeSize),
      shares_(param.edgeSize)
    {
    }

}

#endif