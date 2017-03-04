/* 
 * File:   shift.h
 * Author: student
 *
 * Created on February 17, 2017, 12:33 PM
 */

#ifndef SHIFT_H
#define	SHIFT_H

#include "systemc.h"

SC_MODULE(shift_register) {
    sc_in_clk clock; // Clock input of the design
    sc_in<bool> reset;
    sc_in<bool> sreset; // active high, synchronous Reset input
    sc_in<bool> load; //load signal
    sc_in<sc_uint<8> > data_in;
    sc_in<bool> c_in;
    sc_out<bool> c_out;
    sc_out<sc_uint<8> > register_out;
    
    sc_uint<8> reg;
    
    void shifting();
    
    SC_CTOR(shift_register) :
            clock("clock"),
            reset("reset"),
            sreset("sreset"),
            load("load"),
            data_in("data_in"),
            register_out("register_out"),
            c_out("c_out"),
            c_in("c_in")
    {
        cout << "Executing new" << endl;
        SC_CTHREAD(shifting, clock.pos());
        async_reset_signal_is(reset, true);
        reset_signal_is(sreset, true);
    }
    
};

#endif	/* SHIFT_H */

