#include "at89c5131.h"
#include "serial.c"




void main(void)
{
   uart_init();					//Call function to initialize UART
   while(1)   
	 {
     		//Call function to transmit 'A'
			//transmit_char(unsigned char ch)
		transmit_char('a');
		msdelay(100);
		//Call 100 ms delay function
   }		 
}


