#include "systemc.h"
#include "constants.h"

#ifndef DMA_H
#define DMA_H

SC_MODULE(dma){
	sc_in_clk clock;
	sc_in<bool> reset, cs0, cs1, operation0, operation1, ready;
	sc_in<sc_uint <ADDR_WIDTH> > addr0, addr1;
	sc_in<sc_uint <DATA_WIDTH> > data_in0, data_in1, mem_in;
	sc_out<sc_uint <DATA_WIDTH> > out0, out1, mem_out;
	sc_out<sc_uint <ADDR_WIDTH> > addr_out;
	sc_out<bool> write, read, ack0, ack1;

	sc_signal<sc_uint <4> > state;

	void dma_states();
	void dma_out();

	enum dmaState{
		IDLE = 0,
		DEVICE0 = 1,
		WRITE_MEM0 = 2,
		END_MEM0 = 3,
		READ_MEM0 = 4,
		GET_RESULT0 = 5,
		DEVICE1 = 6,
		WRITE_MEM1 = 7,
		END_MEM1 = 8,
		READ_MEM1 = 9,
		GET_RESULT1 = 10
	};

	SC_CTOR(dma)
	{
		SC_CTHREAD(dma_states, clock.pos());
		async_reset_signal_is(reset, true);
		SC_METHOD(dma_out);
		sensitive << state;
	}
};

#endif /* DMA_H */