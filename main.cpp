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
unsigned fire_stations_count;
/* Skody zo vsetkych poziarov */
unsigned damage_done = 0;
/* Cas trvania vsetkych poziarov */
unsigned fire_alive = 0;
/* Pocet poziarov */
unsigned fire_count = 0;

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
	Print("IMS - model hasickych stanic\n");
	Print("Trvanie: %d [m]", MINUTES);
	for(int i=0;i<5;i++)
	{
		Print("============================");
		Print("Experiment #%d",i);
		damage_done = 0;
		fire_alive = 0;
		fire_count = 0;
		fire_engines.clear();
		if(i==0)
		{
			fire_engines.pushback(FireEngine(4000,4000));
			fire_engines.pushback(FireEngine(8000,4000));
			fire_engines.pushback(FireEngine(4000,8000));
			fire_engines.pushback(FireEngine(8000,8000));
		}
		else if(i==1)
		{
			fire_engines.pushback(FireEngine(6000,6000));
			fire_engines.pushback(FireEngine(6000,6000));
			fire_engines.pushback(FireEngine(6000,6000));
			fire_engines.pushback(FireEngine(6000,6000));
		}
		else if(i==2)
		{
			fire_engines.pushback(FireEngine(6000,6000));
			fire_engines.pushback(FireEngine(6000,6000));
			fire_engines.pushback(FireEngine(6000,6000));
		}
		else if(i==3)
		{
			fire_engines.pushback(FireEngine(4000,8000));
			fire_engines.pushback(FireEngine(6000,4000));
			fire_engines.pushback(FireEngine(8000,8000));
		}
		else if(i==4)
		{
			fire_engines.pushback(FireEngine(2000,2000));
			fire_engines.pushback(FireEngine(2000,10000));
			fire_engines.pushback(FireEngine(10000,2000));
			fire_engines.pushback(FireEngine(10000,10000));
			fire_engines.pushback(FireEngine(6000,6000));		
		}

		fire_stations_count = fire_engines.size();
		Store experiment_stations( "fire_stations", fire_stations_count );
		fire_stations = &experiment_stations;

		Init(0, MINUTES);
		(new Generator())->Activate();
		Run();

		Print("Celkove skody: %d [Kc]",damage_done);
		Print("Celkove cas horenia: %d [Kc]",damage_done);
		Print("Pocet poziarov: %d [Kc]",damage_done);
	}
	return 0;
}