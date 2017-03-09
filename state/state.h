/* 
 * File:   state.h
 * Author: student
 *
 * Created on March 3, 2017, 11:56 AM
 */

#include "systemc.h"

#ifndef STATE_H
#define	STATE_H

SC_MODULE(state_machine) {
    sc_in_clk clock; // Clock input of the design
    sc_in<bool> reset; // active high, asynchronous Reset input
    sc_in<sc_uint<2> > in;
    sc_out<sc_uint<2> > out;
    
    sc_signal<sc_uint<2> > state;
    
    void state_change();
    
    void data_out_method();
    
    SC_CTOR(state_machine) :
    clock("clock"),
            reset("reset"),
            in("in"),
            out("out")
    {
        cout << "Executing new" << endl;
        SC_CTHREAD(state_change, clock.pos());
        async_reset_signal_is(reset, true);
        SC_METHOD(data_out_method);
        sensitive << state;
    }
};

#endif	/* STATE_H */

