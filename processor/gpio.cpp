#include "gpio.h"

void gpio::processing()
{
	outp = 0;
	internal_out = 0;
	wait();
	while(true){
		if(read)
			internal_out = inp.read();
		if(write)
			outp = internal_in.read();
		wait();
	}
}