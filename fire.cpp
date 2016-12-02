#include <iostream>
#include "simlib.h"
#include <stdlib.h> 
#include <math.h>

#include "params.h"
#include "fire_engine.h"
#include "fire.h"

FireAlarm::FireAlarm(Fire* poziar)
{
	this->obsluhovany_poziar = poziar;
	Priority = 20;
}

void FireAlarm::Behavior()
{
	obsluhovany_poziar->exception = 1;
	obsluhovany_poziar->Activate();
}

void Fire::Behavior() 
{
	unsigned duration = this->start_fire();

	FireEngine engines = {NULL,NULL,NULL};
	unsigned engine_count = 0;

	// Ziskanie aut
	while(engine_count < this->intensity)
	{

	}

	



	// Hasenie poziaru
	Priority = 2;
	
	{
		this->damage = (this->damage + this->damage_inc) % this->max_damage;
		Wait(Time+1);
	}

	for(int i=0;i<3;i++)
	{

	}

	/*
	// Spocitame dostupne vozidla
	for(int i=0;i<0;i++)
	{
		if(engines[i]->state > HOME)
			engines++;
	}
	*/
}

double Fire::strength_dec_index(unsigned engines)
{
	switch(this->intensity)
	{
		case 1:
			return this->strength_dec;
		case 2:
			switch(engines)
			{
				case 1:
					return this->strength_dec*0.5;
				case 2:
					return this->strength_dec;
			}
		case 3:
			switch(engines)
			{
				case 1:
					return this->strength_dec*0.2;
				case 2:
					return this->strength_dec*0.6;
				case 3:
					return this->strength_dec;
			}
	}
}

unsigned Fire::start_fire()
{
	// Random fire position
	this->position[0] = MAP_X * Random();
	this->position[1] = MAP_Y * Random();
	unsigned duration;
	double random = Random();
	switch(1) // (unsigned)Uniform(1,4)
	{
		case 1:
			this->intensity=1;

			// Odpadkovy kos
			this->strength = 20;
			this->max_strength = 100;
			this->strength_dec = 30;
			this->strength_inc = 10;
			this->damage = 0;
			this->damage_inc = 0;
			this->max_damage = 0;
			duration = 40;

			break;
		case 2:
			this->intensity=2;
			break;
		case 3:
			this->intensity=3;
			break;
	}
	return duration;
}