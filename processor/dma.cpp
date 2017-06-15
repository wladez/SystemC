#include "dma.h"

void dma::dma_states()
{
	state = 0;
	wait();
	while(true){
		switch(state.read())
		{
		case IDLE:
			if(cs0)
				state = DEVICE0;
			else if(cs1)
				state = DEVICE1;
			break;
		case DEVICE0:
			if(operation0)
				state = WRITE_MEM0;
			else 
				state = READ_MEM0;
			break;
		case WRITE_MEM0:
			if(ready)
			{
				state = END_MEM0;
			}
			break;
		case END_MEM0:
			if(cs1)
				state = DEVICE1;
			else state = IDLE;
			break;
		case READ_MEM0:
			if(ready)
				state = GET_RESULT0;
			break;
		case GET_RESULT0:
			if(cs1)
				state = DEVICE1;
			else state = IDLE;
			break;
		case DEVICE1:
			if(operation1)
				state = WRITE_MEM1;
			else
				state = READ_MEM1;
			break;
		case WRITE_MEM1:
			if(ready)
			{
				state = END_MEM1;
			}
			break;
		case END_MEM1:
			if(cs0)
				state = DEVICE0;
			else state = IDLE;
			break;
		case READ_MEM1:
			if(ready)
				state = GET_RESULT1;
			break;
		case GET_RESULT1:
			if(cs0)
				state = DEVICE0;
			else state = IDLE;
			break;
		}
		wait();
	}
}

void dma::dma_out()
{
	switch(state.read())
	{
	case IDLE:
		{
			write = false;
			read = false;
			ack0 = false;
			ack1 = false;
			addr_out = 0;
			mem_out = 0;
			break;
		}
	case DEVICE0:
		{
			write = false;
			read = false;
			ack0 = false;
			ack1 = false;
			mem_out = 0;
			addr_out = addr0.read();
			break;
		}
	case WRITE_MEM0:
		{
			write = true;
			read = false;
			ack0 = false;
			ack1 = false;
			mem_out = data_in0.read();
			break;
		}
	case END_MEM0:
		{
			write = false;
			read = false;
			ack0 = true;
			ack1 = false;
			break;
		}
	case READ_MEM0:
		{
			write = false;
			read = true;
			ack0 = false;
			ack1 = false;
			break;
		}
	case GET_RESULT0:
		{
			write = false;
			read = false;
			out0 = mem_in.read();
			ack0 = true;
			ack1 = false;
			break;
		}
	case DEVICE1:
		{
			write = false;
			read = false;
			ack0 = false;
			ack1 = false;
			mem_out = 0;
			addr_out = addr1.read();
			break;
		}
	case WRITE_MEM1:
		{
			write = true;
			read = false;
			ack0 = false;
			ack1 = false;
			mem_out = data_in1.read();
			break;
		}
	case END_MEM1:
		{
			write = false;
			read = true;
			ack0 = false;
			ack1 = false;
			break;
		}
	case READ_MEM1:
		{
			write = false;
			read = true;
			ack0 = false;
			ack1 = false;
			break;
		}
	case GET_RESULT1:
		{
			write = false;
			read = false;
			out1 = mem_in.read();
			ack1 = true;
			ack0 = false;
			break;
		}
	}
}