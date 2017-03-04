/* 
 * File:   counter.h
 * Author: student
 *
 * Created on February 17, 2017, 11:50 AM
 */

#ifndef COUNTER_H
#define	COUNTER_H

#include "systemc.h"

SC_MODULE(counter) {
    sc_in_clk clock; // Clock input of the design
    sc_in<bool> reset;
    sc_in<bool> sreset; // active high, synchronous Reset input
    sc_in<bool> ena; // count enable signal
    sc_out<sc_uint<8> > counter_out; // 8 bit vector output

    //------------Local Variables Here---------------------
    sc_uint<8> count;
    
    void do_count();
    
    //constructor
    SC_CTOR(counter) :
            clock("clock"),
            reset("reset"),
            sreset("sreset"),
            ena("ena"),
            counter_out("counter_out")
    {
        cout << "Executing new" << endl;
        SC_CTHREAD(do_count, clock.pos());
        async_reset_signal_is(reset, true);
        reset_signal_is(sreset, true);
    }
    
};

#endif	/* COUNTER_H */

