#include "counter.h"

void counter::do_count() {
    count = 0;
    counter_out.write(count);
    wait();
    while (true) {
        if (ena.read() == 1) {
            count = count + 1;
            counter_out.write(count);
        }
        wait();
    }
}
