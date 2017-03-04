#include "shift.h"

void shift_register::shifting()
{
    reg = 0;
    register_out.write(reg);
    c_out = reg.bit(0);
    wait();
    while(true)
    {
        if(load.read()==1)
            reg = data_in.read();
        else {
            c_out = reg.bit(0);
            reg = (c_in, reg(7,1));
        }
        register_out.write(reg);
        wait();
    }
}
