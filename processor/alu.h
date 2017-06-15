#include "systemc.h"
#include "constants.h"

#ifndef ALU_H
#define	ALU_H

SC_MODULE(alu) {
	sc_in_clk clock; // Clock input of the design
	sc_in<bool> reset;
	sc_in<sc_uint<4> > command;
	sc_in<sc_uint<3> > inFlags;
	sc_in<sc_uint<DATA_WIDTH> > a;
	sc_in<sc_uint<DATA_WIDTH> > b;
	sc_out<sc_uint<DATA_WIDTH> > result;
	sc_out<bool> z_flag, c_flag, ready;

	sc_signal<sc_uint<DATA_WIDTH+1> > tmp;

	void operation();
	void computing();

	enum operationCode{
		ADD = 1, //! сложение
		SUB = 2, //! вычитание
		INR = 3, //! инкремент
		DCR = 4, //! декремент
		MULT = 5, //! умножение
		SHIFT_LEFT = 6, //! сдвиг влево
        SHIFT_RIGHT = 7 //! сдвиг вправо
	};

	SC_CTOR(alu) :
		clock("clock"),
			command("command"),
			inFlags("inFlags"),
			a("a"),
			b("b"),
			result("result")
		{
			cout << "Executing new" << endl;
			//SC_CTHREAD(operation, clock.pos());
			//async_reset_signal_is(reset, true);
			SC_METHOD(computing);
			sensitive << command << a << b; 
		}
};

#endif	/* ALU_H */
