#include "fifo.h"

void fifo::processing()
{
	empty = true;
	full = false;
	data_out = 0;
	first = 0;
	num_elements = 0;
	for(int i = 0; i<=max; i++)
		data[i] = 0;
	wait();
	while(true){
		if(num_elements == max)
		{
			full = true;
			read();
		}
		else if(num_elements == 0)
		{
			empty = true;
			write();
		}
		else{
			empty = false;
			full = false();
			read();
			write();
		}
		wait();
	}
}

void fifo::read()
{
	if(pop)
	{
		data_out = data[first];
		--num_elements;
		first = (first + 1) % max;
	}
}

void fifo::write()
{
	if(push)
	{
		data[(first + num_elements) % max] = data_in.read();
		++num_elements;
	}
}