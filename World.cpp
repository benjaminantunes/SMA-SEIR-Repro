#include "World.hpp"
#include <typeinfo>
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

// -------------------------------------------------------------------- //
// World::World:  Constructeur de l'objet World                         //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    inSimulationParams :                                              //
//       Permet de récupérer les données du fichier de config           //
//                                                                      //                          
//    inNomFichierLog :                                                 //
//       Détermine le nom du fichier de log dans lequel                 //
//       on va stocker les résultats                                    //
//                                                                      //
//    inLog :                                                           //
//       Booléen pour déterminer si oui ou non on stocke les résultats  //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
World::World(SimulationParams * inSimulationParams,
             char             * inNomFichierLog, 
             bool               inLog)
{
   _size               = inSimulationParams->getSize();
                     
   _nbIteration        = inSimulationParams->getNbIteration();
                     
   _nbHumain           = inSimulationParams->getNbHumain();

   _nbMalade           = inSimulationParams->getNbMalade();
   
   _transmissionRate   = inSimulationParams->getTransmissionRate();
                     
   _durationImmunity   = inSimulationParams->getDurationImmunity();
                     
   _durationIncubation = inSimulationParams->getDurationIncubation();
                     
   _durationInfection  = inSimulationParams->getDurationInfection();
   
   _nbDeplacementJour  = inSimulationParams->getNbDeplacementJour();
                     

   _carte = (Human ***)malloc(_size * sizeof(Human**));
   for(int i = 0; i < _size; i++)
   {
      _carte[i] = (Human **)malloc(_size * sizeof(Human*));
   }
   
   for(int i = 0; i<_size ;i++)
   {
      for(int j = 0; j<_size ; j++)
      {
         
         _carte[i][j] = nullptr;
      }
   }

   _log = inLog;
   _stats["susceptible"] = 0;
   _stats["exposed"] = 0;
   _stats["infected"] = 0;
   _stats["recovered"] = 0;
   if(_log)
   {
      _logfile.open(inNomFichierLog,ios::out);
   }
   
}




// -------------------------------------------------------------------- //
// World::~World:  Destructeur de l'objet World                         //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    Pas d'entrée                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//   Pas de sortie                                                      //
// -------------------------------------------------------------------- //
World::~World()
{
    
   if(_log)
   {
      _logfile.close();
   }
}



// -------------------------------------------------------------------- //
// World::writeLog:  Permet d'écrire un message dans le fichier de log  //
//                                                                      //
// En entrée:                                                           // 
//                                                                      //
//    inSimulationParams :                                              //
//       Permet de récupérer les données du fichier de config           //
//                                                                      //
//    inNomFichierLog :                                                 //
//       Détermine le nom du fichier de log dans lequel                 //
//       on va stocker les résultats                                    //
//                                                                      //
//    inLog :                                                           //
//       Booléen pour déterminer si oui ou non on stocke les résultats  //
//                                                                      //
// En sortie:                                                           //
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
void World::writeLog(string inMsg)
{
   _logfile << inMsg;
   _logfile << endl;
}


// -------------------------------------------------------------------- //
// World::pause:  Permet d'afficher un message de pause                 //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    Pas d'entrée                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//   Pas de sortie                                                      //
// -------------------------------------------------------------------- //
void World::pause()
{
   cout << "Hit <Return to continue" << endl ;
}



// -------------------------------------------------------------------- //
// World::Display:                                                      //
//    Permet d'afficher l'état actuel de la carte et des Humains        //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    Pas d'entrée                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//   Pas de sortie                                                      //
// -------------------------------------------------------------------- //
void World::display()
{
   cout << endl;
   cout << endl;
   cout << endl;
   cout << endl;
   cout << endl;
   cout << "*****************************************************************";
   for(int row = 0; row<_size; row++)
   {
      cout << "   " << endl;
      for(int column = 0; column<_size; column++ )
      {
         if(_carte[row][column] == nullptr)
         {
            // Null n'existe pas en C++. J'ai initialisé à NULL, 
            //donc j'imagine un tableau rempli de 0.
            cout << World::SYMBOL_EMPTY;
         }
         else
         {            
            cout << _carte[row][column]->to_string();
         }

      }
      cout << endl;
   }
}



// -------------------------------------------------------------------- //
// World::updateStats:                                                  //
//    Permet de mettre à jour les statistiques des Humains.             //
//    Les statistiques sont : Le nombre d'humain qui n'ont pas eu la    //
//    maladie, le nombre d'humain qui ont eu la maladie puis ce sont    //
//    retabli, le nombre d'humain actuellement contaminé, et enfin      //
//    le nombre d'humain qui sont morts.                                //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    inState :                                                         //
//       String représentant le changement d'état de l'Humain           //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//   Pas de sortie                                                      //
// -------------------------------------------------------------------- //
void World::updateStats(string inState)
{
   if(inState == "recovered")
   {
      _stats.at("infected") -= 1;
      _stats.at("recovered") += 1;     
   }
   else if(inState == "infected")
   {
      _stats.at("exposed") -= 1;
      _stats.at("infected") += 1;
   }
   else if(inState == "exposed")
   {
      _stats.at("susceptible") -= 1;
      _stats.at("exposed") += 1;
   }
   else if(inState == "susceptible")
   {
      _stats.at("recovered") -= 1;
      _stats.at("susceptible") += 1;
   }
}


// -------------------------------------------------------------------- //
// World::displayStats:  Affiche les statistiques                       //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    Pas d'entrée                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//   Pas de sortie                                                      //
// -------------------------------------------------------------------- //
void World::displayStats()
{
   cout << "***************STATISTICS******************" <<endl;
   cout <<"Susceptible : " << _stats.at("susceptible") << endl;
   cout <<"Exposed : " << _stats.at("exposed") << endl;
   cout <<"Infected : " << _stats.at("infected") << endl;
   cout <<"Recovered : " << _stats.at("recovered") << endl;
}


// -------------------------------------------------------------------- //
// World::isValid:  Teste si la position est valide (dans la carte)     //
//                                                                      //
// En entrée:                                                           // 
//                                                                      //
//    inRow :                                                           //
//       Ligne de la position à tester                                  //
//    inColumn :                                                        //
//       Colonne de la position à tester                                //
//                                                                      //
// En sortie:                                                           //
//                                                                      //
//    Booléen :                                                         //
//       Oui ou non la position est valide                              //
// -------------------------------------------------------------------- //
bool World::isValid(int inRow, int inColumn)
{
   return inRow >= 0 && inColumn >= 0 && inRow < _size and inColumn < _size;
}



// -------------------------------------------------------------------- //
// World::isHuman:  Teste si la position est un humain                  //
//                                                                      //
// En entrée:                                                           // 
//                                                                      //
//    inRow :                                                           //
//       Ligne de la position à tester                                  //
//    inColumn :                                                        //
//       Colonne de la position à tester                                //
//                                                                      //
// En sortie:                                                           //
//                                                                      //
//    Booléen :                                                         //
//       Oui ou non la position est un humain                           //
// -------------------------------------------------------------------- //
bool World::isHuman(int inRow, int inColumn)
{
   if(isValid(inRow,inColumn) && _carte[inRow][inColumn] != nullptr)
   {
      return
         isValid(inRow,inColumn) && 
         (
            _carte[inRow][inColumn]->to_string() == 'S' ||
            _carte[inRow][inColumn]->to_string() == 'E' ||
            _carte[inRow][inColumn]->to_string() == 'I' ||
            _carte[inRow][inColumn]->to_string() == 'R'
               
           );
   }
   return false;

}



// -------------------------------------------------------------------- //
// World::isEmpty:  Teste si la position est vide                       //
//                                                                      //
// En entrée:                                                           // 
//                                                                      //
//    inRow :                                                           //
//       Ligne de la position à tester                                  //
//    inColumn :                                                        //
//       Colonne de la position à tester                                //
//                                                                      //
// En sortie:                                                           //
//                                                                      //
//    Booléen :                                                         //
//       Oui ou non la position est vide                                //
// -------------------------------------------------------------------- //
bool World::isEmpty(int inRow, int inColumn)
{
   return isValid(inRow,inColumn) && _carte[inRow][inColumn] == nullptr;
}


// -------------------------------------------------------------------- //
// World::addAgent:  Initialise la carte en ajoutant les Agents         //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    inSimulationParams :                                              //
//       Les paramètres du fichier de configuration                     //
//                                                                      //
//    inWorldMax :                                                      //
//        Facteur déterminant le nombre maximal d'humain                //
//        (Param à supprimer ?)                                         //
//                                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//   Pas de sortie                                                      //
// -------------------------------------------------------------------- //
void World::addAgent(SimulationParams * inSimulationParams, 
                     float              inWorldMax 
                    )
{


   int maxAgents = pow(_size,2) * inWorldMax;
   _stats.at("susceptible") = _nbHumain - _nbMalade;
   _stats.at("infected") = _nbMalade;


   if(_nbHumain > maxAgents)
   {
      cout << "trop d'agent" << endl;
      exit(1);
   }

   if(_nbMalade > _nbHumain)
   {
      cout << "trop de malade" << endl;
      exit(1);
   }
   
   for(int qtyAgent = 0; qtyAgent < _nbHumain-_nbMalade; qtyAgent++)
   {        
      bool  varEmpty = false;
      int   row;
      int   column;
        
      while(!varEmpty)
      {
            
         // On cherche une position libre sur la carte 
         row = randmt->genrand_int32()%_size ;
         column = randmt->genrand_int32()%_size ;
         varEmpty = isEmpty(row,column);
      }

         
      _carte[row][column] = new Human(inSimulationParams,row,column);
      
      _humanSusceptiblePositions.push_back(_carte[row][column]->getPosition());
      
   }
   

   for(int qtyAgents = 0; qtyAgents<_nbMalade;qtyAgents++)
   {
      
      bool varEmpty = false;
      int row;
      int column;
      while(!varEmpty)
      {
         row = randmt->genrand_int32()%_size ;
         column = randmt->genrand_int32()%_size ;
         varEmpty = isEmpty(row,column);
      }
      
      _carte[row][column] = new Human(inSimulationParams,row,column);
      _carte[row][column]->setStateName("infected");
      _humanInfectedPositions.push_back(_carte[row][column]->getPosition());
      

   }
   

}


// -------------------------------------------------------------------- //
// World::initialize:  Initialise en appelant addAgent                  // 
//                     (Fonction inutile ?)                             //
//                                                                      //
// En entrée:                                                           // 
//                                                                      //
//    inSimulationParams :                                              //
//       Les paramètres du fichier de configuration                     //
//                                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//   Pas de sortie                                                      //
// -------------------------------------------------------------------- //
void World::initialize(SimulationParams * inSimulationParams)
{
   addAgent(inSimulationParams,World::MAX_HUMANS);
}



// -------------------------------------------------------------------- //
// World::vision:  Détermine le contenu des cases voisines d'une cases  //
//                                                                      //
// En entrée:                                                           // 
//                                                                      //
//    inLength :                                                        //
//       La distance à observer (voisinage de Moore)                    //
//                                                                      //
//    inRow :                                                           //
//       La ligne de départ (Position X)                                //
//                                                                      //
//    inColumn :                                                        //
//       La colonne de départ (Position Y)                              //
//                                                                      //
// En sortie:                                                           //
//                                                                      //
//    Un dictionnaire/map associant les cases vides à une liste de      //
//    positions et les cases occupées à une liste de positions.         // 
// -------------------------------------------------------------------- //
map<string,vector<Position>> World::vision(int inLength,
                                           int inRow, 
                                           int inColumn)
{


   Position                      maPositionTest;
   map<string, vector<Position>> neighborhood;

   neighborhood["empty"];
   neighborhood["human"];
    

   for(int elt_1 = 0-inLength; elt_1<inLength+1;elt_1++)
   {
      for(int elt_2 = 0-inLength; elt_2<inLength+1;elt_2++)
      {

         maPositionTest.setPosX(elt_1);
         maPositionTest.setPosY(elt_2);

         bool isInNeighborhood = false;

         for(Position  temp: neighborhood.at("empty") )
         {
            if(temp == maPositionTest)
            {
               isInNeighborhood = true;
            }
         }
         for(Position  temp: neighborhood.at("human") )
         {

            if(temp == maPositionTest)
            {
               isInNeighborhood = true;
            }
         }


         
         // Tous ça pour ça : and (elt_1, elt_2) not in neighborhood
         if((elt_1 != 0 || elt_2 !=0) && !isInNeighborhood )
         {
            
            
            if(isHuman(inRow+elt_1,inColumn+elt_2))
            {
               neighborhood["human"].push_back(_carte[inRow+elt_1][inColumn+elt_2]->getPosition());
            }
            if(isEmpty(inRow+elt_1,inColumn+elt_2))
            {
               Position pos;
               pos.setPosX(inRow+elt_1);
               pos.setPosY(inColumn+elt_2);
               neighborhood["empty"].push_back(pos);
            }
            
         }
      }
   }

   return neighborhood;
}





// -------------------------------------------------------------------- //
// World::Contamination:                                                // 
//    Un malade essaye de contaminer ses voisins sains                  //
//    lors d'un déplacement                                             //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    inRow :                                                           //
//       La ligne de destination de l'humain malade                     //
//                                                                      //
//    inColumn :                                                        //
//       La colonne de destination de l'humain malade                   //
//                                                                      //
//                                                                      //
//    inCurrentRow :                                                    //
//       La ligne actuelle de l'humain malade                           //
//                                                                      //
//    inCurrentColumn :                                                 //
//       La colonne actuelle de l'humain malade                         //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
void World::contamination(int inRow, int inColumn, int inCurrentRow, int inCurrentColumn)
{
   
   map<string, vector<Position>> target_v1 = vision(2,inRow,inColumn);
   Position                      maPositionTest;

   
   for(Position  pos : target_v1["human"])
   {
   
      if(_carte[pos.getPosX()][pos.getPosY()]->to_string() == 'S')
      {
      
         float randomValue = randmt->genrand_real1();

         if(randomValue < _transmissionRate)
         {
            _carte[pos.getPosX()][pos.getPosY()]->setStateName("exposed");
            _newHumanExposedPositions.push_back(
                                            Position(pos.getPosX(),
                                                    pos.getPosY()));
    
    
            maPositionTest.setPosX(pos.getPosX());
            maPositionTest.setPosY(pos.getPosY());
            int index = 0;
            for(Position  temp: _newHumanSusceptiblePositions )
            {

                if(temp == maPositionTest)
                {
                    _newHumanSusceptiblePositions.erase(
                                _newHumanSusceptiblePositions.begin()+index
                                            );
                }
                index++;
            }

            updateStats("exposed");
         }
      }
   }
}



// -------------------------------------------------------------------- //
// World::humanGoFromTo:                                                //
//    Déplace un humain sur la carte, et le tue s'il doit mourir        //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    inFromRow :                                                       //
//       La ligne actuelle de l'humain                                  //
//                                                                      //
//    inFromColumn :                                                    //
//       La colonne actuelle de l'humain                                //
//                                                                      //
//    inToRow :                                                         //
//       La ligne de destination de l'humain                            //
//                                                                      //
//    inToColumn :                                                      //
//        La colonne de destination de l'humain                         //
//                                                                      //
//    inDie :                                                           //
//       Booléen pour définir si l'humain doit mourir                   //
//                                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
void World::humanGoFromTo(int  inFromRow,
                          int  inFromColumn, 
                          int  inToRow, 
                          int  inToColumn)
{
    

   _carte[inToRow][inToColumn] = _carte[inFromRow][inFromColumn];
   _carte[inToRow][inToColumn]->setPosition(inToRow,inToColumn);
   _carte[inFromRow][inFromColumn] = nullptr;
   // Interet de rajouter cette ligne ? Bizarre
   //_carte[inToRow][inToColumn]->setPosition(inToRow,inToColumn);
}





// -------------------------------------------------------------------- //
// World::moveHumanSusceptible:                                         //
//                        Réalise le comportement des humains sains     //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    inRow :                                                           //
//       La ligne de l'humain sur la carte (Position X sur la carte)    //
//                                                                      //
//    inColumn :                                                        //
//       La colonne de l'humain sur la carte (Position Y sur la carte)  //
//                                                                      //
//                                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
void World::moveHumanSusceptible(int inRow, int inColumn)
{

   
   int rowDeplacement;
   int columnDeplacement;

   // Il peut se déplacer de 0 à size
   rowDeplacement = randmt->genrand_int32()%_size; 
   columnDeplacement = randmt->genrand_int32()%_size; 

   map<string, vector<Position>> target_v1 = vision(1,rowDeplacement,columnDeplacement);

   if(target_v1["empty"].size() != 0)
   {
      int taille = target_v1.at("empty").size();
      int randomValue = ((long)floor(randmt->genrand_int32()))%taille;
      humanGoFromTo(inRow,
                    inColumn, 
                    target_v1.at("empty").at(randomValue).getPosX(),
                    target_v1.at("empty").at(randomValue).getPosY()
                   );
      _newHumanSusceptiblePositions.push_back(target_v1.at("empty").at(randomValue));
      _carte[target_v1.at("empty").at(randomValue).getPosX()][target_v1.at("empty").at(randomValue).getPosY()]->incrementState();
   }
   else
   {
      _newHumanSusceptiblePositions.push_back(_carte[inRow][inColumn]->getPosition());
      _carte[inRow][inColumn]->incrementState();
   }
   

}




// -------------------------------------------------------------------- //
// World::moveHumanAsymptomatique:                                      //
//    Réalise la comportement des humains asymptomatique                //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    inRow :                                                           //
//       La ligne de l'humain sur la carte (Position X sur la carte)    //
//                                                                      //
//    inColumn :                                                        //
//       La colonne de l'humain sur la carte (Position Y sur la carte)  //
//                                                                      //
//                                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
void World::moveHumanInfected(int inRow, int inColumn)
{

   
   if(_carte[inRow][inColumn]->getState() > _durationInfection)
   {
      _carte[inRow][inColumn]->resetState();
      _carte[inRow][inColumn]->setStateName("recovered");
      updateStats("recovered");
      _newHumanRecoveredPositions.push_back(Position(inRow, inColumn)); 
      return;
          
   }

   int rowDeplacement;
   int columnDeplacement;

   // Il peut se déplacer de 0 à size
   rowDeplacement = randmt->genrand_int32()%_size; 
   columnDeplacement = randmt->genrand_int32()%_size; 
   
   map<string, vector<Position>> target_v1 = vision(1,rowDeplacement,columnDeplacement);


   if(target_v1["empty"].size() != 0)
   {
   
      int taille = target_v1.at("empty").size();
      int randomValue = ((long)floor(randmt->genrand_int32()))%taille;

      contamination(target_v1.at("empty").at(randomValue).getPosX(),
                     target_v1.at("empty").at(randomValue).getPosY(),
                     inRow,
                     inColumn
                     );

      humanGoFromTo(inRow,
                     inColumn, 
                     target_v1.at("empty").at(randomValue).getPosX(),
                     target_v1.at("empty").at(randomValue).getPosY()
                     );


      _newHumanInfectedPositions.push_back(
                                       target_v1.at("empty").at(randomValue)
                                                         );
      
   }
   else
   {
      contamination(inRow,inColumn, inRow, inColumn);
      _newHumanInfectedPositions.push_back(
                                       _carte[inRow][inColumn]->getPosition()
                                                         );
   }
   
   
}





// -------------------------------------------------------------------- //
// World::moveHumanConfined:                                            //
//    Réalise la comportement des humains confiné                       //
//    Les humains contaminés qui ne sont pas asymptomatique             //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    inRow :                                                           //
//       La ligne de l'humain sur la carte (Position X sur la carte)    //
//                                                                      //
//    inColumn :                                                        //
//       La colonne de l'humain sur la carte (Position Y sur la carte)  //
//                                                                      //
//                                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
void World::moveHumanExposed(int inRow, int inColumn)
{

   if(_carte[inRow][inColumn]->getState() > _durationIncubation)
   {  
      _carte[inRow][inColumn]->resetState();
      _carte[inRow][inColumn]->setStateName("infected");
      _newHumanInfectedPositions.push_back(_carte[inRow][inColumn]->getPosition());
      updateStats("infected");
      return;
   
   }
   int rowDeplacement;
   int columnDeplacement;

   // Il peut se déplacer de 0 à size
   rowDeplacement = randmt->genrand_int32()%_size; 
   columnDeplacement = randmt->genrand_int32()%_size; 
   

   map<string, vector<Position>> target_v1 = vision(1,rowDeplacement,columnDeplacement);
   
   if(target_v1["empty"].size() != 0)
   {
      int taille = target_v1.at("empty").size();
      int randomValue = ((long)floor(randmt->genrand_int32()))%taille;
      humanGoFromTo(inRow,
                    inColumn, 
                    target_v1.at("empty").at(randomValue).getPosX(),
                    target_v1.at("empty").at(randomValue).getPosY()
                   );
      _newHumanExposedPositions.push_back(target_v1.at("empty").at(randomValue));
      _carte[target_v1.at("empty").at(randomValue).getPosX()][target_v1.at("empty").at(randomValue).getPosY()]->incrementState();
   }
   else
   {
      _newHumanExposedPositions.push_back(_carte[inRow][inColumn]->getPosition());
      _carte[inRow][inColumn]->incrementState();
   }
   

}




// -------------------------------------------------------------------- //
// World::moveHumanHospital:                                            //
//    Réalise le comportement des humains à l'hopital                   //
//                                                                      //
// En entrée:                                                           //
//                                                                      //
//    inRow :                                                           //
//       La ligne de l'humain sur la carte (Position X sur la carte)    //
//                                                                      //
//    inColumn :                                                        //
//       La colonne de l'humain sur la carte (Position Y sur la carte)  //
//                                                                      //
//                                                                      //
//                                                                      //
// En sortie:                                                           // 
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
void World::moveHumanRecovered(int inRow, int inColumn)
{


   if(_carte[inRow][inColumn]->getState() > _durationImmunity)
   { 
      _carte[inRow][inColumn]->setStateName("susceptible");
      _newHumanSusceptiblePositions.push_back(_carte[inRow][inColumn]->getPosition());
      _carte[inRow][inColumn]->resetState();
      updateStats("susceptible");
      return;
   }
  
   int rowDeplacement;
   int columnDeplacement;

   // Il peut se déplacer de 0 à size
   rowDeplacement = randmt->genrand_int32()%_size; 
   columnDeplacement = randmt->genrand_int32()%_size; 

   map<string, vector<Position>> target_v1 = vision(1,rowDeplacement,columnDeplacement);

   if(target_v1["empty"].size() != 0)
   {
      int taille = target_v1.at("empty").size();
      int randomValue = ((long)floor(randmt->genrand_int32()))%taille;
      humanGoFromTo(inRow,
                    inColumn, 
                    target_v1.at("empty").at(randomValue).getPosX(),
                    target_v1.at("empty").at(randomValue).getPosY()
                   );
      _newHumanRecoveredPositions.push_back(target_v1.at("empty").at(randomValue));
      _carte[target_v1.at("empty").at(randomValue).getPosX()][target_v1.at("empty").at(randomValue).getPosY()]->incrementState();
   }
   else
   {
      _newHumanRecoveredPositions.push_back(_carte[inRow][inColumn]->getPosition());
      _carte[inRow][inColumn]->incrementState();
   }
   

}





// -------------------------------------------------------------------- //
// World::nextIteration:                                                //
//    Effectue les changements necessaires et écrit les log             //
//    pour passer à l'itération suivante                                //
//                                                                      //
// En entrée:                                                           // 
//    Pas d'entrée                                                      //
//                                                                      //
// En sortie:                                                           //
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
void World::nextIteration()
{
   writeLog(to_string(_iteration));
   writeLog(to_string(_stats.at("susceptible")));
   writeLog(to_string(_stats.at("exposed")));
   writeLog(to_string(_stats.at("infected")));
   writeLog(to_string(_stats.at("recovered")));
   writeLog("##########");

   _iteration += 1;
   /*
   printf("nb iteration = %d\n",_iteration);
   printf("nb humain susceptible = %d\n", _humanSusceptiblePositions.size());
   printf("nb humain exposed = %d\n", _humanExposedPositions.size());
   printf("nb humain infected = %d\n", _humanInfectedPositions.size());
   printf("nb humain recovered = %d\n", _humanRecoveredPositions.size());
   */
   
   for(Position  temp: _humanSusceptiblePositions)
   {
      moveHumanSusceptible(temp.getPosX(),temp.getPosY());
   }

   //////////////////////////////////////////////////////////  

    
   for(Position  temp: _humanExposedPositions)
   {
      moveHumanExposed(temp.getPosX(),temp.getPosY());

   }

   ////////////////////////////////////////////////

   for(int x = 0; x < _nbDeplacementJour; x++)
   {
      for(Position  temp: _humanInfectedPositions)
      {
         moveHumanInfected(temp.getPosX(),temp.getPosY());
      
      }
      _humanInfectedPositions = _newHumanInfectedPositions;
      _newHumanInfectedPositions.clear();
   }
   for(Position  temp: _humanInfectedPositions)
   {
      _carte[temp.getPosX()][temp.getPosY()]->incrementState();
   }

   
 
   

////////////////////////////////////////////////////////////

   for(Position  temp: _humanRecoveredPositions)
   {
      moveHumanRecovered(temp.getPosX(),temp.getPosY());

   }

   _humanSusceptiblePositions = _newHumanSusceptiblePositions;
   _humanExposedPositions = _newHumanExposedPositions;
   _humanRecoveredPositions = _newHumanRecoveredPositions;
   
   _newHumanSusceptiblePositions.clear();
   _newHumanExposedPositions.clear();
   _newHumanRecoveredPositions.clear();
   
}




// -------------------------------------------------------------------- //
// World::startSimulation:  Démarre la simulation                       //
//                                                                      //
// En entrée:                                                           // 
//                                                                      //
//    inSimulationParams :                                              //
//       Objet contenant les paramètres du fichier de configuration     //
//                                                                      //
//                                                                      //
// En sortie:                                                           //
//                                                                      //
//    Pas de sortie                                                     //
// -------------------------------------------------------------------- //
void World::startSimulation(SimulationParams * inSimulationParams)
{


   for(int iteration = 0;
       iteration < inSimulationParams->getNbIteration();
       iteration++)
   {
      displayStats();
      //display();
      
      //World::pause();
      nextIteration();
      /*
      if(_humansPosition.size() == 0){
         cout << "no more humans in the simulation" << endl;
         //self.writeLog('[STOP] No more human in the simulation !\n')
         break;
      }
      */
   }

   //cout << "\033[2J" << endl;
   displayStats();
   
}


