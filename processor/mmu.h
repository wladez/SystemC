#include "systemc.h"
#include "constants.h"

#ifndef MMU_H
#define	MMU_H
    SC_MODULE(mmu){

            sc_in_clk clk;                          // main clock
            sc_in<bool> sreset;                     // restart
            
            // to cache
            sc_in<bool> controller_read;                       // read from cell
            sc_in<bool> controller_write;                      // write in cell
            sc_in<sc_uint<12> > controller_addr_in;
            sc_in<sc_uint<DATA_WIDTH> > controller_data_in;
            sc_out<sc_uint<DATA_WIDTH> > controller_data_out;
            sc_out<bool> controller_out_ready;
            sc_in<bool> controller_in_save_all;
            
            // to ram memory
            sc_out<bool> ram_read;                  // read from cell
            sc_out<bool> ram_write;                 // write in cell
            sc_out<sc_uint<10> > ram_addr_out;
            sc_out<sc_uint<DATA_WIDTH> > ram_data_out;
            sc_in<sc_uint<DATA_WIDTH> > ram_data_in;
            sc_in<bool> ram_in_ready;
            
            // to hard memory
            sc_out<bool> disk_read;                  // read from cell
            sc_out<bool> disk_write;                 // write in cell
            sc_out<sc_uint<12> > disk_addr_out;
            sc_out<sc_uint<DATA_WIDTH> > disk_data_out;
            sc_in<sc_uint<DATA_WIDTH> > disk_data_in;
            sc_in<bool> disk_in_ready;
            
            unsigned int pages[4][2];               // page-table   (addr + flag_changed)
            
            
            
            int page, addr;                        // local variables
            bool first_step; //= true;                // first step in the executing command

            // buffer for input signals
            bool inner_read; //= false;                        
            bool inner_write; //= false;                      
            int inner_cell;
    
            // flags
            bool changing_page; //= false;
            bool page_hit; //= false;
            bool ram_is_empty; //= true;
            bool memory_is_lock; //= false;
            bool wait_data; //= false;
            bool stop_and_save; //= false;
            bool page_is_saving; //= false;
            
            // local registers
            int addr_page; //= 0;       // addr of the page
            int page_index; //= 0;     // index in array
            int page_size; //= 256;
            int bias; //= 0;
            int pointer; //= 0;
            int pointer_for_end; //= 0;
            
            void workWithMemory();
            
            SC_CTOR(mmu) :
                    clk("clk"),
                    sreset("sreset"),
                    controller_read("controller_read"),
                    controller_write("controller_write"),
                    controller_addr_in("controller_addr_in"), 
                    controller_data_in("controller_data_in"),
                    controller_data_out("controller_data_out"),
                    controller_out_ready("controller_out_ready"),
                    controller_in_save_all("controller_in_save_all"),
                    ram_read("ram_read"),
                    ram_write("ram_write"),
                    ram_addr_out("ram_addr_out"),
                    ram_data_out("ram_data_out"),
                    ram_data_in("ram_data_in"),
                    ram_in_ready("ram_in_ready"),
                    disk_read("disk_read"),
                    disk_write("disk_write"),
                    disk_addr_out("disk_addr_out"),
                    disk_data_out("disk_data_out"),
                    disk_data_in("disk_data_in"),
                    disk_in_ready("disk_in_ready")
            {      
                SC_CTHREAD(workWithMemory, clk.pos());
                async_reset_signal_is(sreset,false);
                
                for(int i = 0; i < 4; i++){
                    pages[i][0] = 16;
                    pages[i][1] = 0;
                }
                cout<<"reset"<<endl;
            }
    };
#endif