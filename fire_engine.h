#ifndef FIRE_ENGINE_H
#define FIRE_ENGINE_H

// Stavy poziarneho auta
enum eng_state { HOME,TO_FIRE,ON_SITE,FROM_FIRE,RE_FILLING };

// Trieda poziarneho auta
class FireEngine {
	public:
		unsigned home[2];
		eng_state state;
		Event* alarm;

		FireEngine(unsigned x, unsigned y);
		unsigned travel_time(unsigned x, unsigned y);
};

/* Poziarne stanice a auta */
extern Store* fire_stations;
extern std::vector<FireEngine> fire_engines;
extern unsigned fire_stations_count;

#endif