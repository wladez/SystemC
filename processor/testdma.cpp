#include "systemc.h"
#include "dma.h"
#include "mem.h"

#define soft_assert(signal, expected) \
      if (signal.read() != expected) { \
          cerr << "@" << sc_time_stamp() << " Check failed. Expected: " << expected << ". Actual: " << signal.read() << ".\n" << endl; \
      }

int sc_main(int argc, char* argv[]) {
	sc_clock clock("clock", 4, SC_NS);
	sc_signal<bool> reset;

	//dma
	sc_signal<bool> cs0, cs1, operation0, operation1, ready, write, read, ack0, ack1;
	sc_signal<sc_uint <ADDR_WIDTH> > addr0, addr1, addr_out;
	sc_signal<sc_uint <DATA_WIDTH> > data_in0, data_in1, mem_in;
	sc_signal<sc_uint <DATA_WIDTH> > out0, out1, mem_out;
	
	dma dma1("dma");
	dma1.clock(clock);
	dma1.reset(reset);
	dma1.cs0(cs0);
	dma1.cs1(cs1);
	dma1.operation0(operation0);
	dma1.operation1(operation1);
	dma1.ready(ready);
	dma1.write(write);
	dma1.read(read);
	dma1.ack0(ack0);
	dma1.ack1(ack1);
	dma1.addr0(addr0);
	dma1.addr1(addr1);
	dma1.addr_out(addr_out);
	dma1.data_in0(data_in0);
	dma1.data_in1(data_in1);
	dma1.mem_in(mem_in);
	dma1.out0(out0);
	dma1.out1(out1);
	dma1.mem_out(mem_out);

	memory mem("mem");
	mem.clock(clock);
	mem.reset(reset);
	mem.ack(ready);
	mem.address(addr_out);
	mem.we(write);
	mem.oe(read);
	mem.data_in(mem_out);
	mem.data_out(mem_in);

	sc_trace_file *wf = sc_create_vcd_trace_file("dma_test");
	sc_trace(wf, clock, "clock");
	sc_trace(wf, reset, "reset");
	sc_trace(wf, cs0, "cs0");
	sc_trace(wf, cs1, "cs1");
	sc_trace(wf, operation0, "operation0");
	sc_trace(wf, operation1, "operation1");
	sc_trace(wf, ready, "ready");
	sc_trace(wf, write, "write_e");
	sc_trace(wf, read, "read_e");
	sc_trace(wf, ack0, "ack0");
	sc_trace(wf, ack1, "ack1");
	sc_trace(wf, addr0, "addr0");
	sc_trace(wf, addr1, "addr1");
	sc_trace(wf, data_in0, "in0");
	sc_trace(wf, data_in1, "in1");
	sc_trace(wf, addr_out, "addr_out");
	sc_trace(wf, out0, "out0");
	sc_trace(wf, out1, "out1");
	sc_trace(wf, mem_out, "dma_to_mem");
	sc_trace(wf, mem_in, "mem_to_dma");
	sc_trace(wf, dma1.state, "state");

	reset = 1; // Assert the reset
	cout << "@" << sc_time_stamp() << " Asserting reset\n" << endl;
	sc_start(8, SC_NS);

	reset = 0; // De-assert the reset
	cs0 = true;
	cs1 = false;
	operation0 = true;
	operation1 = false;
	data_in0 = 15;
	data_in1 = 0;
	addr0 = 10;
	addr1= 10;
	cout << "@" << sc_time_stamp() << " De-Asserting reset\n" << endl;

	sc_start(8, SC_NS);

	cs1 = true;
	sc_start(28, SC_NS);

	sc_close_vcd_trace_file(wf);

	return 0;
}