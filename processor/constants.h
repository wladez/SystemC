#ifndef CONSTANTS_H
#define CONSTANTS_H

#define DATA_WIDTH			32 
#define ADDR_WIDTH			10 
#define RAM_DEPTH			1 << ADDR_WIDTH
#define RON_ADDR_WIDTH		5 
#define RON_DEPTH			1 << RON_ADDR_WIDTH
#define GPIO_WIDTH			8

const int n = 4;
const int m = 5;
const int p = 6;

const int offsetA = 48;
const int offsetB = offsetA + n*m;
const int offsetQ = offsetB + m*p;

const int width = 5;
const int height = 7;
const int offsetIn = 112;
const int offsetOut = offsetIn + width*height;

#endif /* CONSTANTS_H */