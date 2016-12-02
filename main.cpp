#include <iostream>
#include "simlib.h"
#include <stdlib.h> 
#include <math.h>

#include "params.h"
#include "fire_engine.h"
#include "fire.h"

// Generator poziarov
class Generator : public Event {
	void Behavior() 
	{
		(new Fire)->Activate();
		Activate(Time+Exponential(FIRE_X));
	}
};

/* Hasicke vozidla */
FireEngine* fire_engines;

/* Simulacia stanic */
Store fire_stations( "fire_stations", 4 );

int main() 
{
	FireEngine experiment_engines[] = 
	{
		FireEngine(5400,5400), FireEngine(5400,10600), FireEngine(10600,10600), FireEngine(10600,5400)
	};
	fire_engines = experiment_engines;

	Init(0, 120);	
	(new Fire)->Activate();
	(new Fire)->Activate();
	Run();
	return 0;

	//std::cout << (FireEngine(1000,1000)).travel_time(0,0) << std::endl;
	SetOutput("multiexp.dat");
	Print("# MULTIEXP - multiple experiments example\n");
	Print("# Experiment (čas=%g) \n", MINUTES);

	Init(0, MINUTES);
	
	Run();

	return 0;
}