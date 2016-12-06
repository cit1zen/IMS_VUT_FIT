#include <iostream>
#include "simlib.h"
#include <stdlib.h> 
#include <math.h>
#include <vector>
#include "params.h"
#include "fire_engine.h"

// Konstruktor
FireEngine::FireEngine(unsigned x, unsigned y)
{
	this->home[0] = x;
	this->home[1] = y;
	this->state = HOME;
	this->alarm = NULL;
}

// Vypocitaj cas nutny na cestu medzi poziarom a stanicou
double FireEngine::travel_time(unsigned x, unsigned y)
{
	return sqrt(pow(abs((double)this->home[0]-x),2)+pow(abs((double)this->home[1]-y),2))/FIRE_ENG_SPPEED;
}