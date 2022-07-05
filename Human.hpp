#pragma once

#include <string>

#include "Position.hpp"
#include "RandMT.hpp"
#include "SimulationParams.hpp"

extern RandMT * randmt;

class Human 
{
   private:
      
      static const char SYMBOL_SUSCEPTIBLE = 'S';
      static const char SYMBOL_EXPOSED = 'E';
      static const char SYMBOL_INFECTED = 'I';
      static const char SYMBOL_RECOVERED = 'R';
      
      int      _state = 0;
      char     _symbol;
      Position _pos;
      string   _stateName;      

   public:
 
      Human(SimulationParams*,int,int);
      
      int       getState();
      void      resetState();
      void      incrementState();
      char      to_string();
      void      setPosition(int,int);
      Position  getPosition();
      string    getStateName();
      void      setStateName(string);

};
