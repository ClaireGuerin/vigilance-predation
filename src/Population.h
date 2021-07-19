#ifndef VIGI_POPULATION_H_INCLUDED
#define VIGI_POPULATION_H_INCLUDED

#include <vector>
#include "Individual.h"
#include "Utils.h"

namespace vigi {

    class Population 
    {
        public:
            Population(const Parameter&);
            size_t size() {return size_; }
        private:
            size_t size_;
            std::vector<Individual> individuals_;
    };

    Population::Population(const Parameter& param) :
        size_(param.popSize)
    {
    }
}

#endif