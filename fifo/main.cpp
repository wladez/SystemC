#include "systemc.h"
#include "fifo.h"
#include "fifoparam.h"

#define soft_assert(signal, expected) \
      if (signal.read() != expected) { \
          cerr << "@" << sc_time_stamp() << " Check failed. Expected: " << expected << ". Actual: " << signal.read() << ".\n" << endl; \
      }

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clock", 4, SC_NS);
    sc_signal<bool> sreset_n;
    //sc_signal<sc_uint<8> > data_in;
	sc_signal<char> data_in;
	sc_signal<bool> push;
	sc_signal<bool> pop;
    //sc_signal<sc_uint<8> > data_out;
	sc_signal<char> data_out;
	sc_signal<bool> empty;
	sc_signal<bool> full;

	//fifo test("test");
	fifoparam<char> test("test");
	test.clk(clk);
	test.sreset_n(sreset_n);
	test.data_in(data_in);
	test.push(push);
	test.pop(pop);
	test.data_out(data_out);
	test.empty(empty);
	test.full(full);

	// Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("fifo_waveform");
    // Dump the desired signals
    sc_trace(wf, clk, "clock");
    sc_trace(wf, sreset_n, "reset");
    sc_trace(wf, data_in, "in");
	sc_trace(wf, push, "push");
	sc_trace(wf, pop, "pop");
    sc_trace(wf, data_out, "out");
	sc_trace(wf, empty, "empty");
	sc_trace(wf, full, "full");
	sc_trace(wf, test.num_elements, "elements");

	sreset_n = 1; // Assert the reset
    cout << "@" << sc_time_stamp() << " Asserting reset\n" << endl;
	sc_start(6, SC_NS);
    
	sreset_n = 0; // De-assert the reset
    cout << "@" << sc_time_stamp() << " De-Asserting reset\n" << endl;
    data_in = 5;
	push = true;
    sc_start(21, SC_NS);

	data_in = 7;
	pop = true;
	sc_start(33, SC_NS);
	return 0;
}