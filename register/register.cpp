//-----------------------------------------------------
// An 8 bit register with asynchronous active high reset
//-----------------------------------------------------
#include "register.h"

void eightbit_register::register_store() {
    // We check if reset is active    
    myregister = 0;
    register_out.write(myregister);
    wait();
    while (true) {
        myregister = register_in.read();
        cout << "@" << sc_time_stamp() << " :: Have stored " << myregister << endl;
        register_out.write(myregister);
        wait();
    }
}
