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
	this->fire_engine->alarm = NULL;
	this->fire->strength = this->fire->current_strenght();
	this->fire_engine->state = ON_SITE;
	delete(this->fire->alarm);
	this->fire->alarm = new FireAlarm(this->fire);
	this->fire->alarm->Activate(Time + abs(this->fire->strength / (this->fire->strength_inc - this->fire->strength_dec_index())));
	this->fire->phase_start = Time;
}

void Fire::Behavior() 
{
	this->beginning = Time;
	this->phase_start = Time;
	this->start_fire();

	this->alarm = new FireAlarm(this);
	this->alarm->Activate(Time + this->max_duration);

	// Ziskanie aut
	for(unsigned i = 0; i < this->intensity; i++)
	{
		Enter(*fire_stations, 1);
		// Aby sme odteraz dostavali auta prioritne
		Priority = 1;
		// Vyberieme najblizsiu stanicu
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

		// Poziar dohorel skor ako sme dostali auta
		if(exception)
		{
			// Posleme vsetky auta naspat
			for(unsigned i = 0; i < this->intensity; i++)
			{
				if(this->engines[i] != NULL)
				{
					new FireEngineReturn(this->engines[i],this->position[0],this->position[1]);
				}
			}
			damage_done = this->get_damage(Time);
			fire_alive = Time - this->beginning;
			return;
		}
	}
	// Pockaj na uhasenie
	// Tuto by to mal event uhasit skor
	Wait(Time + this->max_duration);
	for(unsigned i = 0; i < this->intensity; i++)
	{
		if(this->engines[i] != NULL)
		{
			new FireEngineReturn(this->engines[i],this->position[0],this->position[1]);
		}
	}

	std::cout << Time - this->beginning << std::endl;
	damage_done = this->get_damage(Time);
	fire_alive = Time - this->beginning;
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
int Fire::current_strenght()
{
	int current_strenght = this->strength + (Time - this->phase_start) * (this->strength_inc - strength_dec_index());
	if (current_strenght <= 0)
		return 0;
	else
		return current_strenght;
}

// Vypocitaj skody
unsigned Fire::get_damage(unsigned now)
{
	return this->damage + (now - this->beginning) * this->damage_inc;
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
					return this->strength_dec*0.5;
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

void Fire::start_fire()
{
	// Random fire position
	this->position[0] = MAP_X * Random();
	this->position[1] = MAP_Y * Random();
	switch((unsigned)Uniform(1,4))
	{
		case 1:
			this->intensity=1;
			double subtype = Random();
			if(subtype < 0.22)
			{
				// Auto
				this->strength = 250;
				this->max_strength = 600;
				this->strength_dec = 30;
				this->strength_inc = 10;
				this->damage = 4000;
				this->damage_inc = 2000 + Random() * 2000;
				this->max_duration = 90;
			}
			else if(subtype < 0.3)
			{
				// Porost
				this->strength = 250;
				this->max_strength = 600;
				this->strength_dec = 30;
				this->strength_inc = 10;
				this->damage = 4000;
				this->damage_inc = 0;
				this->max_duration = 90;
			}
			else if(subtype < 0.34)
			{
				// Nizka budova
			}
			else if(subtype < 0.92	)
			{
				// Kontainer
				this->strength = 20;
				this->max_strength = 100;
				this->strength_dec = 30;
				this->strength_inc = 10;
				this->damage = 0;
				this->damage_inc = 0;
				this->max_duration = 60;
			}
			else
			{
				// Vyskova budova
			}
			break;
		case 2:
			this->intensity=2;
			double subtype = Random();
			if (subtype < 0.34)
			{
				// Nizka budova
			}
			else if(subtype < 0.39	)
			{
				// Odpad a skladka
			}
			else if(subtype < 0.51	)
			{
				// Polny porost
			}
			else
			{
				// Vyskova budova
			}
			break;
		case 3:
			this->intensity=3;
			double subtype = Random();
			if (subtype < 0.44)
			{
				// Nizka bodova
			}
			else if(subtype < 0.55)
			{
				// Priemyselna budova
			}
			else
			{
				// Vyskova budova
			}
			break;
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
	this->fire_engine->state = HOME;
	Leave(*fire_stations, 1);
}