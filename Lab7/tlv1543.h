//This is header file for 10 bit spi adc tlv1543 ic  

//include spi.h for using fns from it. Preprocessor directives are used for guarding against multiple inclusion of header files, read compliler user guide for more information

#ifndef spi_h														//if identifier spi_h is not defined then goto next line
#include "spi.h"													//Include spi driver file, Driver for uC 8051 spi module
#endif																//end if directive

//Declaration of functions which are contained in this header file

void adc_init(void);												//Initilize slave adc ic tlv1543 
unsigned int adc(unsigned int);												//gives command to adc ic to sample channel no 1 and returns 2 bytes of which lower 10 bits are adc value.

//Signal to dac ic tlv1543

sbit cs_bar_adc = P1^4;												// Chip Select for the ADC

//Function definition

void adc_init(void)
{
	cs_bar_adc=1;
}

// unsigned int adc(void)
// {
// 		unsigned int temp_adc_data;

// 																								//Configure here uC SFRs which are specific to tlv1543
	
// 		cs_bar_adc = 0;                 						//Enable adc ic tlv1543
// 		temp_adc_data = spi_trx_16_bit(0x0000);			//sending address of channel no A0 of adc ic
// 		cs_bar_adc = 1;    													//Disable adc ic tlv1543

// 		temp_adc_data=temp_adc_data>>6;							//Right shifting by 6 bit to insure data in a correct format
				
// 	  return temp_adc_data;
// }
unsigned int adc(unsigned int n)
{
		unsigned int temp_adc_data;

																								//Configure here uC SFRs which are specific to tlv1543
	
		cs_bar_adc = 0;                 						//Enable adc ic tlv1543
		temp_adc_data = spi_trx_16_bit(n);			//sending address of channel no A0 of adc ic
		cs_bar_adc = 1;    													//Disable adc ic tlv1543

		temp_adc_data=temp_adc_data>>6;							//Right shifting by 6 bit to insure data in a correct format
				
	  return temp_adc_data;
}
