#include "Utility.h"



Utility::Utility()
{
}


Utility::~Utility()
{
}

int Utility::GetRand(int min, int max)
{
	return (min + rand() % (max - min + 1));
}
