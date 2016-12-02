#ifndef FIRE_ENGINE_H
#define FIRE_ENGINE_H

/* Poziarnicke auto a stanica */

// Stavy poziarneho auta
enum eng_state { HOME,TO_FIRE,FIRE,FROM_FIRE,RE_FILLING };

// Trieda poziarneho auta
class FireEngine {
	public:
		unsigned home[2];
		eng_state state;

		FireEngine(unsigned x, unsigned y);
		unsigned travel_time(unsigned x, unsigned y);
};

extern Store fire_stations;
extern FireEngine* fire_engines;

#endif