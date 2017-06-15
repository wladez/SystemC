#include "systemc.h"
#include "constants.h"

#ifndef RON_H
#define RON_H

SC_MODULE (ron) {
	sc_in_clk clock;
	sc_in<bool> reset;
	sc_in<sc_uint <RON_ADDR_WIDTH> > address0;
	sc_in<bool> we0 ;
	sc_in<bool> oe0  ;
	sc_in    <sc_uint <RON_ADDR_WIDTH> > address1;
	sc_in    <bool> we1 ;
	sc_in    <bool> oe1  ;
	sc_in	 <sc_uint <DATA_WIDTH> > data_in0;
	sc_out	 <sc_uint <DATA_WIDTH> > data_out0;
	sc_in	 <sc_uint <DATA_WIDTH> > data_in1;
	sc_out	 <sc_uint <DATA_WIDTH> > data_out1;

	//-----------Internal variables-------------------
	sc_uint <DATA_WIDTH> mem [RON_DEPTH];

	//-----------Methods------------------------------
	void  write ();
	void  read0 ();
	void  read1 ();
	//void  write0 ();
	//void  write1 ();

	//-----------Constructor--------------------------
	SC_CTOR(ron) {
		SC_CTHREAD(write, clock.pos());
		async_reset_signal_is(reset, true);
		SC_CTHREAD(read0, clock.pos());
		async_reset_signal_is(reset, true);
		SC_CTHREAD(read1, clock.pos());
		async_reset_signal_is(reset, true);
		/*SC_METHOD(write0);
		sensitive << clock.pos() << cs0 << oe0 << we0;
		SC_METHOD(write1);
		sensitive << clock.pos() << cs1 << oe1 << we1;*/
	}
};
#endif /* RON_H */