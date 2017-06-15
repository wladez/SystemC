#include "systemc.h"
#include "constants.h"
#include "ron.h"
#include "mem.h"
#include "alu.h"
#include "memory_controller.h"
#include "decoder.h"
#include "program_counter.h"
#include "gpio.h"

#define soft_assert(signal, expected) \
      if (signal.read() != expected) { \
          cerr << "@" << sc_time_stamp() << " Check failed. Expected: " << expected << ". Actual: " << signal.read() << ".\n" << endl; \
      }

int sc_main(int argc, char* argv[]) {
	sc_clock clock("clock", 4, SC_NS);
	sc_signal<bool> reset, z_flag, c_flag, ready;
	sc_signal<sc_uint <4> > alu_command;
	sc_signal<sc_uint <DATA_WIDTH> > a,b;
	sc_signal<sc_uint <DATA_WIDTH> > result;
	sc_signal<sc_uint <3> > inFlags;
	//ron
	sc_signal<bool> oe0, oe1, we0, we1;
	sc_signal<sc_uint <RON_ADDR_WIDTH> > ron_address0, ron_address1;
	sc_signal<sc_uint <DATA_WIDTH> > ron_in0, ron_in1, ron_out0, ron_out1;
	//decoder
	sc_signal<bool> instruction_end, mem_write, mem_read;
	sc_signal<sc_uint <DATA_WIDTH> > mem_in, mem_out, tmp0, tmp1;
	sc_signal<sc_uint <ADDR_WIDTH> > mem_addr;
	//mem_contr
	sc_signal<bool> we, oe, mem_ready;
	sc_signal<sc_uint <ADDR_WIDTH> > address;
	//memory
	sc_signal<bool> ack;
	sc_signal<sc_uint <DATA_WIDTH> > mem_to_contr, contr_to_mem;
	//counter
	sc_signal<bool> load, enable;
	sc_signal<sc_uint <ADDR_WIDTH> > count_in, count_out;
	//gpio
	sc_signal<bool> io_write, io_read;
	sc_signal<sc_uint <GPIO_WIDTH> > io_in, io_out, internal_in, internal_out;

	alu alu1("alu");
	alu1.clock(clock);
	alu1.reset(reset);
	alu1.command(alu_command);
	alu1.a(ron_out0);
	alu1.b(ron_out1);
	alu1.inFlags(inFlags);
	alu1.result(result);
	alu1.z_flag(z_flag);
	alu1.ready(ready);
	alu1.c_flag(c_flag);

	ron ron1("ron");
	ron1.clock(clock);
	ron1.reset(reset);
	ron1.oe0(oe0);
	ron1.oe1(oe1);
	ron1.we0(we0);
	ron1.we1(we1);
	ron1.address0(ron_address0);
	ron1.address1(ron_address1);
	ron1.data_in0(ron_in0);
	ron1.data_in1(ron_in1);
	ron1.data_out0(ron_out0);
	ron1.data_out1(ron_out1);

	decoder dec("decoder");
	dec.clock(clock);
	dec.reset(reset);
	dec.alu_ready(ready);
	dec.alu_command(alu_command);
	dec.alu_res(result);
	dec.ron_inp0(ron_out0);
	dec.ron_inp1(ron_out1);
	dec.mem_in(mem_in);
	dec.memory_address(mem_addr);
	dec.mem_write(mem_write);
	dec.mem_read(mem_read);
	dec.mem_out(mem_out);
	dec.ron_address0(ron_address0);
	dec.ron_address1(ron_address1);
	dec.ron_we0(we0);
	dec.ron_we1(we1);
	dec.ron_re0(oe0);
	dec.ron_re1(oe1);
	dec.ron_data0(ron_in0);
	dec.ron_data1(ron_in1);
	dec.instruction_end(instruction_end);
	dec.command_addr(count_out);
	dec.count_ena(enable);
	dec.count_load(load);
	dec.ack(mem_ready);
	dec.z_flag(z_flag);
	dec.c_flag(c_flag);
	dec.io_write(io_write);
	dec.io_read(io_read);
	dec.io_in(internal_out);
	dec.io_out(internal_in);

	memory mem("mem");
	mem.clock(clock);
	mem.reset(reset);
	mem.we(we);
	mem.oe(oe);
	mem.address(address);
	mem.data_in(contr_to_mem);
	mem.data_out(mem_to_contr);
	mem.ack(ack);

	memory_controller controller("controller");
	controller.clock(clock);
	controller.reset(reset);
	controller.wr(mem_write);
	controller.rd(mem_read);
	controller.ack(ack);
	controller.address_in(mem_addr);
	controller.data_in(mem_out);
	controller.mem_in(mem_to_contr);
	controller.address_out(address);
	controller.data_out(mem_in);
	controller.mem_out(contr_to_mem);
	controller.write_ena(we);
	controller.read_ena(oe);
	controller.ready(mem_ready);

	program_counter counter("counter");
	counter.clock(clock);
	counter.reset(reset);
	counter.load(load);
	counter.enable(enable);
	counter.data_in(mem_addr);
	counter.out(count_out);

	gpio io("io");
	io.clock(clock);
	io.reset(reset);
	io.write(io_write);
	io.read(io_read);
	io.outp(io_out);
	io.inp(io_in);
	io.internal_in(internal_in);
	io.internal_out(internal_out);

	// Open VCD file
	sc_trace_file *wf = sc_create_vcd_trace_file("decoder_w");
	// Dump the desired signals
	sc_trace(wf, clock, "clock");
	sc_trace(wf, reset, "reset");
	sc_trace(wf, alu_command, "alu_command");
	sc_trace(wf, ron_out0, "a");
	sc_trace(wf, ron_out1, "b");
	sc_trace(wf, result, "alu_result");
	sc_trace(wf, mem_addr, "mem_addr");
	sc_trace(wf, mem_ready, "mem_ready");
	sc_trace(wf, dec.instruction, "instruction");
	sc_trace(wf, ron_address0, "ron_address0");
	sc_trace(wf, ron_address1, "ron_address1");
	sc_trace(wf, oe0, "oe0");
	sc_trace(wf, we0, "we0");
	sc_trace(wf, oe1, "oe1");
	sc_trace(wf, we1, "we1");
	sc_trace(wf, ron_in0, "ron_in0");
	sc_trace(wf, ron_in1, "ron_in1");
	sc_trace(wf, dec.state, "state");
	sc_trace(wf, ron1.mem[0], "ron0");
	sc_trace(wf, ron1.mem[1], "ron1");
	sc_trace(wf, ron1.mem[2], "ron2");
	sc_trace(wf, ron1.mem[3], "ron3");
	sc_trace(wf, ron1.mem[10], "ron10");
	sc_trace(wf, ron1.mem[12], "ronIndIn");
	sc_trace(wf, ron1.mem[15], "ronIndC");
	sc_trace(wf, ron1.mem[24], "ronEdgeY");
	sc_trace(wf, ron1.mem[27], "ronX");
	sc_trace(wf, ron1.mem[28], "ronY");
	sc_trace(wf, ron1.mem[29], "ronFX");
	sc_trace(wf, ron1.mem[30], "ronFY");
	sc_trace(wf, count_out, "count");
	sc_trace(wf, mem_in, "from_mem");
	sc_trace(wf, we, "mem_we");
	sc_trace(wf, oe, "mem_re");
	sc_trace(wf, mem.mem[0], "mem0");
	sc_trace(wf, contr_to_mem, "mem_in");
	sc_trace(wf, dec.command, "command");
	sc_trace(wf, mem_read, "mem_read");
	sc_trace(wf, mem_write, "mem_write");
	sc_trace(wf, z_flag, "z_flag");
	sc_trace(wf, io_out, "io_out");
	sc_trace(wf, io_write, "io_write");
	sc_trace(wf, internal_in, "dec_to_io");
	sc_trace(wf, internal_out, "io_to_dec");
	sc_trace(wf, c_flag, "c_flag");

	reset = 1; // Assert the reset
	cout << "@" << sc_time_stamp() << " Asserting reset\n" << endl;
	sc_start(8, SC_NS);

	io_in = 11;

	reset = 0; // De-assert the reset
	cout << "@" << sc_time_stamp() << " De-Asserting reset\n" << endl;
	sc_start(100, SC_NS);
	//command = 0x8400000F;
	sc_start(190000, SC_NS);

	//!!!!!!!!!!!!! сортировка массива
	//for(int i = 32; i<48; i++)
	//{
	//	std::cout << i-32 << " number = " << mem.mem[i] <<endl;
	//}

	//!!!!!!!!!!!!! умножение матриц
	//cout << "Result matrix:" << endl;
	//int digit = 0;
	//for (int i = offsetQ; i < offsetQ+n; i++)
	//{
	//	for (int j = 0; j < p; j++)
	//	{
	//		digit = mem.mem[(i-offsetQ)*p+j+offsetQ];
	//		cout << digit << "\t";
	//	}
	//	cout << "\n";
	//}
	//cout<<endl;

	//!!!!!!!!!!!!!!! медианный фильтр
	cout << "Result matrix:" << endl;
	int digit = 0;
	for (int i = offsetOut; i < offsetOut+width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			digit = mem.mem[(i-offsetOut)*height+j+offsetOut];
			cout << digit << "\t";
		}
		cout << "\n";
	}
	cout<<endl;

	//command = 0x4C00002F;
	//sc_start(12, SC_NS);

	//command = 0x18000000;
	//sc_start(20, SC_NS);

	//command = 0x88000001;
	//sc_start(16, SC_NS);

	//command = 0x04000000;
	//sc_start(20, SC_NS);

	////command = 0x4400002F;
	////sc_start(12, SC_NS);

	//command = 0x84000010;
	//sc_start(10, SC_NS);

	//command = 0x50000000;
	//sc_start(12, SC_NS);

	//command = 0x90000001;
	//sc_start(24, SC_NS);

	//command = 0x8C000000;
	//sc_start(35, SC_NS);

	sc_close_vcd_trace_file(wf);

	return 0;
}