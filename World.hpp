#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <deque>

#include "RandMT.hpp"
#include "Human.hpp"
#include "SimulationParams.hpp"

using namespace std;

extern RandMT * randmt;

class World{
   private:
      int              _size;
      int              _nbIteration;
      Human        *** _carte;
      int              _nbHumain;
      int              _nbMalade;
      int              _nbDeplacementJour;

      int              _durationIncubation;
      int              _durationInfection;
      float            _transmissionRate;
      int              _durationImmunity;
      
        
      vector<Position> _humanSusceptiblePositions;
      vector<Position> _humanExposedPositions;
      vector<Position> _humanInfectedPositions;
      vector<Position> _humanRecoveredPositions;
      
      vector<Position> _newHumanSusceptiblePositions;
      vector<Position> _newHumanExposedPositions;
      vector<Position> _newHumanInfectedPositions;
      vector<Position> _newHumanRecoveredPositions;
   
      bool             _log = false;
      fstream          _logfile;
      int              _iteration = 0;
      map<string, int> _stats;

      static const char SYMBOL_EMPTY = '.';
      static constexpr float MAX_HUMANS = 0.8;
      
      
      

   public:
      World(SimulationParams*,char *, bool);
      ~World();
      void                         writeLog(string);
      static void                  pause();
      void                         display();
      void                         updateStats(string);
      void                         displayStats();
      bool                         isValid(int,int);
      bool                         isHuman(int,int);
      bool                         isEmpty(int,int);
      void                         addAgent(SimulationParams*,float);
      void                         initialize(SimulationParams*);
      map<string,vector<Position>> vision (int,int,int);
      void                         contamination(int,int,int, int);
      void                         humanGoFromTo(int,int,int,int);
      
      // Si on avait eu plusieurs classes héritant de human pour asymp, hops, etc, on aurait pu utiliser le polymorphisme sur une seule fonction.
      void                         moveHumanSusceptible(int,int);
      void                         moveHumanExposed(int,int);
      void                         moveHumanInfected(int,int);
      void                         moveHumanRecovered(int,int);
      
      void                         nextIteration();
      void                         startSimulation(SimulationParams*);
};
