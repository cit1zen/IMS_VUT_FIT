#include "simlib.h"

// Trvanie simulacie v sim. case
#define MINUTES 60

// Pravdepodobnost poziaru
#define FIRE_X 5


// Poziarne auto
class FireEngine {
	public:
		unsigned home[2];
} engine;


// Poziar
class Fire : public Process 
{
	unsigned intensity;
	double strength;

	void Behavior() 
	{
		
	}

	void SetFireType()
	{
		intensity = Uniform(1, 4)
	}
};


// Generator poziarov
class Generator : public Event {
	void Behavior() 
	{
		(new Fire)->Activate();
		Activate(Time+Exponential(FIRE_X));
	}
};

int main() 
{
	SetOutput("multiexp.dat");
	Print("# MULTIEXP - multiple experiments example\n");
	Print("# Experiment (čas=%g) \n", MINUTES);

	Init(0, MINUTES);
	(new Generator())->Activate();
	Run();

	return 0;
}