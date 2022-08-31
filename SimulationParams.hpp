#pragma once
#include <map>
#include <string>

using namespace std;

class SimulationParams
{

   private:

      int              _size;
      int              _nbMalade;
      int              _nbHumain;
      int              _nbDeplacementJour;
      int              _nbIteration;
      float            _transmissionRate;
      int              _durationIncubation;
      int              _durationInfection;
      int              _durationImmunity;
      map<string, int> _mapStringValues;

   public:
      SimulationParams(char *);
      int     getSize();
      int     getNbMalade();
      int     getNbHumain();
      int     getNbIteration();
      int     getNbDeplacementJour();
      float   getTransmissionRate();
      int     getDurationInfection();
      int     getDurationIncubation();
      int     getDurationImmunity();

};
