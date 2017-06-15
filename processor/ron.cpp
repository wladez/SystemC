#include "ron.h"

//void  ron::read0 () {
//	if (cs0.read() && oe0.read() &&  ! we0.read()) {
//		data_out0 = mem[(int)address0.read()];
//	}
//}
//
//void  ron::read1 () {
//	if (cs1.read() && oe1.read() &&  ! we1.read()) {
//		data_out1 = mem[address1.read()];
//	}
//}

void  ron::read0 () {
	data_out0 = 0;
	wait();
	while(true){
		if (oe0.read() &&  ! we0.read()) {
			data_out0 = mem[(int)address0.read()];
		}
		//else data_out0 = 0;
		wait();
	}
}

void  ron::read1 () {
	data_out1 = 0;
	wait();
	while(true){
		if (oe1.read() &&  ! we1.read()) {
			data_out1 = mem[address1.read()];
		}
		else 
			data_out1 = 0;
                wait();
	}
}

void ron::write(){
	for(int i = 0; i< RON_DEPTH; i++)
		mem[i] = 0;
	wait();
	while(true){
		if (we0.read()) {
			mem[address0.read()] = data_in0.read();
		}
		else if (we1.read()) {
			mem[address1.read()] = data_in1.read();
		}
		wait();
	}
}

//void ron::write0 (){
//	if (cs0.read() && we0.read()) {
//		mem[address0.read()] = data_in0.read();
//	}
//}
//
//void  ron::write1 () {
//	if (cs1.read() && we1.read()) {
//		mem[address1.read()] = data_in1.read();
//	} 
//}