#ifndef FIRE_H
#define FIRE_H

/* Skody zo vsetkych poziarov */
extern unsigned damage_done;
/* Cas trvania vsetkych poziarov */
extern double fire_alive;
/* Pocet poziarov */
extern unsigned fire_count;
/* Poziare ktore boli uhasene predtym nez dosli vsetky auta */
extern unsigned not_all_engines;

/* Poziar */
class FireAlarm;
class Fire : public Process 
{
	public:
		unsigned intensity;
		unsigned exception;
		// Zaciatok poziaru
		double beginning;
		// Pozicia poziaru
		unsigned position[2];
		// Sila poziaru
		double strength;
		double max_strength;
		// Znizenie intenzity, ak potrebny pocet vozidiel dojde
		double strength_dec;
		// Zvysovanie sili poziaru
		double strength_inc;
		// Napachane skody
		unsigned damage;
		// Zvysovanie sko
		unsigned damage_inc;
		// Maximalne trvanie
		unsigned max_duration;
		// Dostupne auta
		double phase_start;
		FireAlarm* alarm;
		FireEngine* engines[3] = {NULL,NULL,NULL};

		Fire();
		void Behavior();
		unsigned engines_on_site();
		double current_strenght();
		unsigned get_damage();
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