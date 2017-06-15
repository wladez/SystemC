#include "systemc.h"
#include "constants.h"

#ifndef GPIO_H
#define GPIO_H

SC_MODULE(gpio){
	sc_in_clk clock;
	sc_in<bool> reset, write, read;
	sc_in<sc_uint <GPIO_WIDTH> > inp, internal_in;
	sc_out<sc_uint <GPIO_WIDTH> > outp, internal_out;

	void processing();

	SC_CTOR(gpio){
		SC_CTHREAD(processing, clock.pos());
		async_reset_signal_is(reset, true);
	}

};

#endif /* GPIO_H */