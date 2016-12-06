#include <iostream>
#include "simlib.h"
#include <stdlib.h> 
#include <math.h>
#include <vector>

#include "params.h"
#include "fire_engine.h"
#include "fire.h"

/* Hasicke vozidla */
std::vector<FireEngine> fire_engines;
Store *fire_stations;
unsigned fire_stations_count = 0;
/* Skody zo vsetkych poziarov */
unsigned damage_done = 0;
/* Cas trvania vsetkych poziarov */
double fire_alive = 0;
/* Pocet poziarov */
unsigned fire_count = 0;
/* Poziare ktore boli uhasene predtym nez dosli vsetky auta */
unsigned not_all_engines = 0;

// Generator poziarov
class Generator : public Event {
	void Behavior() 
	{
		(new Fire)->Activate();
		Activate(Time+Exponential(FIRE_X));
		fire_count++;
	}
};

int main() 
{
	RandomSeed(time(NULL));
	Print("IMS - model hasickych stanic\n");
	Print("Trvanie: %d [m]\n", MINUTES);
	for(int i=0;i<5;i++)
	{
		Print("============================\n");
		Print("Experiment #%d\n",i);
		damage_done = 0;
		fire_alive = 0;
		fire_count = 0;
		not_all_engines = 0;
		fire_engines.clear();
		if(i==0)
		{
			fire_engines.push_back(FireEngine(4000,4000));
			fire_engines.push_back(FireEngine(8000,4000));
			fire_engines.push_back(FireEngine(4000,8000));
			fire_engines.push_back(FireEngine(8000,8000));
		}
		else if(i==1)
		{
			fire_engines.push_back(FireEngine(6000,6000));
			fire_engines.push_back(FireEngine(6000,6000));
			fire_engines.push_back(FireEngine(6000,6000));
			fire_engines.push_back(FireEngine(6000,6000));
		}
		else if(i==2)
		{
			fire_engines.push_back(FireEngine(6000,6000));
			fire_engines.push_back(FireEngine(6000,6000));
			fire_engines.push_back(FireEngine(6000,6000));
		}
		else if(i==3)
		{
			fire_engines.push_back(FireEngine(4000,8000));
			fire_engines.push_back(FireEngine(6000,4000));
			fire_engines.push_back(FireEngine(8000,8000));
		}
		else if(i==4)
		{
			fire_engines.push_back(FireEngine(2000,2000));
			fire_engines.push_back(FireEngine(2000,10000));
			fire_engines.push_back(FireEngine(10000,2000));
			fire_engines.push_back(FireEngine(10000,10000));
			fire_engines.push_back(FireEngine(6000,6000));		
		}

		Store experiment_stations( "experiment_stations", fire_engines.size() );
		fire_stations_count = fire_engines.size();
		fire_stations = &experiment_stations;

		Init(0, MINUTES);
		fire_stations->Clear();
		(new Generator())->Activate();
		Run();

		fire_stations->Output();
		Print("Celkove skody: %u [Kc]\n",damage_done);
		Print("Celkovy cas horenia: %0.2f [m]\n",fire_alive);
		Print("Pocet poziarov: %d\n",fire_count);
		Print("Poziare s nedostatkom aut: %d\n",not_all_engines);		
	}
	return 0;
}