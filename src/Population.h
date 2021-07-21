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
            size_t size() {return size_; };

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

            template <typename RENG>
            void evolutionaryStep(const Parameter& param, RENG& reng) {

                std::vector<Individual> offspring;

                for (auto& ind : individuals_) {
                    ind.reproduce(param, reng);

                    for (int offs = 0; offs < ind.nOffspring_; ++offs) {
                        Individual newIndiv = ind;
                        newIndiv.cleanSlate();
                        newIndiv.mutate(param, reng);
                        offspring.push_back(newIndiv);
                    }
                }

                individuals_ = offspring;
            }

        private:
            size_t size_;
            std::vector<Individual> individuals_;

            template <typename RENG>
            std::pair< grd::Grid<double>, grd::Grid<size_t> > 
                individualsExplore(const Parameter& param, RENG& reng) {

                    // set abundance & vigilance to zero before each exploration
                    auto vigil = grd::Grid<double>{param.edgeSize * param.edgeSize, 0.0};
                    auto abund = grd::Grid<size_t>{param.edgeSize * param.edgeSize, 0};

                    for (auto& ind : individuals_) {
                        if (ind.isAlive_) {
                            ind.explore(param, reng);
                            vigil.write(ind.coordinates_.x, 
                                        ind.coordinates_.y, 
                                        vigil.read(ind.coordinates_.x, ind.coordinates_.y) + ind.vigilance_);
                            abund.write(ind.coordinates_.x, 
                                        ind.coordinates_.y, 
                                        abund.read(ind.coordinates_.x, ind.coordinates_.y) + 1);
                        }
                    }

                    return {vigil, abund};
                }
            
            template <typename RENG>
            grd::Grid<double> individualsGatherAndSurvive(   const Parameter& param, 
                                                std::pair< grd::Grid<double>, grd::Grid<size_t> > vigidance,
                                                grd::Grid<double> resources,
                                                RENG& reng) {
                // calculate share of resources
                auto shares = grd::Grid<double>{param.edgeSize * param.edgeSize, 0.0};

                for (size_t cell = 0; cell < (param.edgeSize * param.edgeSize); ++cell) {
                    // share = SUM(1-v_i)/(gamma*n), gamma: competition parameter
                    double shareCell = (vigidance.second.read(cell) - vigidance.first.read(cell)) / (param.competition * vigidance.second.read(cell));
                    shares.write(cell, shareCell);
                }

                
                for (auto& ind : individuals_) {
                    if (ind.isAlive_) {
                        // individuals gather resources
                        ind.gather( param, 
                                    resources.read(ind.coordinates_.x, ind.coordinates_.y), 
                                    shares.read(ind.coordinates_.x, ind.coordinates_.y));
                        // individuals survive or get predated upon
                        ind.survive(param, reng);
                    }
                }

                return shares;
            }
    };

    Population::Population(const Parameter& param) :
        size_(param.popSize),
        individuals_(param.popSize, Individual{param})
    {
    }
}

#endif