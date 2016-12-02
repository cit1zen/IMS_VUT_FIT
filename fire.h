#ifndef FIRE_H
#define FIRE_H

/* Poziar */
class Fire : public Process 
{
	public:
		unsigned intensity;
		unsigned exception;
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
		// Maximalne skody
		unsigned max_damage;
		// Maximalne trvanie
		unsigned duration;

		void Behavior();
		void start_fire();
		double strength_dec_index(unsigned engines);
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
class FireEngineArrival : public FireAlarm
{
	public:
		Fire* obsluhovany_poziar;

		FireEngineArrival(Fire* poziar);
		void Behavior();
};

/* Prichod hasickeho auta na scenu */
class FireExtinguished : public FireAlarm
{
	public:
		Fire* obsluhovany_poziar;

		FireExtinguished(Fire* poziar);
		void Behavior();
};

#endif