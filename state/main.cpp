/* 
 * File:   main.cpp
 * Author: student
 *
 * Created on March 3, 2017, 11:56 AM
 */
//-----------------------------------------------------
// Testbench for the state machine ---------------->
//-----------------------------------------------------
#include "systemc.h"
#include "state.h"

#define soft_assert(signal, expected) \
      if (signal.read() != expected) { \
          cerr << "@" << sc_time_stamp() << " Check failed. Expected: " << expected << ". Actual: " << signal.read() << ".\n" << endl; \
      }

int sc_main(int argc, char* argv[]) {
    sc_clock clock("clock", 4, SC_NS);
    sc_signal<bool> reset;
    sc_signal<sc_uint<2> > in;
    sc_signal<sc_uint<2> > out;
    
    state_machine test("test");
    test.clock(clock);
    test.reset(reset);
    test.in(in);
    test.out(out);
    
    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("state_waveform");
    // Dump the desired signals
    sc_trace(wf, clock, "clock");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, in, "in");
    sc_trace(wf, out, "out");
    sc_trace(wf, test.state, "state");
    
    reset = 1; // Assert the reset
    cout << "@" << sc_time_stamp() << " Asserting reset\n" << endl;

    sc_start(6, SC_NS);
    assert(out.read() == 3);

    reset = 0; // De-assert the reset
    cout << "@" << sc_time_stamp() << " De-Asserting reset\n" << endl;
    in = 0;
    sc_start(17, SC_NS);
    assert(out.read() == 2);
    
    in = 1;
    sc_start(6, SC_NS);
    assert(out.read() == 0);
    
    in = 3;
    sc_start(14, SC_NS);
	assert(out.read() == 3);

    in = 0;
    sc_start(5, SC_NS);
	assert(out.read() == 1);

    in = 1;
    sc_start(12, SC_NS);
	assert(out.read() == 3);
    return 0;
}

