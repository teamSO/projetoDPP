#include "lpc2103.h"


int main(){
	
	
	int p = 0x0;
	int r = 0x0;
	int l = 0x4;
	
	__asm
	{
			
			SWP p, l, [r]
			
	}
	

}