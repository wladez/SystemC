#include "state.h"

void state_machine::state_change()
{
    state = 0;
    wait();
    while(true){
        switch(state.read()){
            case 0:{
                if(in.read() == 0)
                    state = 1;
                else if(in.read() == 3)
                    state = 2;
                break;
            }
            case 1:{
                if(in.read() == 0)
                    state = 3;
                else if(in.read() == 1)
                    state = 0;
                break;
            }
            case 2:{
                if(in.read() == 3)
                    state = 0;
                break;
            }
            case 3:{
                if(in.read() == 0)
                    state = 1;
                else if(in.read() == 1)
                    state = 2;
                break;
            }
        }
        wait();
    }
}

void state_machine::data_out_method()
{
    switch(state.read())
    {
        case 0:{
            out = 3;
            break;
        }
        case 1:{
            out = 1;
            break;
        }
        case 2:{
            out = 0;
            break;
        }
        case 3:{
            out = 2;
            break;
        }
    }
}