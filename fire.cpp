#include <iostream>
#include "simlib.h"
#include <stdlib.h> 
#include <math.h>
#include <vector>

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
	//Print("FireAlarm %f\n", Time);
	obsluhovany_poziar->exception = 1;
	obsluhovany_poziar->Activate();
}

FireEngineArrival::FireEngineArrival(Fire* fire, FireEngine* fire_engine)
{
	Priority = 3;
	this->fire = fire;
	this->fire_engine = fire_engine;
	this->fire_engine->state = TO_FIRE;
	this->fire_engine->alarm = (Event*)this;
	this->Activate(Time + fire_engine->travel_time(fire->position[0], fire->position[1]));
}

void FireEngineArrival::Behavior()
{
	//Print("FireEngineArrival %f\n", Time);
	// Zacina nova faza
	this->fire->phase_start = Time;
	// Vymazeme pointer na seba, aby sa potom nepokusal niekto dealokov
	// tento priestor
	this->fire_engine->alarm = NULL;

	// Vymazeme stary Timeout na uhasenie a urobme novy
	this->fire->strength = this->fire->current_strenght();
	this->fire_engine->state = ON_SITE;
	delete this->fire->alarm;
	this->fire->alarm = new FireAlarm(this->fire);
	this->fire->alarm->Activate(Time + abs(this->fire->strength / (this->fire->strength_inc - this->fire->strength_dec_index())));
}

void Fire::Behavior() 
{
	/*
	std::cout << "Start: " << Time << std::endl;
	std::cout << "max_duration: " << Time + this->max_duration << std::endl;
	*/
	this->exception = 0;
	this->beginning = Time;
	this->phase_start = Time;

	this->alarm = new FireAlarm(this);
	this->alarm->Activate(Time + this->max_duration);

	// Ziskanie aut
	for(unsigned i = 0; i < this->intensity; i++)
	{
		Enter(*fire_stations, 1);
		// Aby sme odteraz dostavali auta prioritne
		Priority = 1;
		
		// Poziar dohorel skor ako sme dostali auta
		if(exception)
		{
			not_all_engines++;
			goto END;
		}
		// Dostali sme auto
		else
		{
			int closest_station = -1;
			for (int i=0;i<fire_stations_count;i++)
			{
				if(fire_engines[i].state == HOME)
				{
					if(closest_station == -1)
					{
						closest_station = i;
					}
					else if(fire_engines[i].travel_time(this->position[0],this->position[1]) < fire_engines[closest_station].travel_time(this->position[0],this->position[1]))
					{
						closest_station = i;
					}
				}
			}
			new FireEngineArrival(this,&fire_engines[closest_station]);
			this->engines[i] = &fire_engines[closest_station];
		}
	}
	// Pockaj na uhasenie
	// Tuto by to mal event uhasit skor
	Wait(Time + this->max_duration);

	END:
	for(unsigned i = 0; i < this->intensity; i++)
	{
		if(this->engines[i] != NULL)
		{
			new FireEngineReturn(this->engines[i],this->position[0],this->position[1]);
		}
	}
	damage_done += this->get_damage();
	fire_alive += Time - this->beginning;
	//std::cout << "End: " << Time << std::endl;
}

unsigned Fire::engines_on_site()
{
	unsigned on_site = 0;
	for(unsigned i=0;i<this->intensity;i++)
	{
		if(this->engines[i] != NULL)
			if(this->engines[i]->state == ON_SITE)
				on_site++;
	}
	return on_site;
}

// Vypocita aktualnu silu poziaru
double Fire::current_strenght()
{
	double current_strenght = this->strength + (Time - this->phase_start) * (this->strength_inc - strength_dec_index());
	if (current_strenght <= 0)
		return 0;
	else
		return current_strenght;
}

// Vypocitaj skody
unsigned Fire::get_damage()
{
	return this->damage + (Time - this->beginning) * this->damage_inc;
}

double Fire::strength_dec_index()
{
	switch(this->intensity)
	{
		case 1:
			if (this->engines_on_site() == 1)
				return this->strength_dec;
		case 2:
			switch(this->engines_on_site())
			{
				case 1:
					return this->strength_dec*0.6;
				case 2:
					return this->strength_dec;
			}
		case 3:
			switch(this->engines_on_site())
			{
				case 1:
					return this->strength_dec*0.2;
				case 2:
					return this->strength_dec*0.6;
				case 3:
					return this->strength_dec;
			}
	}
	return 0;
}

Fire::Fire()
{
	// Random fire position
	this->position[0] = MAP_X * Random();
	this->position[1] = MAP_Y * Random();
	double subtype = Random();
	switch((unsigned)Uniform(1,4))
	{
		case 1:
		{
			this->intensity=1;
			if(subtype < 0.22)
			{
				// Auto
				this->strength = 10 + 10 * Random();
				this->max_strength = 60;
				this->strength_dec = 3;
				this->strength_inc = 1;

				this->damage = 4000;
				this->damage_inc = 400 + Random() * 200;
				this->max_duration = 120;
			}
			else if(subtype < 0.3)
			{
				// Porost
				this->strength = 5 + 10 * Random();
				this->max_strength = 80;
				this->strength_dec = 3;
				this->strength_inc = 1;
;
				this->damage = 400;
				this->damage_inc = 10 + Random() * 50;
				this->max_duration = 120;
			}
			else if(subtype < 0.34)
			{
				// Nizka budova
				this->strength = 10 + 20 * Random();
				this->max_strength = 35;
				this->strength_dec = 2;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 100 + Random() * 100;
				this->max_duration = 180;
			}
			else if(subtype < 0.92	)
			{
				// Kontajner
				this->strength = 2;
				this->max_strength = 10;
				this->strength_dec = 2;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 5 + Random() * 10;
				this->max_duration = 60;
			}
			else
			{
				// Vyskova budova
				this->strength = 10 + 25 * Random();
				this->max_strength = 35;
				this->strength_dec = 2;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 100 + Random() * 100;
				this->max_duration = 300;
			}
			break;
		}
		case 2:
		{
			this->intensity=2;
			if (subtype < 0.34)
			{
				// Nizka budova
				this->strength = 15 + 15 * Random();
				this->max_strength = 120;
				this->strength_dec = 2;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 200 + Random() * 300;
				this->max_duration = 360;
			}
			else if(subtype < 0.39	)
			{
				// Odpad a skladka
				this->strength = 20 + 10 * Random();
				this->max_strength = 80;
				this->strength_dec = 2;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 10 * Random();
				this->max_duration = 120;
			}
			else if(subtype < 0.51	)
			{
				// Polny porost
				this->strength = 10 + 30 * Random();
				this->max_strength = 100;
				this->strength_dec = 2;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 50 * Random();
				this->max_duration = 180;
			}
			else
			{
				// Vyskova budova
				this->strength = 20 + 15 * Random();
				this->max_strength = 35;
				this->strength_dec = 2;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 200 + Random() * 300;
				this->max_duration = 500;	
			}
			break;
		}
		case 3:
		{
			this->intensity=3;
			if (subtype < 0.44)
			{
				// Nizka bodova
				this->strength = 100 + 40 * Random();
				this->max_strength = 600;
				this->strength_dec = 3;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 200 + Random() * 500;
				this->max_duration = 420;
			}
			else if(subtype < 0.55)
			{
				// Priemyselna budova
				this->strength = 200 + 40 * Random();
				this->max_strength = 600;
				this->strength_dec = 3;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 800 + Random() * 1000;
				this->max_duration = 600;
			}
			else
			{
				// Vyskova budova
				this->strength = 100 + 40 * Random();
				this->max_strength = 600;
				this->strength_dec = 3;
				this->strength_inc = 1;

				this->damage = 0;
				this->damage_inc = 200 + Random() * 500;
				this->max_duration = 600;
			}
			break;
		}
	}
}

FireEngineReturn::FireEngineReturn(FireEngine* fire_engine, unsigned x, unsigned y)
{
	Priority = 4;
	this->fire_engine = fire_engine;
	this->Activate(Time + fire_engine->travel_time(x,y) + RESTORE_TIME);
	if(this->fire_engine->alarm != NULL)
	{
		delete(this->fire_engine->alarm);
		this->fire_engine->alarm = NULL;
	}
}

void FireEngineReturn::Behavior()
{
	//Print("FireEngineReturn %f\n", Time);
	this->fire_engine->state = HOME;
	Leave(*fire_stations, 1);
}