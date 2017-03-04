//-----------------------------------------------------
// Testbench for the register ---------------->
//-----------------------------------------------------
#include "systemc.h"
#include "register.h"
#include "counter.h"
#include "shift.h"

#define soft_assert(signal, expected) \
      if (signal.read() != expected) { \
          cerr << "@" << sc_time_stamp() << " Check failed. Expected: " << expected << ". Actual: " << signal.read() << ".\n" << endl; \
      }

//int sc_main(int argc, char* argv[]) {
//    sc_clock clock("clock", 4, SC_NS);
//    sc_signal<bool> reset;
//    sc_signal<sc_uint<8> > register_in;
//    sc_signal<sc_uint<8> > register_out;
//    int i = 0;
//    // Connect the DUT
//    eightbit_register test_register("test_register");
//    test_register.clock(clock);
//    test_register.reset(reset);
//    test_register.register_in(register_in);
//    test_register.register_out(register_out);
//
//    // Open VCD file
//    sc_trace_file *wf = sc_create_vcd_trace_file("register_waveform");
//    // Dump the desired signals
//    sc_trace(wf, clock, "clock");
//    sc_trace(wf, reset, "reset");
//    sc_trace(wf, register_in, "din");
//    sc_trace(wf, register_out, "dout");
//
//    reset = 1; // Assert the reset
//    cout << "@" << sc_time_stamp() << " Asserting reset\n" << endl;
//
//    sc_start(6, SC_NS);
//
//    reset = 0; // De-assert the reset
//    cout << "@" << sc_time_stamp() << " De-Asserting reset\n" << endl;
//
//    for (i = 0; i < 5; i++) {
//        register_in = i * 5;
//        sc_start(4, SC_NS);
//        assert(register_out.read() == i * 5);
////        soft_assert(register_out, i * 5);
//    }
//
//    reset = 1; // Assert the reset
//    cout << "@" << sc_time_stamp() << " Asserting reset\n" << endl;
//    sc_start(1, SC_NS);
//    assert(register_out.read() == 0);
//    sc_start(5, SC_NS);
//
//    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
//    sc_close_vcd_trace_file(wf);
//    return 0; // Terminate simulation
//
//}


//int sc_main(int argc, char* argv[]) {
//    sc_clock clock("clock", 4, SC_NS);
//    sc_signal<bool> reset;
//    sc_signal<bool> sreset;
//    sc_signal<bool> ena;
//    sc_signal<sc_uint<8> > counter_out;
//    int i = 0;
//    
//    counter test_count("test_count");
//    test_count.clock(clock);
//    test_count.reset(reset);
//    test_count.sreset(sreset);
//    test_count.ena(ena);
//    test_count.counter_out(counter_out);
//    
//    // Open VCD file
//    sc_trace_file *wf = sc_create_vcd_trace_file("counter");
//    // Dump the desired signals
//    sc_trace(wf, clock, "clock");
//    sc_trace(wf, reset, "reset");
//    sc_trace(wf, sreset, "sreset");
//    sc_trace(wf, ena, "ena");
//    sc_trace(wf, counter_out, "out");
//    
//    sreset = 1;
//    ena = 1;
//    sc_start(6, SC_NS);
//    
//    sreset = 0;
//    sc_start(18, SC_NS);
//    
//    ena = 0;
//    sc_start(6, SC_NS);
//    
//    sreset = 1;
//    sc_start(6, SC_NS);   
//    return 0;
//}

int sc_main(int argc, char* argv[]) {
    sc_clock clock("clock", 4, SC_NS);
    sc_signal<bool> reset;
    sc_signal<bool> sreset;
    sc_signal<bool> load;
    sc_signal<sc_uint<8> > data_in;
    sc_signal<sc_uint<8> > register_out;
    sc_signal<bool> c_out;
    sc_signal<bool> c_in;
    
    shift_register test_shift("test_shift");
    test_shift.clock(clock);
    test_shift.reset(reset);
    test_shift.sreset(sreset);
    test_shift.load(load);
    test_shift.data_in(data_in);
    test_shift.register_out(register_out);
    test_shift.c_out(c_out);
    test_shift.c_in(c_in);
    
    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("shift");
    // Dump the desired signals
    sc_trace(wf, clock, "clock");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, sreset, "sreset");
    sc_trace(wf, load, "load");
    sc_trace(wf, data_in, "din");
    sc_trace(wf, register_out, "out");
    sc_trace(wf, c_out, "c_out");
    sc_trace(wf, c_in, "c_in");
    
    reset = 0;
    sreset = 1;
    load = 0;
    c_in = 0;
    sc_start(6,SC_NS);
    
    sreset = 0;
    load = 1;
    data_in = 77;
    sc_start(12, SC_NS);
    
    load = 0;
    sc_start(10, SC_NS);
    //assert();
    
    c_in = 1;
    sc_start(20, SC_NS);
    
    sreset = 1;
    sc_start(10, SC_NS);
    sc_close_vcd_trace_file(wf);
    return 0;
}

