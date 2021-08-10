#ifndef VIGI_SIMULATION_H
#define VIGI_SIMULATION_H

#include <string>
#include <iostream>
#include <random>
//#include <srand>
#include <vector>
#include <string>
#include "Parameters.h"
#include "Random.h"
#include "Population.h"

namespace vigi 
{
    class Simulation
    {
        public:
            explicit Simulation(std::string);

            void setup();

            void run();

            void save(std::ofstream&, std::ofstream&, std::ofstream&);

            bool vis() const { return param_.visual; }

        private:
            std::string dir_;
            Parameter param_;
            std::default_random_engine reng_;
            Random rd_;
            Population population_;
            Grid<double> resources_;
            std::vector<std::string> resources_out_;
            std::vector<std::string> vigilance_out_;
            std::vector<std::string> exploration_out_;

            void write(std::vector<std::string>, std::ofstream&);


    };

    Simulation::Simulation(std::string path) :
        dir_(path),
        param_(path),
        reng_(),
        population_(param_),
        resources_(param_.edgeSize, param_.initResources),
        rd_(param_)
        //resources_out_(param_.nGen * param_.ecoTime * param_.edgeSize * param_.edgeSize), // huge memory allocation!!!
        //vigilance_out_(param_.nGen),
        //exploration_out_() // size unknown yet since population changes
    {
    }

    void Simulation::setup() 
    {
        srand(param_.rdSeed);
        std::cout << "Placing " << population_.size() << " individuals on grid...\n";
        population_.place(reng_, rd_);
    }

    void Simulation::run()
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
                population_.ecologicalStep(param_, reng_, rd_, resources_, timer);
                auto explor_dat = population_.getExplorationData();
                exploration_out_.insert(std::end(exploration_out_), std::begin(explor_dat), std::end(explor_dat));

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

                    // write out resources into vector
                    resources_out_.push_back(   std::to_string(timer) + "," +
                                                std::to_string(resources_.lin_to_coord(cell).x) + "," +
                                                std::to_string(resources_.lin_to_coord(cell).y) + "," +
                                                std::to_string(resources_[cell]));
                }

                // EVOLUTIONARY TIME STEP

                std::cout << "Reproduction...\n";
                population_.evolutionaryStep(param_, reng_, rd_);

                vigilance_out_.push_back(   std::to_string(gen) + "," + 
                                            std::to_string( population_.meanVigilance() ));
            }
        }

        if (population_.size() == 0) std::cout << "Extinction, how sad\n";
    }

    void Simulation::save(std::ofstream& ofsR, std::ofstream& ofsV, std::ofstream& ofsE) 
    {
        write(resources_out_, ofsR);
        write(vigilance_out_, ofsV);
        write(exploration_out_, ofsE);
    }

    void Simulation::write(std::vector<std::string> v, std::ofstream& ofs)
    {
	    for(int i=0; i<v.size(); ++i)
        {
		    ofs << v[i] << std::endl;
	    }
	    ofs.close();
    }
}

#endif