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
            size_t size() {return individuals_.size(); };

            template <typename RENG>
            void place(const Parameter & par, RENG& reng) {
                for (auto& ind : individuals_) {
                    ind.set_random_coord(par.edgeSize, reng);
                }
            }

            std::vector<vigi::Individual> individuals() const { return individuals_; }

            template <typename RENG>
            grd::Grid<double> ecologicalStep(const Parameter& param, RENG& reng, grd::Grid<double> resources) {

                return individualsGatherAndSurvive( param, 
                                                    individualsExplore(param, reng),
                                                    resources,
                                                    reng);

            }

        private:
            // size_t size_;    // info already available in individuals_
            std::vector<vigi::Individual> individuals_;

            struct vigil_abund_t
            {
              double vigil;
              size_t abund;
            };


            template <typename RENG>
            Grid<vigil_abund_t> individualsExplore(const Parameter& param, RENG& reng) 
            {
              // set abundance & vigilance to zero before each exploration
              //auto vigil = grd::Grid<double>{param.edgeSize * param.edgeSize, 0.0};
              //auto abund = grd::Grid<size_t>{param.edgeSize * param.edgeSize, 0};
              auto va = Grid<vigil_abund_t>(param.edgeSize, { 0.0,0 });

              for (auto& ind : individuals_) {
                if (ind.isAlive()) {
                  ind.explore(param, reng);
                  Coord c = ind.coordinates();
              //      vigil.write(c.x, c.y, vigil.read(c.x, c.y) + ind.vigilance());
              //      abund.write(c.x, c.y, abund.read(c.x, c.y) + 1);
              //      vigil(c) += ind.vigilance();
              //      abund(c) += 1;
                  va(c).vigil += ind.vigilance();
                  ++va(c).abund;
                }
              }
              return va;    // subject to RVO, no copy included
            }
            
            template <typename RENG>
            Grid<double> individualsGatherAndSurvive(const Parameter& param, 
                                                     Grid<vigil_abund_t> vigidance,
                                                     grd::Grid<double> resources,
                                                     RENG& reng) 
            {
              // calculate share of resources
              //auto shares = Grid<double>{ param.edgeSize * param.edgeSize, 0.0 };
              auto shares = Grid<double>{ param.edgeSize, 0.0 };
            
              //for (size_t cell = 0; cell < (param.edgeSize * param.edgeSize); ++cell) {
              //  // share = SUM(1-v_i)/(gamma*n), gamma: competition parameter
              //  double shareCell = (vigidance.second.read(cell) - vigidance.first.read(cell)) / (param.competition * vigidance.second.read(cell));
              //  shares.write(cell, shareCell);
              //}

              for (size_t cell = 0; cell < shares.size(); ++cell) {
                // share = SUM(1-v_i)/(gamma*n), gamma: competition parameter
                const auto& vd = vigidance[cell];
                shares[cell] = (vd.vigil - vd.abund) / (param.competition * vd.abund);
              }

                
                for (auto& ind : individuals_) {
                    if (ind.isAlive()) {
                        Coord c = ind.coordinates();

                        // individuals gather resources
//                        ind.gather(param, resources.read(c.x, c.y), shares.read(c.x, c.y));
                        ind.gather(param, resources(c), shares(c));
                        // individuals survive or get predated upon
                        ind.survive(param, reng);
                    }
                }

                return shares;
            }
    };

    
    inline Population::Population(const Parameter& param) :
        individuals_(param.popSize, vigi::Individual{param})
    {
    }
}

#endif