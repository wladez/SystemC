/* 
 * File:   fifoparamextend.h
 * Author: student
 *
 * Created on March 17, 2017, 11:56 AM
 */
#include <systemc.h>
#include "fifoparam.h"

#ifndef FIFOPARAMEXTEND_H
#define	FIFOPARAMEXTEND_H

template <typename T>
struct fifoparamextend : public fifoparam<T> {
public:
	sc_out<bool> almost_empty;
	sc_out<bool> almost_full;

	unsigned int almost;
	//void processing();
	void almostfe();

	SC_HAS_PROCESS(fifoparamextend);

	fifoparamextend(sc_module_name name, int _size, unsigned int _almost):
	fifoparam<T>(name, _size),
		almost(_almost)
	{
		cout << "Executing new" << endl;
		//SC_CTHREAD(almostfe, fifoparam<T>::clk.pos());
		SC_METHOD(almostfe);
		sc_module::sensitive << fifoparam<T>::is_write << is_read << clk;            
	}
};

template <typename T>
void fifoparamextend<T>::almostfe()
{
	//almost_empty = true;
	//almost_full = false;
	//wait();
	//while(true){
	if(fifoparam<T>::num_elements <= almost)
		almost_empty = true;
	else almost_empty = false;
	if(fifoparam<T>::num_elements >= fifoparam<T>::max - almost)
		almost_full = true;
	else almost_full = false;
	//	wait();
	//}
}

//template <typename T>
//void fifoparamextend<T>::processing(){
//    empty = true;
//	full = false;
//        almost_empty = true;
//        almost_full = false;
//	data_out = 0;
//	first = 0;
//	num_elements = 0;
//	for(int i = 0; i<=max; i++)
//		data[i] = 0;
//	wait();
//	while(true){
//		if(num_elements == max)
//		{
//			full = true;
//			read();
//		}
//		else if(num_elements == 0)
//		{
//			empty = true;
//			write();
//		}
//		else{
//                        if(num_elements <= almost)
//                            almost_empty = true;
//                        else almost_empty = false;
//                        if(num_elements >= max - almost)
//                            almost_full = true;
//                        else almost_full = false;
//                        empty = false;
//			full = false;
//			read();
//			write();
//		}
//		wait();
//	}
//}
#endif	/* FIFOPARAMEXTEND_H */

