#include "alu.h"

void alu::operation()
{
	result = 0;
	z_flag = false;
	c_flag = false;
	ready = false;
	wait();
	while(true){
		ready = false;
		switch(command.read()){
		case ADD:
			result = a.read() + b.read();
			tmp = a.read() + b.read();
			ready = true;
			break;
		case SUB:
			result = a.read() - b.read();
			tmp = a.read() - b.read();
			ready = true;
			break;
		case INR:
			result = a.read() + 1;
			tmp = a.read() + 1;
			ready = true;
			break;
		case DCR:
			result = a.read() - 1;
			tmp = a.read() - 1;
			ready = true;
			break;
		case MULT:
			result = a.read() * b.read();
			tmp = a.read() * b.read();
			ready = true;
			break;
		case SHIFT_LEFT:
			result = a.read() << 1;
			tmp = a.read() << 1;
			ready = true;
			break;
		case SHIFT_RIGHT:
			result = a.read() >> 1;
			tmp = a.read() >> 1;
			ready = true;
			break;
		}
		if(result.read() == 0)
			z_flag = true;
		else z_flag = false;

		if(tmp.read() >> 32 & 0x1)
			c_flag = true;
		else c_flag = false;

		wait();
	}
}

void alu::computing()
{
	switch(command.read()){
	case ADD:
		result = a.read() + b.read();
		tmp = a.read() + b.read();
		ready = true;
		break;
	case SUB:
		result = a.read() - b.read();
		tmp = a.read() - b.read();
		ready = true;
		break;
	case INR:
		result = a.read() + 1;
		tmp = a.read() + 1;
		ready = true;
		break;
	case DCR:
		result = a.read() - 1;
		tmp = a.read() - 1;
		ready = true;
		break;
	case MULT:
		result = a.read() * b.read();
		tmp = a.read() * b.read();
		ready = true;
		break;
	case SHIFT_LEFT:
		result = a.read() << 1;
		tmp = a.read() << 1;
		ready = true;
		break;
	case SHIFT_RIGHT:
		result = a.read() >> 1;
		tmp = a.read() >> 1;
		ready = true;
		break;
	//default:
	//	result = 0;
	//	z_flag = false;
	//	ready = false;
	//	break;
	}
	if(result.read() == 0)
		z_flag = true;
	else z_flag = false;

	if(tmp.read() >> 32 & 0x1)
		c_flag = true;
	else c_flag = false;
}