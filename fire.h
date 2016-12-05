#ifndef FIRE_H
#define FIRE_H

/* Skody zo vsetkych poziarov */
extern unsigned damage_done;
/* Cas trvania vsetkych poziarov */
extern unsigned fire_alive;

/* Poziar */
class FireAlarm;
class Fire : public Process 
{
	public:
		unsigned intensity;
		unsigned exception;
		// Zaciatok poziaru
		unsigned beginning;
		// Pozicia poziaru
		unsigned position[2];
		// Sila poziaru
		double strength;
		unsigned max_strength;
		// Znizenie intenzity, ak potrebny pocet vozidiel dojde
		unsigned strength_dec;
		// Zvysovanie sili poziaru
		unsigned strength_inc;
		// Napachane skody
		unsigned damage;
		// Zvysovanie sko
		unsigned damage_inc;
		// Maximalne trvanie
		unsigned max_duration;
		// Dostupne auta
		unsigned phase_start;
		FireAlarm* alarm;
		FireEngine* engines[3] = {NULL,NULL,NULL};

		void Behavior();
		void start_fire();
		double strength_dec_index(unsigned engines);
		unsigned engines_on_site();
		int current_strenght();
		unsigned get_damage(unsigned now);
		double strength_dec_index();
};

/* Timeout pre poziar */
class FireAlarm : public Event
{
	public:
		Fire* obsluhovany_poziar;

		FireAlarm(Fire* poziar);
		void Behavior();
};

/* Prichod hasickeho auta na scenu */
class FireEngineArrival : public Event
{
	public:
		Fire* fire;
		FireEngine* fire_engine;

		FireEngineArrival(Fire* fire, FireEngine* fire_engine);
		void Behavior();
};

/* Odchod hasickeho auta na stanicu */
class FireEngineReturn : public Process	
{
	public:
		FireEngine* fire_engine;

		FireEngineReturn(FireEngine* fire_engine, unsigned x, unsigned y);
		void Behavior();
};

#endif