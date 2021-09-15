#include <stdexcept>
#include <iostream>
//#include <vector>
//#include <random>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "src/Simulation.h"
#include "src/Individual.h"

int main(int argc, char* argv[]) 
{
  try 
  {
    //std::vector<vigi::Individual> {10, 2}

    std::string pathname = "../output/"; // default
    if (argc > 1) pathname = argv[1];

    std::ofstream ofsR(pathname + "resources_out.txt", std::ofstream::out);
    if (!ofsR.is_open())
    {
      throw std::runtime_error("unable to open resources file " + pathname + "resources_out.txt");
    }

    std::ofstream ofsV(pathname + "vigilance_out.txt");
    if (!ofsV.is_open())
    {
      std::cerr << "error: unable to open vigilance file\n";
      exit(EXIT_FAILURE);
    }

    std::ofstream ofsE(pathname + "exploration_out.txt");
    if (!ofsE.is_open())
    {
      std::cerr << "error: unable to open vigilance file\n";
      exit(EXIT_FAILURE);
    }

    vigi::Simulation sim(pathname);
    sim.setup();
    sim.run();
    sim.save(ofsR, ofsV, ofsE);

    if (sim.vis()) 
    {
      try
      {
        system("Rscript ../anim-vigil/animate_sim.r ../output/");
			  //system("xdg-open ../output/vigilance_out.gif");
			  //system("xdg-open //.output/grid_out.gif");
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << '\n';
      }
      
/* 
      struct stat info;

      if( stat( "anim-vigil/", &info ) != 0 )
        throw std::runtime_error("cannot find R animation program. Is it in root?\n");
      else if( info.st_mode & S_IFDIR )
      {
        system("Rscript anim-vigil/animate_sim.r");
			  system("xdg-open output/vigilance_out.gif");
			  system("xdg-open output/grid_out.gif");
      } else
        throw std::runtime_error(pathname + " is no directory\n"); */
    
    }

    
    
    return 0;
  }
  
  catch (const std::exception& err) 
  {
    std::cerr << "Exception: " << err.what() << '\n';
  }

  catch (...) 
  {
    std::cerr << "Unknown exception\n";
  }
  
  return -1;
}
