#include "lpc2103.h"

#define locked 1


struct pt_sem {
  unsigned int count;
};


void muda_mutex(int *mutex, int *valor) {
  

  __asm {
				LDR    	r0, [mutex]
        LDR     r1, [valor]
        SWP 		r1, r1, [r0]
  
	}
}

void muda_mutex_sucess(int *mutex) {
  int r9 = *mutex + 0x1;

  __asm {
			
        SWP 		r9, r9, [mutex]
	}
}

void sem_inc(unsigned int *mutex) {
  unsigned int r0 = *mutex + 0x1;

  __asm {
			
        SWP 		r0, r0, [mutex]
	}
}

void sem_dec(unsigned int *mutex) {
  unsigned int r0 = *mutex - 0x1;

  __asm {
			
        SWP 		r0, r0, [mutex]
	}
}



int main(){
	
	static struct pt_sem mu; 
	
	(&mu)->count = 0x4;
	
	sem_dec(&((&mu)->count));
	

}
