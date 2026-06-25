#include <avr/io.h>
#include "avrlibdefs.h"


#include "ks0108.h"



int main()
{

	glcdInitHW();
	glcdCircle(50, 30, 20);


	while(1);
}
