#include "systemc.h"
#include "constants.h"

#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

SC_MODULE(program_counter)
{
	sc_in_clk clock;
	sc_in<bool> reset, load, enable;
	sc_in<sc_uint <ADDR_WIDTH> > data_in;
	sc_out<sc_uint <ADDR_WIDTH> > out;

	sc_uint<ADDR_WIDTH> count;

	void counter()
	{
		count = 0;
		out.write(count);
		wait();
		while(true){
			if(enable.read()){
				if(load.read())
					count = data_in.read();
				else 
					count = count + 1;
				out.write(count);
			}
			wait();
		}
	}

	SC_CTOR(program_counter)
	{
		SC_CTHREAD(counter, clock.pos());
		async_reset_signal_is(reset, true);
	}
};

#endif /* PROGRAM_COUNTER_H */