#include "systemc.h"
#include "constants.h"
#include <fstream>

#ifndef MEM_H
#define MEM_H

SC_MODULE (memory) {
	sc_in_clk clock;
	sc_in	 <bool>					reset;
	sc_in    <sc_uint<ADDR_WIDTH> > address ;
	sc_in    <bool>                 we      ;
	sc_in    <bool>                 oe      ;
	sc_in    <sc_uint<DATA_WIDTH> > data_in ;
	sc_out   <sc_uint<DATA_WIDTH> > data_out;
	sc_out	 <bool>					ack;

	//-----------Internal variables-------------------
	sc_uint <DATA_WIDTH> mem [RAM_DEPTH];
	bool ack_register;

	// Memory Write Block 
	// Write Operation : When we = 1, cs = 1
	void write_mem () 
	{
		for(int i=0; i < RAM_DEPTH; i++)
			mem[i] = 0;
		mem[0] = 0xA0000005;
		mem[1] = 0x9C000000;
		//mem[0] = 0x8400000A;		
		//mem[1] = 0x84200005;
		//mem[2] = 0x18000000;
		////mem[2] = 0x84000FAF;
		////mem[3] = 0x98000020;
		//mem[3] = 0x08000400;
		//mem[4] = 0x50000008;
		////mem[4] = 0x9C000001;
		//mem[5] = 0x04000401;
		//mem[6] = 0x04000000;
		//mem[7] = 0x8C000000;
		//mem[8] = 0x44000000;
		std::string tmp;
		//fstream f1("commands.txt", std::fstream::in);
		//fstream f1("2a.txt", std::fstream::in);
		fstream f1("3a.txt", std::fstream::in);
		int j = 0;
		while(getline(f1, tmp))
		{
			mem[j] = strtoul(tmp.c_str(), NULL, 16);
			//std::cout << j << " command = " << mem[j] <<endl;
			j++;
		}
		f1.close();
		
		//!!!!!!!!!!!!!!!! сортировка массива
		//fstream f2("numbers.txt", std::fstream::in);
		//j = 32;
		//while(getline(f2, tmp))
		//{
		//	mem[j] = atoi(tmp.c_str());
		//	//std::cout << j << " number = " << mem[j] <<endl;
		//	j++;
		//}
		//f2.close();

		//!!!!!!!!!!!!!!!!!!! умножение матриц
		//ifstream in("matrix.txt");
		//int mat = 0;
		//for (int i = offsetA; i < offsetA + n*m; i++)
		//	in >> mem[i];
		////cout << "matrix A:" << endl;
		////for (int i = offsetA; i < offsetA+n; i++)
		////{
		////	for (int j = 0; j < m; j++)
		////	{	
		////		mat = mem[(i-offsetA)*m+j+offsetA];
		////		cout << mat << "\t";
		////	}
		////	cout << "\n";
		////}
		////cout<<endl;
		//for (int i = offsetB; i < offsetB + m*p; i++)
		//	in >> mem[i];
		////cout << "matrix B:" << endl;
		////for (int i = offsetB; i < offsetB+m; i++)
		////{
		////	for (int j = 0; j < p; j++)
		////	{
		////		mat = mem[(i-offsetB)*p+j+offsetB];
		////		cout << mat << "\t";
		////	}
		////	cout << "\n";
		////}
		////cout<<endl;
		//in.close();
		
		//!!!!!!!!!!!!!!!!! медианный фильтр
		ifstream in("med.txt");
		for (int i = offsetIn; i < offsetOut; i++)
			in >> mem[i];
		in.close();
		cout << "matrix:" << endl;
		for (int i = offsetIn; i < offsetIn+width; i++)
		{
			for (int j = 0; j < height; j++)
				cout << mem[(i-offsetIn)*height+j+offsetIn] << "\t";
			cout << "\n";
		}
		cout<<endl;

		wait();
		while(true)
		{
			if (we.read())
			{
				mem[(int)address.read()] = data_in.read();
			}
			wait();
		}
	}

	// Memory Read Block 
	// Read Operation : When we = 0, oe = 1, cs = 1
	void read_mem () 
	{
		data_out = 0;
		wait();
		while(true)
		{
			if ( ! we.read() && oe.read())
			{
				data_out.write(mem[(int)address.read()]);
			}
			//else data_out = 0;
			wait();
		}
	}

	void confirm()
	{
		ack_register = false;
		ack.write(ack_register);
		wait();
		while(true)
		{
			//if(stb.pos())
			ack_register = false;
			ack.write(ack_register);
			if (we.read())
			{
				ack_register = true;
				ack.write(ack_register);
			}
			else if (! we.read() && oe.read())
			{
				ack_register = true;
				ack.write(ack_register);
			}
			else
			{
				ack_register = false;
				ack.write(ack_register);
			}
			wait();
		}
	}

	SC_CTOR(memory) 
	{
		SC_CTHREAD(read_mem, clock.pos());
		async_reset_signal_is(reset, true);
		SC_CTHREAD(write_mem, clock.pos());
		async_reset_signal_is(reset, true);
		SC_CTHREAD(confirm, clock.pos());
		async_reset_signal_is(reset, true);
	}

};
#endif /* MEM_H */