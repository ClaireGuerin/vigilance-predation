#ifndef VIGI_POPULATION_H_INCLUDED
#define VIGI_POPULATION_H_INCLUDED

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

        private:
            size_t size_;
            std::vector<vigi::Individual> individuals_;
            //grd::Grid<double> resources_;
            //grd::Grid<int> abundance_;
    };

    Population::Population(const Parameter& param) :
        size_(param.popSize),
        individuals_(param.popSize, vigi::Individual{param})
    {
    }
}

#endif