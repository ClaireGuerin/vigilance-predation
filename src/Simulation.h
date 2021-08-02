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

            void run();

            void save();

        private:
            std::string dir_;
            Parameter param_;
            std::default_random_engine reng_;
            Population population_;
            Grid<double> resources_;

    };

    Simulation::Simulation(std::string path) :
        dir_(path),
        param_(),
        reng_()
    {
        assert(param_.v >= 0.0 && param_.v <= 1.0);
    }

    void Simulation::setup() 
    {
        Random rd{ param_ };
        population_(param_);
        std::cout << "Pop size: " << population_.size() << "\n";
        resources_( param_.edgeSize, param_.initResources );
    }
}

#endif