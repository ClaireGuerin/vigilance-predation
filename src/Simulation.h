#ifndef VIGI_SIMULATION_H
#define VIGI_SIMULATION_H

#include <string>
#include <iostream>
#include <random>
#include <cassert>
#include "Utils.h"
#include "Random.h"
#include "Population.h"

namespace vigi 
{
    class Simulation
    {
        public:
            explicit Simulation(std::string);

            void setup();

            void run(std::ofstream&, std::ofstream&, std::ofstream&);

            void save();

        private:
            std::string dir_;
            Parameter param_;
            std::default_random_engine reng_;
            Random rd_;
            Population population_;
            Grid<double> resources_;

    };

    Simulation::Simulation(std::string path) :
        dir_(path),
        param_(),
        reng_(),
        population_(param_),
        resources_(param_.edgeSize, param_.initResources),
        rd_(param_)
    {
        assert(param_.v >= 0.0 && param_.v <= 1.0);
    }

    void Simulation::setup() 
    {
        std::cout << "Placing " << population_.size() << " individuals on grid...\n";
        population_.place(reng_, rd_);
    }

    void Simulation::run(std::ofstream& ofsR, std::ofstream& ofsE, std::ofstream& ofsV)
    {
        // SIM LOOP
        size_t timer = 0;

        for (size_t gen = 0; gen < param_.nGen && population_.size(); ++gen) 
        {
            std::cout << "Gen " << gen << "\n";

            // ECOLOGICAL TIME STEPS
            for (size_t step = 0; step < param_.ecoTime; ++step, ++timer) 
            {

                std::cout << "eco time step " << step << "\n";
                population_.ecologicalStep(param_, reng_, rd_, resources_, ofsE, timer);

                // DEPLETE AND GROW RESOURCES
                // resourceConsumption = 1 - self.efficiency * shares
                // resourceGrowth = self.grid.resources * self.growth
                // limit growth of resources arbitrarily set to 200 / fecundity
                for (size_t cell = 0; cell < resources_.size(); ++cell) 
                {
                    if (resources_[cell] > (param_.maxGrowth / param_.fecundity))
                    {
                        resources_[cell] = param_.initResources;
                    } 
                    else
                    {
                        resources_[cell] *= param_.rGrowth * (1.0 - param_.eff * population_.share(cell));
                        //if (cell % 5 == 0) std::cout << resources[cell] << "\n";
                    }

                    // write out resources
                    ofsR    << timer << "," 
                            << resources_.lin_to_coord(cell).x << "," 
                            << resources_.lin_to_coord(cell).y << "," 
                            << resources_[cell] << "\n";
                }

                // EVOLUTIONARY TIME STEP

                std::cout << "Reproduction...\n";
                population_.evolutionaryStep(param_, reng_, rd_);
                ofsV << gen << "," << population_.meanVigilance() << "\n";
            }
        }

        if (population_.size() == 0) std::cout << "Extinction, how sad\n";
    }
}

#endif