#ifndef FIRE_TYPE_H
#define FIRE_TYPE_H

/* Druhy poziarov */

class FireType {
	public:
		unsigned intensity;
		unsigned start_strengt;
		unsigned max_strenght;
		unsigned max_duration;
		unsigned streng_increase;
		unsigned extinction_rate;
		unsigned start_damage;
		unsigned max_damage;
		unsigned damage_rate;
		
		FireType(unsigned a, unsigned b, unsigned c, 
				 unsigned d, unsigned e, unsigned f)
		{
			this->intensity = a;
			this->start_strengt = b;
			this->max_strenght = c;
			this->max_duration = d;
			this->streng_increase = e;
			this->extinction_rate = f;
		}	
};

extern FireType* types[];

#endif