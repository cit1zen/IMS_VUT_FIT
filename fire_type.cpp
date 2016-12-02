#include "fire_type.h"

/* Tabulka obsahujuca druhy poziarov */
FireType dummy1(1, 20, 100, 60, 10, 20);
FireType dummy2(1, 20, 100, 60, 10, 20);
FireType dummy3(1, 20, 100, 60, 10, 20);
FireType intesity_1[] = 
{
	dummy1
	// Odpadkovy kos
	// Auto
	// Travnik
};
FireType intesity_2[] = 
{
	dummy2
	// Auto
	// Nizka budova
	// Vyskova budova
	// Odpad
};
FireType intesity_3[] = 
{
	dummy3
	// Nizka budova
	// Vyskova budova
	// Priemyselna budova
};
FireType* types[] =  {intesity_1, intesity_2, intesity_3};