//This header file contains function related uC 8051 spi module. It have fns to initialize spi module and to communicate with spi slave 

//preprocessors directives for guarding against multiple inclusion of header files, read compliler user guide for more information

#ifndef spi_h											//if identifier spi_h is not defined then goto next line
#define spi_h 1											//Define spi_h identifier and assign constant 1 to it 
#endif													//end if directive

//Declaration of functions which are contained in this header file

void spi_init(void);									//Initilize spi module 
unsigned int spi_trx_16_bit(unsigned int spi_data_tx);	//Takes 2 bytes of input and send it to spi slave ic and returns 2 bytes which are recived from slave
void spi_interrupt(void);								//SPI ISR


//Variable declaration

bit transmit_completed= 0;				// To check if spi data transmit is complete
unsigned char temp_spi_data;			// Temporary variable

//Function definition

void spi_init(void)
{
	//SPCON=0x__ is not a bit addressable sfr
	//Master mode
	SPCON |= 0x10;
	//Disable SSBAR ,P1.1(SSBAR) is available as standard I/O pin 
	SPCON |= 0x20;
	//Fclk/16 & Fclk Periph=12MHz ,thus BAUD RATE=750KHz	             	 	 
	SPCON |= 0x03;
	SPCON &= 0x7F;
	//Configure clock polarity, Refer timing diagram of uC and ADC
	//Configure clock phase, Refer timing diagram of uC and ADC
	SPCON &= 0xF3;
	//enable spi interrupt IEN1 |= 0x__
	IEN1 |= 0x04;
	//enable interrupts EA=_; 
	IEN0 |= 0x80;
	//Enable spi module 
	SPCON |= 0x40;
}

unsigned int spi_trx_16_bit(unsigned int spi_data_tx)
{
	//declaration of local variables 
	unsigned int spi_data_rx;
	unsigned char spi_data_high,spi_data_low;
	
	//Send upper byte
		transmit_completed=0;
		SPDAT= (spi_data_tx>>8);													// Right shifting by 8 bits and sending upper byte to spi slave
		while(!transmit_completed);													// Wait till transmission is complete. 	
		spi_data_high	=	temp_spi_data ;											// Save upper byte received from SPI slave
	
	//Send lower byte
		transmit_completed=0;
		SPDAT = (spi_data_tx%256);													//Extracting lower 8 bits and sending to spi slave
		while(!transmit_completed);													// Wait till transmission is complete.
		spi_data_low	=	temp_spi_data ;											//Save lower byte received from SPI slave
	
		spi_data_rx= (spi_data_high<<8) + spi_data_low;								//Concatenate both byte to form a word
	
		return spi_data_rx;															//Concatenate both byte & return it	
}

void spi_interrupt(void) interrupt 9 
/* interrupt address is 0x004B, (Address -3)/8 = interrupt no.*/
{
	
	switch	( SPSTA )         														// read and clear spi status register 
	{
		case 0x80:																	//SPIF is set i.e. SP data transfer is complete 
			temp_spi_data=SPDAT;  													//Read received data 
			transmit_completed=1;													//Set transmission complete flag 
 		break;

		case 0x10:
			
		break;
	
		case 0x40:
		break;
	}
}