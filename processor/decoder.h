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
	//! убрать вход command и заменить его на внутреннюю переменную
	sc_signal<sc_uint <DATA_WIDTH> > command;


	//void command_handler();
	//void enable_method();
	//void alu_enable();

	void state_change();
	void out_method();

	enum instructionCode{
		//команды для АЛУ
		ADD = 1, //! сложение
		SUB = 2, //! вычитание
		INR = 3, //! инкремент
		DCR = 4, //! декремент
		MULT = 5, //! умножение
		SHIFT_LEFT = 6, //! сдвиг влево
        SHIFT_RIGHT = 7, //! сдвиг вправо

		//команды переходов
		JMP = 0x10, //! безусловный переход
		JNZ = 0x11, //! переход по ненулевому результату
		JZ = 0x12, //! переход по нулевому результату
		JNC = 0x13, //! переход при отсутствии переноса
		JC = 0x14, //! переход при наличии переноса
		CALL = 0x15, //! вызов подпрограммы
		RET = 0x16, //! возврат из подпрограммы

		//команды для работы с памятью
		LOAD_I = 0x21, //! загрузка регистра непосредственная
		MOV = 0x22, //! пересылка из регистра в регистр
		LOAD_M = 0x23, //! загрузка из памяти в регистр
		MOV_M = 0x24, //! сохранение из регистра в память
		LOAD_MI = 0x25, //! косвенная загрузка из памяти в регистр
		MOV_MI = 0x26, //! косвенное сохранение из регистра в память
		
		//команды для работы с GPIO
		LOAD_IO = 0x27, //! считывание из блока GPIO
		MOV_IO = 0x28 //! запись в блок GPIO
	};

	enum stateCode{
		//-----считывание команды из памяти------
		IDLE = 0, //! начальное состояние
		READ_INSTR_ADDR = 37, //! начало чтения команды - считывание её адреса
		READ_INSTR = 38, //! процесс считывания команды

		//-----обработка и выполнение команды------
		INSTR_DEF = 39, //! определение кода операции
		INSTR_BEGIN = 40, //! начало обработки команды
		INSTR_ADD_SUB = 1, //! команда для АЛУ сложение или вычитание (2 операнда)
		INSTR_INR_DCR_SHIFT = 2, //! команда для АЛУ инкремент, декремент или логический сдвиг (1 операнд)
		INSTR_LOAD_I = 3, //! команда загрузка регистра непосредственная
		INSTR_MOV = 4, //! команда пересылка из регистра в регистр
		INSTR_LOAD_M = 5, //! команда загрузка из памяти в регистр
		INSTR_MOV_M = 6, //! команда сохранение из регистра в память
		INSTR_LOAD_MI = 7, //! команда косвенная загрузка из памяти в регистр
		INSTR_MOV_MI = 8, //! команда косвенное сохранение из регистра в память
		INSTR_JMP = 9, //! команда безусловного перехода
		INSTR_JNZ = 10, //! команда перехода по ненулевому результату
		INSTR_JZ = 11, //! команда перехода по нулевому результату
		INSTR_JNC = 12, //! команда перехода при отсутствии переноса
		INSTR_JC = 13, //! команда перехода при наличии переноса
		INSTR_CALL = 14, //! команда вызова подпрограммы
		INSTR_RET = 15, //! команда возврата из подпрограммы
		INSTR_LOAD_IO = 16, //! команда считывания из блока GPIO
		INSTR_MOV_IO = 17, //! команда записи в блок GPIO
		RON_READ_2_PORTS = 18, //! чтение из обоих портов РОН
		RON0_READ = 19, //! чтение из 0 порта РОН
		ADD_SUB_WRITE_TO_RON = 20, //! часть исполнения команды сложения или вычитания - запись результата в РОН
		INR_WRITE_TO_RON = 21, //! часть исполнения команды инкремента, декремента или логического сдвига - запись результата в РОН
		WRITE_RON0 = 22, //! запись в 0 порт РОН
		MOV_READ0 = 23, //! чтение при пересылке из 0 порта РОН
		MOV_WRITE1 = 24, //! запись в 1 порт РОН при пересылке
		LOAD_READ_DATA = 25, //! часть исполнения команды загрузки из памяти - чтение данных из памяти
		LOAD_WRITE_RON = 26, //! часть исполнения команды загрузки из памяти - запись в РОН
		MOV_RON_READ = 27, //! часть исполнения команды сохранения в память - чтение данных из РОН
		MOV_WRITE_MEM = 28, //! часть исполнения команды сохранения в память - запись данных в память
		JMP_WRITE_ADDR = 29, //! часть исполнения команды безусловного перехода - запись в счётчик команд необходимого адреса
		CALL_SAVE_OLD_ADDR = 30, //! часть исполнения команды вызова подпрограммы - запоминание текущего адреса
		RET_READ_ADDR = 31, //! 
		LOAD_IO_READ = 32, //! чтение данных из блока GPIO
		IO_TO_RON = 33, //! запись из GPIO в РОН
		READ_MEM_ADDR = 34, //! чтенеие адреса из регистра РОН - часть косвенной загрузки
		LOAD_MI_READ_DATA = 35, //! чтение данных из памяти - часть косвенной загрузки из памяти
		MOV_MI_RON_READ = 36 //! чтение адреса и данных из РОН - косвенная запись в память
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

