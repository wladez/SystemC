#include "systemc.h"
#include "constants.h"

#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H

SC_MODULE(memory_controller){
	sc_in_clk clock;
	sc_in<bool> reset, wr, rd, ack;
	sc_in<sc_uint <ADDR_WIDTH> > address_in;
	sc_in<sc_uint <DATA_WIDTH> > data_in, mem_in;
	sc_out<sc_uint <ADDR_WIDTH> > address_out;
	sc_out<sc_uint <DATA_WIDTH> > data_out, mem_out;
	sc_out<bool> write_ena, read_ena, ready;

	void handler();
	void read();
	void write();

	SC_CTOR(memory_controller){
		SC_CTHREAD(handler, clock.pos());
		async_reset_signal_is(reset, true);
	}

};

#endif /* MEMORY_CONTROLLER_H */