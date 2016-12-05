#include <iostream>
#include "simlib.h"
#include <stdlib.h> 
#include <math.h>

#include "params.h"
#include "fire_engine.h"
#include "fire.h"

/* Hasicke vozidla */
FireEngine* fire_engines;
Store *fire_stations;
unsigned fire_stations_count;
/* Skody zo vsetkych poziarov */
unsigned damage_done = 0;
/* Cas trvania vsetkych poziarov */
unsigned fire_alive = 0;

// Generator poziarov
class Generator : public Event {
	void Behavior() 
	{
		(new Fire)->Activate();
		Activate(Time+Exponential(FIRE_X));
	}
};

int main() 
{
	FireEngine experiment_engines[] = 
	{
		FireEngine(5400,5400), FireEngine(5400,10600), FireEngine(10600,10600), FireEngine(10600,5400)
	};
	fire_engines = experiment_engines;
	fire_stations_count = sizeof(experiment_engines)/sizeof(FireEngine);
	Store experiment_stations( "fire_stations", fire_stations_count );
	fire_stations = &experiment_stations;

	Init(0, 120);	
	(new Fire())->Activate();
	Run();
	for(int i=0;i<fire_stations_count;i++)
	{
		std::cout << "Engine " << fire_engines[i].state << std::endl;
	}
	return 0;

	
	SetOutput("multiexp.dat");
	Print("# MULTIEXP - multiple experiments example\n");
	Print("# Experiment (čas=%g) \n", MINUTES);

	Init(0, MINUTES);
	
	Run();

	return 0;
}