#include "systemc.h"

#ifndef FIFO_H
#define	FIFO_H
SC_MODULE(fifo){
	sc_in_clk clk;
	sc_in<bool> sreset_n;
	sc_in<sc_uint<8> > data_in;
	sc_in<bool> push;
	sc_in<bool> pop;
	sc_out<sc_uint<8> > data_out;
	sc_out<bool> empty;
	sc_out<bool> full;

	sc_signal<bool> is_read;
	sc_signal<bool> is_write;

	//enum e {max = 10};
	int max;
	unsigned int *data;
	int num_elements, first;

	void processing();
	void read();
	void write();
	void flags();

	SC_HAS_PROCESS(fifo);

	//SC_CTOR(fifo):
	fifo(sc_module_name name, int _size) :
	clk("clk"),
		sreset_n("reset"),
		data_in("data_in"),
		push("push"),
		pop("pop"),
		data_out("data_out"),
		empty("empty"),
		full("full"),
		max(_size)
	{
		data = new unsigned int [max];            
		cout << "Executing new" << endl;
		SC_CTHREAD(processing, clk.pos());
		reset_signal_is(sreset_n,true);
		SC_METHOD(flags);
		sensitive << is_read << is_write << clk;
	}

};
#endif /*FIFO_H*/