#include "systemc.h"
#include "mmu.h"
#include "constants.h"

#define soft_assert(signal, expected) \
      if (signal.read() != expected) { \
          cerr << "@" << sc_time_stamp() << " Check failed. Expected: " << expected << ". Actual: " << signal.read() << ".\n" << endl; \
      }

sc_clock clk("clock", 4, SC_NS);
sc_signal<bool> sreset;                     // restart

sc_signal<bool> controller_read;                       // read from cell
sc_signal<bool> controller_write;                      // write in cell
sc_signal<sc_uint<12> > controller_addr_in;
sc_signal<sc_uint<DATA_WIDTH> > controller_data_in;
sc_signal<sc_uint<DATA_WIDTH> > controller_data_out;
sc_signal<bool> controller_out_ready;
sc_signal<bool> controller_in_save_all;

// to ram memory
sc_signal<bool> ram_read;                  // read from cell
sc_signal<bool> ram_write;                 // write in cell
sc_signal<sc_uint<10> > ram_addr_out;
sc_signal<sc_uint<DATA_WIDTH> > ram_data_out;
sc_signal<sc_uint<DATA_WIDTH> > ram_data_in;
sc_signal<bool> ram_in_ready;

// to hard memory
sc_signal<bool> disk_read;                  // read from cell
sc_signal<bool> disk_write;                 // write in cell
sc_signal<sc_uint<12> > disk_addr_out;
sc_signal<sc_uint<DATA_WIDTH> > disk_data_out;
sc_signal<sc_uint<DATA_WIDTH> > disk_data_in;
sc_signal<bool> disk_in_ready;

// memory
int ram_memory[1024];
int hardware_memory[4096];


int TIMEOUT_RAM = 4;   
int TIMEOUT_HARD_MEMORY = 7;  
int CYCLE_INDEX = TIMEOUT_HARD_MEMORY + 1;
int ram_counter = 0;
int hm_counter = 0;
bool ram_run = false, hm_run = false;
bool buffer_ram_read = false, buffer_ram_write = false, buffer_hm_read = false, buffer_hm_write = false;

void setSignal(){
    
    if(ram_read || ram_write){
        ram_run = true;
        ram_in_ready = false; 
        buffer_ram_read = ram_read.read();
        buffer_ram_write = ram_write.read();
    }
    
    if(ram_run){
        ram_counter++;
    }
    
    if(ram_counter == TIMEOUT_RAM && buffer_ram_read)
        ram_data_in = ram_memory[ram_addr_out.read()];

    
    if(ram_counter == TIMEOUT_RAM && buffer_ram_write)
        ram_memory[ram_addr_out.read()] = ram_data_out.read();
    
    
    if(ram_counter == TIMEOUT_RAM){
        ram_counter = 0;
        ram_run = false;
        buffer_ram_read = false;
        buffer_ram_write = false;
        ram_in_ready = true;
    }
    
    if(hm_run){
        hm_counter++;
    }
    
    if(disk_read || disk_write){
        hm_run = true;
        disk_in_ready = false; 
        buffer_hm_read = disk_read.read();
        buffer_hm_write = disk_write.read();
    }
    
    if(hm_counter == TIMEOUT_HARD_MEMORY && buffer_hm_read){        
        disk_data_in = hardware_memory[disk_addr_out.read()];
    }
    
    
    if(hm_counter == TIMEOUT_HARD_MEMORY && buffer_hm_write) 
        hardware_memory[disk_addr_out.read()] = disk_data_out.read();
    
    if(hm_counter == TIMEOUT_HARD_MEMORY){
        hm_counter = 0;
        hm_run = false;
        buffer_hm_read = false;
        buffer_hm_write = false;
        disk_in_ready = true;
    }

}

void printRAM(){
    cout <<"\n\n\n\n*******************************printRAM*************************" <<endl;
    int i,j;
    for(j = 0; j < 32; j++){
        for(i = 0; i < 32; i++){
            cout<< ram_memory[j*32 + i] <<" ";
        }
        if((j*32 + i) % 256 == 0)
            cout<<"_"<< endl;
        else
            cout<<endl;
    }
}

void printHARDWARE(){
    cout <<"\n\n\n\n*******************************printDISK********************" <<endl;
    int i,j;
    for(j = 0; j < 128; j++){
        for(i = 0; i < 32; i++){
            cout<< hardware_memory[j*32 + i] <<" ";
        }
        if((j*32 + i) % 256 == 0)
            cout<<"_"<< endl;
        else
            cout<<endl;
    }
}
int sc_main(int argc, char* argv[]) {
    mmu vm("mmu");
    vm.clk(clk);
    vm.sreset(sreset);   
    vm.controller_read(controller_read);
    vm.controller_write(controller_write);
    vm.controller_addr_in(controller_addr_in);
    vm.controller_data_in(controller_data_in);
    vm.controller_data_out(controller_data_out);
    vm.controller_out_ready(controller_out_ready);
    vm.controller_in_save_all(controller_in_save_all);
    vm.ram_read(ram_read);
    vm.ram_write(ram_write);
    vm.ram_addr_out(ram_addr_out);
    vm.ram_data_out(ram_data_out);
    vm.ram_data_in(ram_data_in);
    vm.ram_in_ready(ram_in_ready);
    vm.disk_read(disk_read);
    vm.disk_write(disk_write);
    vm.disk_addr_out(disk_addr_out);
    vm.disk_data_out(disk_data_out);
    vm.disk_data_in(disk_data_in);
    vm.disk_in_ready(disk_in_ready);

	vm.first_step=true;
	vm.inner_read = false;                        
	vm.inner_write = false;

	// flags
	vm.changing_page = false;
	vm.page_hit = false;
	vm.ram_is_empty = true;
	vm.memory_is_lock = false;
	vm.wait_data = false;
	vm.stop_and_save = false;
	vm.page_is_saving = false;

	// local registers
	vm.addr_page = 0;       // addr of the page
	vm.page_index = 0;     // index in array
	vm.page_size = 256;
	vm.bias = 0;
	vm.pointer = 0;
	vm.pointer_for_end = 0;

    sc_trace_file *wf = sc_create_vcd_trace_file("MMU_waveform");
    // Dump the desired signals
    sc_trace(wf, clk, "clock");
    sc_trace(wf, sreset, "sreset");
    sc_trace(wf, controller_read, "controller_read");
    sc_trace(wf, controller_write, "controller_write");
    sc_trace(wf, controller_addr_in, "controller_addr_in");
    sc_trace(wf, controller_data_in, "controller_data_in");
    sc_trace(wf, controller_data_out, "controller_data_out");
    sc_trace(wf, controller_out_ready, "controller_out_ready");
    sc_trace(wf, controller_in_save_all, "controller_in_save_all");
    sc_trace(wf, ram_read, "ram_read");
    sc_trace(wf, ram_write, "ram_write");
    sc_trace(wf, ram_addr_out, "ram_addr_out");
    sc_trace(wf, ram_data_out, "ram_data_out");
    sc_trace(wf, ram_data_in, "ram_data_in");
    sc_trace(wf, ram_in_ready, "ram_in_ready");
    sc_trace(wf, disk_read, "disk_read");
    sc_trace(wf, disk_write, "disk_write");
    sc_trace(wf, disk_addr_out, "disk_addr_out");
    sc_trace(wf, disk_data_out, "disk_data_out");
    sc_trace(wf, disk_data_in, "disk_data_in");
    sc_trace(wf, disk_in_ready, "disk_in_ready");
    sreset = false;
    
    //init hardware memory
    for(int i = 0; i < 4096; i ++)
        hardware_memory[i] = i;
    
 //   sreset = 1; // Assert the reset
	//cout << "@" << sc_time_stamp() << " Asserting reset\n" << endl;
	//sc_start(8, SC_NS);

	//sreset = 0; // De-assert the reset
	//cout << "@" << sc_time_stamp() << " De-Asserting reset\n" << endl;
	//sc_start(8, SC_NS);
	
	// nothing
    sc_start(4, SC_NS);
    disk_in_ready = true;                 // real memory is ready to work
    ram_in_ready = true; 
    sc_start(11, SC_NS);
    
    //1 READ *******************************************************************
    sc_start(4, SC_NS);
    
    controller_read = true;                  // set signal by reading
    controller_addr_in = 10;               // virtual addr a cell, which should be reading
    
    sc_start(4, SC_NS);
    
    // reset input signals (show, that signals is not up all time)
    controller_read = false;
    controller_addr_in = 0; 
    
    // read page from hardware memory
    for(int i = 0; i < 260 * CYCLE_INDEX; i ++){
        setSignal();
        sc_start(4, SC_NS);
    }
    
    // 2 WRITE WITHOU LOADING NEW PAGE *****************************************
    controller_write = true;                 // set signal by reading
    controller_addr_in = 50;               // virtual addr a cell, which should be reading
    controller_data_in = 2222222;
    
   
    sc_start(4, SC_NS);
    setSignal();
     
    //3 WRITE WITH LOADING NEW PAGE *******************************************************************
    controller_write = true;                  // set signal by reading
    controller_addr_in = 2010;               // virtual addr a cell, which should be reading
    controller_data_in = 33333333;
    sc_start(4, SC_NS);
    
    // reset input signals (show, that signals is not up all time)
    controller_write = false;
    controller_addr_in = 0; 
    controller_data_in = 0;
    
    // read page from hardware memory
    for(int i = 0; i < 260 * CYCLE_INDEX; i ++){
        setSignal();
        sc_start(4, SC_NS);
    }
   
    sc_start(8, SC_NS);
    
    //4 WRITE WITH LOADING NEW PAGE *******************************************************************
    controller_write = true;                  // set signal by reading
    controller_addr_in = 4095;               // virtual addr a cell, which should be reading
    controller_data_in = 4444444;
    sc_start(4, SC_NS);
    
    // reset input signals (show, that signals is not up all time)
    controller_write = false;
    controller_addr_in = 0; 
    controller_data_in = 0;
    
    // read page from hardware memory
    for(int i = 0; i < 260 * CYCLE_INDEX; i ++){
        setSignal();
        sc_start(4, SC_NS);
    }
   
    sc_start(8, SC_NS);
    
    //5 WRITE WITH LOADING NEW PAGE *******************************************************************
    controller_write = true;                  // set signal by reading
    controller_addr_in = 700;               // virtual addr a cell, which should be reading
    controller_data_in = 5555555;
    sc_start(4, SC_NS);
    
    // reset input signals (show, that signals is not up all time)
    controller_write = false;
    controller_addr_in = 0; 
    controller_data_in = 0;
    
    // read page from hardware memory
    for(int i = 0; i < 260 * CYCLE_INDEX; i ++){
        setSignal();
        sc_start(4, SC_NS);
    }
   
    sc_start(8, SC_NS);
    
	//printHARDWARE();
   // printRAM();
    
    //6 READ *******************************************************************    
    controller_read = true;                  // set signal by reading
    controller_addr_in = 1400;             // virtual addr a cell, which should be reading
    
    sc_start(4, SC_NS);
    
    // reset input signals (show, that signals is not up all time)
    controller_read = false;
    controller_addr_in = 0; 
    
    // read page from hardware memory
    for(int i = 0; i < 520 * CYCLE_INDEX; i ++){
        setSignal();
        sc_start(4, SC_NS);
    }
    sc_start(20, SC_NS);
   
    //6 READ *******************************************************************    
    controller_read = true;                  // set signal by reading
    controller_addr_in = 813;             // virtual addr a cell, which should be reading
    
    sc_start(4, SC_NS);
    
    // reset input signals (show, that signals is not up all time)
    controller_read = false;
    controller_addr_in = 0; 
    
    // read page from hardware memory
    for(int i = 0; i < 520 * CYCLE_INDEX; i ++){
        setSignal();
        sc_start(4, SC_NS);
    }
    sc_start(20, SC_NS);
    
    ////6 SAVE *******************************************************************    
    controller_in_save_all = true;                  // set signal by reading
    sc_start(4, SC_NS);
    
    // reset input signals (show, that signals is not up all time)
    controller_in_save_all = false;

    // save pages to hardware memory
    for(int i = 0; i < 780 * CYCLE_INDEX; i ++){
        setSignal();
        sc_start(4, SC_NS);
    }   
    printHARDWARE();
    printRAM();

    return 0;
}
