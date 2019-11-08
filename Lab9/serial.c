#include <at89c5131.h>

bit tx_complete = 0,rx_complete = 0; //Bit flags for interrupts

//Function for <time> ms delay. 
void delay_ms(unsigned int time)
{
	int i,j;
	for(i=0;i<time;i++)
	{
		for(j=0;j<382;j++);
	}
}

//Function for initializing the UART
void uart_init(void)
{
	EA=1;
	ES=1;
	TMOD=0x20;				//Configure Timer 1 in Mode 2
	TH1=-52;				//Load TH1 to obtain require Baudrate (Refer Serial.pdf for calculations)
	SCON=0x50;				//Configure UART peripheral for 8-bit data transfer 
	TR1 = 1;				//Start Timer 1
						//Enable Serial Interrupt
						//Enable Global Interrupt
}


//Function to transmit 1 character through UART
void transmit_char(unsigned char ch)
{
	SBUF=ch;				//Load data in SBUF
	while(!tx_complete); 			//Wait for tx_complete flag (interrupt to complete)
	tx_complete = 0;			//Clear tx_complete flag 
}

//Function to transmit string through UART
void transmit_string(unsigned char *s)
{
	while(*s != 0)
	{
			transmit_char(*s++);
			while(TI!=0);
	}
}

unsigned char receive_char(void)
{
	unsigned char ch = 0;
	while(!rx_complete);				//Wait for rx_complete(interrupt to complete)
	rx_complete = 0;
	ch = SBUF;					//Read data from SBUF
	return ch;					//Return read character
}

//ISR for Serial Interrupt
void serial_ISR(void) interrupt 4
{
		if(TI==1)			//check whether TI is set
		{
			TI = 0;			//Clear TI flag
			tx_complete = 1;	//Set tx_complete flag indcating interrupt completion
		}
		else if(RI==1)			//check whether RI is set
		{
			RI = 0;			//Clear RI flag
			rx_complete = 1;	//Set rx_complete flag indcating interrupt completion
		}
}
