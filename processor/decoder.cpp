#include "decoder.h"

void decoder::state_change()
{
	state = 0;
	instruction = 0;
	wait();
	while(true)
	{
		switch(state.read())
		{
		case IDLE:
			{
				state = READ_INSTR_ADDR;
				break;
			}
		case READ_INSTR_ADDR:
			{
				state = READ_INSTR;
				break;
			}
		case READ_INSTR:
			{
				if(ack){
					state = INSTR_DEF;
					command = mem_in.read();
				}
				break;
			}
		


		case INSTR_DEF:
			{
				instruction = command.read() >> 26;
				state = INSTR_BEGIN;
				break;
			}
		case INSTR_BEGIN:
			{
				instruction = command.read() >> 26;
				switch(instruction.read())
				{
				case ADD:
				case SUB:
				case MULT:
					state = INSTR_ADD_SUB;
					break;
				case INR:
				case DCR:
				case SHIFT_LEFT:
				case SHIFT_RIGHT:
					state = INSTR_INR_DCR_SHIFT;
					break;
				case LOAD_I:
					state = INSTR_LOAD_I;
					break;
				case MOV:
					state = INSTR_MOV;
					break;
				case LOAD_M:
					state = INSTR_LOAD_M;
					break;
				case MOV_M:
					state = INSTR_MOV_M;
					break;
				case LOAD_MI:
					state = INSTR_LOAD_MI;
					break;
				case MOV_MI:
					state = INSTR_MOV_MI;
					break;
				case JMP:
					state = INSTR_JMP;
					break;
				case JNZ:
					state = INSTR_JNZ;
					break;
				case JZ:
					state = INSTR_JZ;
					break;
				case JNC:
					state = INSTR_JNC;
					break;
				case JC:
					state = INSTR_JC;
					break;
				case CALL:
					state = INSTR_CALL;
					break;
				case RET:
					state = INSTR_RET;
					break;
				case LOAD_IO:
					state = INSTR_LOAD_IO;
					break;
				case MOV_IO:
					state = INSTR_MOV_IO;
					break;
				default:
					state = IDLE;
					break;
				}
				break;
			}
		case INSTR_ADD_SUB:
			{
				state = RON_READ_2_PORTS;
				break;
			}
		case RON_READ_2_PORTS:
			{
				state = ADD_SUB_WRITE_TO_RON;
				break;
			}
		case ADD_SUB_WRITE_TO_RON:
			{
				state = WRITE_RON0;
				break;
			}
		case INSTR_INR_DCR_SHIFT:
			{
				state = RON0_READ;
				break;
			}
		case RON0_READ:
			{
				state = INR_WRITE_TO_RON;
				break;
			}
		case INR_WRITE_TO_RON:
			{
				state = 0;
				instruction = command.read() >> 26;
				break;
			}
		case INSTR_LOAD_I:
			{
				state = 0;
				instruction = command.read() >> 26;
				break;
			}
		case WRITE_RON0:
			{
				state = 0;
				instruction = command.read() >> 26;
				break;
			}
		case INSTR_MOV:
			{
				state = MOV_READ0;
				break;
			}
		case MOV_READ0:
			{
				state = MOV_WRITE1;
				break;
			}
		case MOV_WRITE1:
			{
				state = 0;
				instruction = command.read() >> 26;
				break;
			}
		case INSTR_LOAD_M:
			{
				state = LOAD_READ_DATA;
				break;
			}
		case LOAD_READ_DATA:
			{
				if(ack)
					state = LOAD_WRITE_RON;
				break;
			}
		case LOAD_WRITE_RON:
			{
				state = 0;
				instruction = command.read() >> 26;
				break;
			}
		case INSTR_MOV_M:
			{
				state = MOV_RON_READ;
				break;
			}
		case MOV_RON_READ:
			{
				state = MOV_WRITE_MEM;
				break;
			}
		case MOV_WRITE_MEM:
			{
				instruction = command.read() >> 26;
				if(ack)
					state = 0;
				break;
			}
		case INSTR_LOAD_MI:
			{
				state = READ_MEM_ADDR;
				break;
			}
		case READ_MEM_ADDR:
			{
				state = LOAD_MI_READ_DATA;
				break;
			}
		case LOAD_MI_READ_DATA:
			{
				if(ack)
					state = LOAD_WRITE_RON; 
				break;
			}
		case INSTR_MOV_MI:
			{
				state = MOV_MI_RON_READ;
				break;
			}
		case MOV_MI_RON_READ:
			{
				state = MOV_WRITE_MEM;
				break;
			}
		case INSTR_JMP:
			{
				state = JMP_WRITE_ADDR;
				break;
			}
		case INSTR_JNZ:
			{
				if(!z_flag)
					state = JMP_WRITE_ADDR;
				else 
					state = 0;
				instruction = command.read() >> 26;
				break;
			}
		case INSTR_JZ:
			{
				if(z_flag)
					state = JMP_WRITE_ADDR;
				else 
					state = 0;
				instruction = command.read() >> 26;
				break;
			}
		case INSTR_JNC:
			{
				if(!c_flag)
					state = JMP_WRITE_ADDR;
				else
					state = 0;
				break;
			}
		case INSTR_JC:
			{
				if(c_flag)
					state = JMP_WRITE_ADDR;
				else 
					state = 0;
				instruction = command.read() >> 26;
				break;
			}
		case JMP_WRITE_ADDR:
			{
				instruction = command.read() >> 26;
				state = 0;
				break;
			}
		case INSTR_CALL:
			{
				state = CALL_SAVE_OLD_ADDR;
				break;
			}
		case CALL_SAVE_OLD_ADDR:
			{
				state = JMP_WRITE_ADDR;
				break;
			}
		case INSTR_RET:
			{
				state = RET_READ_ADDR;
				break;
			}
		case RET_READ_ADDR:
			{
				state = JMP_WRITE_ADDR;
				break;
			}
		case INSTR_LOAD_IO:
			{
				state = LOAD_IO_READ;
				break;
			}
		case LOAD_IO_READ:
			{
				state = IO_TO_RON;
				break;
			}
		case IO_TO_RON:
			{
				state = 0;
				break;
			}
		case INSTR_MOV_IO:
			{
				state = 0;
				break;
			}
		}
		wait();
	}
}


//!!!!!!!!проверить все управляющие сигналы во всех состояниях
void decoder::out_method()
{
	switch(state.read())
	{
	case IDLE:
		{
			count_ena = false;
			count_load = false;
			alu_command = 0;
			ron_address0 = 0;
			ron_address1 = 0;
			ron_data0 = 0;
			ron_data1 = 0;
			memory_address = command_addr.read();
			ron_we0 = false;
			ron_we1 = false;
			ron_re0 = false;
			ron_re1 = false;
			mem_out = 0;
			mem_write = false;
			mem_read = false;
			instruction_end = false;
			io_write = false;
			io_read = false;
			break;
		}
	case READ_INSTR_ADDR:
		{
			count_ena = false;
			count_load = false;
			alu_command = 0;
			ron_address0 = 0;
			ron_address1 = 0;
			ron_data0 = 0;
			ron_data1 = 0;
			ron_we0 = false;
			ron_we1 = false;
			ron_re0 = false;
			ron_re1 = false;
			mem_out = 0;
			mem_write = false;
			mem_read = true;
			break;
		}
	case READ_INSTR:
		{
			count_ena = false;
			count_load = false;
			alu_command = 0;
			ron_address0 = 0;
			ron_address1 = 0;
			ron_data0 = 0;
			ron_data1 = 0;
			ron_we0 = false;
			ron_we1 = false;
			ron_re0 = false;
			ron_re1 = false;
			mem_out = 0;
			mem_write = false;
			mem_read = true;
			break;
		}
	case INSTR_DEF:
		{
			count_ena = false;
			count_load = false;
			alu_command = 0;
			ron_address0 = 0;
			ron_address1 = 0;
			ron_data0 = 0;
			ron_data1 = 0;
			ron_we0 = false;
			ron_we1 = false;
			ron_re0 = false;
			ron_re1 = false;
			mem_out = 0;
			mem_write = false;
			mem_read = false;
			break;
		}
	case INSTR_BEGIN:
		{
			count_ena = true;
			count_load = false;
			alu_command = 0;
			ron_address0 = 0;
			ron_address1 = 0;
			ron_data0 = 0;
			ron_data1 = 0;
			memory_address = 0;
			ron_we0 = false;
			ron_we1 = false;
			ron_re0 = false;
			ron_re1 = false;
			mem_out = 0;
			mem_write = false;
			mem_read = false;
			io_write = false;
			io_read = false;
			instruction_end = false;
			break;
		}
	case INSTR_ADD_SUB:
		{
			count_ena = false;
			count_load = false;
			ron_address0 = (command.read() >> 10) & 0x001F;
			ron_address1 = (command.read() >> 5) & 0x001F;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = true;
			ron_we1 = false;
			alu_command = instruction.read();
			instruction_end = false;
			break;
		}
	case RON_READ_2_PORTS:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = true;
			ron_we1 = false;
			instruction_end = false;
			break;
		}
	case ADD_SUB_WRITE_TO_RON:
		{
			count_ena = false;
			count_load = false;
			alu_command = 0;
			ron_address0 = (command.read()) & 0x001F;
			ron_data0 = alu_res.read();
			ron_re0 = false;
			ron_we0 = true;
			ron_re1 = false;
			ron_we1 = false;
			instruction_end = true;
			break;
		}
	case INSTR_INR_DCR_SHIFT:
		{
			count_ena = false;
			count_load = false;
			ron_address0 = (command.read() >> 5) & 0x001F;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			alu_command = instruction.read();
			instruction_end = false;
			break;
		}
	case RON0_READ:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			instruction_end = false;
			break;
		}
	case INR_WRITE_TO_RON:
		{
			count_ena = false;
			count_load = false;
			ron_address0 = (command.read()) & 0x001F;
			ron_data0 = alu_res.read();
			ron_we0 = true;
			ron_re0 = false;
			instruction_end = true;
			break;
		}
	case INSTR_LOAD_I:
		{
			count_ena = false;
			count_load = false;
			ron_address0 = (command.read() >> 21) & 0x001F;
			ron_we0 = true;
			ron_re0 = false;
			ron_data0 = command.read() & 0x01FFFFF;
			break;
		}
	case WRITE_RON0:
		{
			count_ena = false;
			alu_command = 0;
			count_load = false;
			ron_we0 = true;
			ron_re0 = false;
			break;
		}
	case INSTR_MOV:
		{
			count_ena = false;
			count_load = false;
			ron_address0 = (command.read() >> 5) & 0x001F;
			ron_address1 = (command.read()) & 0x001F;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			alu_command = 0;
			instruction_end = false;
			break;
		}
	case MOV_READ0:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			ron_data1 = ron_inp0.read();
			instruction_end = false;
			break;
		}
	case MOV_WRITE1:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = true;
			instruction_end = true;
			break;
		}
	case INSTR_LOAD_M:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			memory_address = (command.read() >> 5) & 0xFF;
			ron_address0 = command.read() & 0x1F;
			mem_out = 0;
			mem_write = false;
			mem_read = true;
			break;
		}
	case LOAD_READ_DATA:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_out = 0;
			mem_write = false;
			mem_read = true;
			break;
		}
	case LOAD_WRITE_RON:
		{
			ron_re0 = false;
			ron_we0 = true;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			ron_data0 = mem_in.read();
			break;
		}
	case INSTR_MOV_M:
		{
			count_ena = false;
			count_load = false;
			ron_address0 = command.read() & 0x1F;
			memory_address = (command.read() >> 5) & 0xFF;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			break;
		}
	case MOV_RON_READ:
		{
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_out = ron_inp0.read();
			mem_write = true;
			mem_read = false;
			break;
		}
	case MOV_WRITE_MEM:
		{
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = true;
			mem_read = false;
			break;
		}
	case INSTR_LOAD_MI:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = true;
			ron_we1 = false;
			ron_address1 = (command.read() >> 5) & 0x1F;
			ron_address0 = command.read() & 0x1F;
			mem_out = 0;
			mem_write = false;
			mem_read = false;
			break;
		}
	case READ_MEM_ADDR:
		{
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			memory_address = ron_inp1.read();
			mem_out = 0;
			mem_write = false;
			mem_read = true;
			break;
		}
	case LOAD_MI_READ_DATA:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_out = 0;
			mem_write = false;
			mem_read = true;
			break;
			break;
		}
	case INSTR_MOV_MI:
		{
			count_ena = false;
			count_load = false;
			ron_address0 = command.read() & 0x1F;
			ron_address1 = (command.read() >> 5) & 0x1F;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = true;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			break;
		}
	case MOV_MI_RON_READ:
		{
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = true;
			ron_we1 = false;
			memory_address = ron_inp1.read();
			mem_out = ron_inp0.read();
			mem_write = false;
			mem_read = false;
			break;
		}
	case INSTR_JMP:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			memory_address = command.read() & 0xFF;
			break;
		}
	case INSTR_JNZ:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			memory_address = command.read() & 0xFF;
			break;
		}
	case INSTR_JZ:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			memory_address = command.read() & 0xFF;
			break;
		}
	case INSTR_JNC:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			memory_address = command.read() & 0xFF;
			break;
		}
	case INSTR_JC:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			memory_address = command.read() & 0xFF;
			break;
		}
	case JMP_WRITE_ADDR:
		{
			count_ena = true;
			count_load = true;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			break;
		}
	case INSTR_CALL:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			ron_data0 = command_addr.read();
			ron_address0 = 31;
			memory_address = command.read() & 0xFF;
			break;
		}
	case CALL_SAVE_OLD_ADDR:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = true;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			break;
		}
	case INSTR_RET:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			ron_address0 = 31; 
			mem_write = false;
			mem_read = false;
			break;
		}
	case RET_READ_ADDR:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = true;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			memory_address = ron_inp0.read();
			break;
		}
	case INSTR_LOAD_IO:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			io_write = false;
			io_read = true;
			ron_address0 = command.read() & 0x1F;
			break;
		}
	case LOAD_IO_READ:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			io_write = false;
			io_read = true;
			ron_data0 = io_in.read();
			break;
		}
	case IO_TO_RON:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = true;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			break;
		}
	case INSTR_MOV_IO:
		{
			count_ena = false;
			count_load = false;
			ron_re0 = false;
			ron_we0 = false;
			ron_re1 = false;
			ron_we1 = false;
			mem_write = false;
			mem_read = false;
			io_out = command.read();
			io_write = true;
			io_read = false;
			break;
		}
	}
}

//void decoder::command_handler()
//{
//	alu_command = 0;
//	ron_address0 = 0;
//	ron_address1 = 0;
//	ron_data0 = 0;
//	ron_data1 = 0;
//	memory_address = 0;
//	instruction = 0;
//	ron_cs0 = false;
//	ron_cs1 = false;
//	instruction_end = false;
//	alu_ena = false;
//	//ron_we0 = false;
//	//ron_we1 = false;
//	//ron_re0 = false;
//	//ron_re1 = false;
//	wait();
//	while(true){
//		instruction_end = false;
//		ron_cs0 = false;
//		ron_cs1 = false;
//		instruction = command.read() >> 26;
//		switch(instruction.read())
//		{
//		case ADD:
//		case SUB:
//			if(instruction_end)
//				alu_ena = false;
//			else alu_ena = true;
//			ron_address0 = (command.read() >> 10) & 0x001F;
//			ron_address1 = (command.read() >> 5) & 0x001F;
//			ron_cs0 = true;
//			//ron_re0 = true;
//			//ron_we0 = false;
//			ron_cs1 = true;
//			//ron_re1 = true;
//			//ron_we1 = false;
//			alu_command = instruction.read();
//			if(alu_ready){
//				ron_address0 = (command.read()) & 0x001F;
//				ron_data0 = alu_res.read();
//				ron_cs1 = false;
//				instruction_end = true;
//				//ron_re1 = false;
//			}			
//			break;
//		case LOAD_I:
//			alu_ena = false;
//			ron_address0 = (command.read() >> 21) & 0x001F;
//			ron_cs0 = true;
//			ron_cs1 = false;
//			//ron_we0 = true;
//			//ron_re0 = false;
//			ron_data0 = command.read() & 0x01FFFFF;
//			instruction_end = true;
//			break;
//		default:
//			ron_cs0 = false;
//			ron_cs1 = false;
//			//ron_we0 = false;
//			//ron_we1 = false;
//			//ron_re0 = false;
//			//ron_re1 = false;
//			alu_command = 0;
//			break;
//		}
//		//if(instruction_end)
//		//{
//		//	ron_cs0 = false;
//		//	ron_cs1 = false;
//		//}
//		wait();
//	}
//}
//
//void decoder::alu_enable()
//{
//
//}
//
//void decoder::enable_method()
//{
//	switch(instruction.read())
//	{
//	case ADD:
//	case SUB:
//		ron_re0 = true;
//		ron_we0 = false;
//		ron_re1 = true;
//		ron_we1 = false;
//		if(alu_ready)
//			ron_we0 = true;
//		break;
//	case LOAD_I:
//		ron_we0 = true;
//		ron_re0 = false;
//		ron_we1 = false;
//		ron_re1 = false;
//		break;
//	default:
//		ron_we0 = false;
//		ron_we1 = false;
//		ron_re0 = false;
//		ron_re1 = false;
//		break;
//	}
//}