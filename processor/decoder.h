#include "systemc.h"
#include "constants.h"


#ifndef DECODER_H
#define	DECODER_H

SC_MODULE(decoder)
{
    sc_in_clk clock;
    sc_in<bool> reset;
	sc_in<bool> alu_ready, ack, z_flag, c_flag;
    sc_in<sc_uint <DATA_WIDTH> > alu_res, mem_in;
	sc_in<sc_uint <DATA_WIDTH> > ron_inp0, ron_inp1;
	sc_in<sc_uint <ADDR_WIDTH> > command_addr;
	sc_in<sc_uint <GPIO_WIDTH> > io_in;
	sc_out<sc_uint <GPIO_WIDTH> > io_out;
	sc_out<bool> io_write, io_read;
	sc_out<bool> count_ena, count_load;
    sc_out<sc_uint <ADDR_WIDTH> > memory_address;
	sc_out<bool> mem_write, mem_read;
	sc_out<sc_uint <DATA_WIDTH> > mem_out;
	sc_out<sc_uint <4> > alu_command;
	sc_out<sc_uint <RON_ADDR_WIDTH> > ron_address0, ron_address1;
	sc_out<bool> ron_we0, ron_re0, ron_we1, ron_re1;
	sc_out<sc_uint <DATA_WIDTH> > ron_data0, ron_data1;
	sc_out<bool> instruction_end;

	sc_signal<sc_uint <6> > instruction;
	sc_signal<sc_uint <6> > state;
	//! ������ ���� command � �������� ��� �� ���������� ����������
	sc_signal<sc_uint <DATA_WIDTH> > command;


	//void command_handler();
	//void enable_method();
	//void alu_enable();

	void state_change();
	void out_method();

	enum instructionCode{
		//������� ��� ���
		ADD = 1, //! ��������
		SUB = 2, //! ���������
		INR = 3, //! ���������
		DCR = 4, //! ���������
		MULT = 5, //! ���������
		SHIFT_LEFT = 6, //! ����� �����
        SHIFT_RIGHT = 7, //! ����� ������

		//������� ���������
		JMP = 0x10, //! ����������� �������
		JNZ = 0x11, //! ������� �� ���������� ����������
		JZ = 0x12, //! ������� �� �������� ����������
		JNC = 0x13, //! ������� ��� ���������� ��������
		JC = 0x14, //! ������� ��� ������� ��������
		CALL = 0x15, //! ����� ������������
		RET = 0x16, //! ������� �� ������������

		//������� ��� ������ � �������
		LOAD_I = 0x21, //! �������� �������� ����������������
		MOV = 0x22, //! ��������� �� �������� � �������
		LOAD_M = 0x23, //! �������� �� ������ � �������
		MOV_M = 0x24, //! ���������� �� �������� � ������
		LOAD_MI = 0x25, //! ��������� �������� �� ������ � �������
		MOV_MI = 0x26, //! ��������� ���������� �� �������� � ������
		
		//������� ��� ������ � GPIO
		LOAD_IO = 0x27, //! ���������� �� ����� GPIO
		MOV_IO = 0x28 //! ������ � ���� GPIO
	};

	enum stateCode{
		//-----���������� ������� �� ������------
		IDLE = 0, //! ��������� ���������
		READ_INSTR_ADDR = 37, //! ������ ������ ������� - ���������� � ������
		READ_INSTR = 38, //! ������� ���������� �������

		//-----��������� � ���������� �������------
		INSTR_DEF = 39, //! ����������� ���� ��������
		INSTR_BEGIN = 40, //! ������ ��������� �������
		INSTR_ADD_SUB = 1, //! ������� ��� ��� �������� ��� ��������� (2 ��������)
		INSTR_INR_DCR_SHIFT = 2, //! ������� ��� ��� ���������, ��������� ��� ���������� ����� (1 �������)
		INSTR_LOAD_I = 3, //! ������� �������� �������� ����������������
		INSTR_MOV = 4, //! ������� ��������� �� �������� � �������
		INSTR_LOAD_M = 5, //! ������� �������� �� ������ � �������
		INSTR_MOV_M = 6, //! ������� ���������� �� �������� � ������
		INSTR_LOAD_MI = 7, //! ������� ��������� �������� �� ������ � �������
		INSTR_MOV_MI = 8, //! ������� ��������� ���������� �� �������� � ������
		INSTR_JMP = 9, //! ������� ������������ ��������
		INSTR_JNZ = 10, //! ������� �������� �� ���������� ����������
		INSTR_JZ = 11, //! ������� �������� �� �������� ����������
		INSTR_JNC = 12, //! ������� �������� ��� ���������� ��������
		INSTR_JC = 13, //! ������� �������� ��� ������� ��������
		INSTR_CALL = 14, //! ������� ������ ������������
		INSTR_RET = 15, //! ������� �������� �� ������������
		INSTR_LOAD_IO = 16, //! ������� ���������� �� ����� GPIO
		INSTR_MOV_IO = 17, //! ������� ������ � ���� GPIO
		RON_READ_2_PORTS = 18, //! ������ �� ����� ������ ���
		RON0_READ = 19, //! ������ �� 0 ����� ���
		ADD_SUB_WRITE_TO_RON = 20, //! ����� ���������� ������� �������� ��� ��������� - ������ ���������� � ���
		INR_WRITE_TO_RON = 21, //! ����� ���������� ������� ����������, ���������� ��� ����������� ������ - ������ ���������� � ���
		WRITE_RON0 = 22, //! ������ � 0 ���� ���
		MOV_READ0 = 23, //! ������ ��� ��������� �� 0 ����� ���
		MOV_WRITE1 = 24, //! ������ � 1 ���� ��� ��� ���������
		LOAD_READ_DATA = 25, //! ����� ���������� ������� �������� �� ������ - ������ ������ �� ������
		LOAD_WRITE_RON = 26, //! ����� ���������� ������� �������� �� ������ - ������ � ���
		MOV_RON_READ = 27, //! ����� ���������� ������� ���������� � ������ - ������ ������ �� ���
		MOV_WRITE_MEM = 28, //! ����� ���������� ������� ���������� � ������ - ������ ������ � ������
		JMP_WRITE_ADDR = 29, //! ����� ���������� ������� ������������ �������� - ������ � ������� ������ ������������ ������
		CALL_SAVE_OLD_ADDR = 30, //! ����� ���������� ������� ������ ������������ - ����������� �������� ������
		RET_READ_ADDR = 31, //! 
		LOAD_IO_READ = 32, //! ������ ������ �� ����� GPIO
		IO_TO_RON = 33, //! ������ �� GPIO � ���
		READ_MEM_ADDR = 34, //! ������� ������ �� �������� ��� - ����� ��������� ��������
		LOAD_MI_READ_DATA = 35, //! ������ ������ �� ������ - ����� ��������� �������� �� ������
		MOV_MI_RON_READ = 36 //! ������ ������ � ������ �� ��� - ��������� ������ � ������
	};

	SC_CTOR(decoder){
		//SC_CTHREAD(command_handler, clock.pos());
		//async_reset_signal_is(reset, true);
		//SC_CTHREAD(alu_enable, clock.pos());
		//async_reset_signal_is(reset, true);
		//SC_METHOD(enable_method);
		//sensitive << instruction << clock.pos();
		SC_CTHREAD(state_change, clock.pos());
		async_reset_signal_is(reset, true);
		SC_METHOD(out_method);
		sensitive << state;
	}
};

#endif	/* DECODER_H */

