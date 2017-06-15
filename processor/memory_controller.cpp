#include "memory_controller.h"

void memory_controller::handler()
{
	write_ena = false;
	read_ena = false;
	ready = false;
	address_out = 0;
	data_out = 0;
	mem_out = 0;
	wait();
	while(true){
		ready = ack.read();
		address_out = address_in.read();
		if(wr){
			write();
			if(ack)
				write_ena = false;
		}
		else if(rd){
			read();
			if(ack)
				read_ena = false;
		}
		wait();
	}
}

void memory_controller::write()
{
	write_ena = true;
	read_ena = false;
	mem_out = data_in.read();
}

void memory_controller::read()
{
	read_ena = true;
	write_ena = false;
	data_out = mem_in.read();
}