#include <stdlib.h>

#include <stm32f10x.h>

void exit(int errno) {
	//__WFE();
	while(1);
}
