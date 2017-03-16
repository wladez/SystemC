#include <systemc.h>

#ifndef FIFOPARAM_H
#define	FIFOPARAM_H

template <typename T>
SC_MODULE(fifoparam){
public:	
	sc_in_clk clk;
	sc_in<bool> sreset_n;
	sc_in<T> data_in;
	sc_in<bool> push;
	sc_in<bool> pop;
	sc_out<T> data_out;
	sc_out<bool> empty;
	sc_out<bool> full;

	enum e {max = 10};
	T data[max];
	int num_elements, first;

	void processing();
	void read();
	void write();
	//fifoparam(sc_module_name _name);

	SC_CTOR(fifoparam):
	//fifoparam(sc_module_name _name):
		clk("clk"),
		sreset_n("reset"),
		data_in("data_in"),
		push("push"),
		pop("pop"),
		data_out("data_out"),
		empty("empty"),
		full("full")
	{
		cout << "Executing new" << endl;
        SC_CTHREAD(processing, clk.pos());
		reset_signal_is(sreset_n,true);
	}

};
#endif /*FIFO_H*/