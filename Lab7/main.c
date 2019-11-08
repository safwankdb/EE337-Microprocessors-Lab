/*
EE337 lab7: ADC IC tlv1543 interfacinng with pt-51
L.W.: (1) Complete spi() function from spi.h so that this projecct will work properly 
	  (2) Check whether it is working or not
	  (3) Edit adc() function to make it genralized.
*/

#include <at89c5131.h>
#include "lcd.h"											// Driver for interfacing lcd 
#include "tlv1543.h"										// Driver for interfacing ADC ic tlv1543


char adc_ip_data_ascii[6]={0,0,0,0,0,'\0'};					// String array for saving ascii of input sample
char display_msg1[]="ADC channel-";						// Display msg on 1st line of lcd
char display_msg2[]=" mV";									// Display msg on 2nd line of lcd
char adc_id[3]={0,0,'\0'   };	

void main(void)
{
	int j=10;
	unsigned int adc_data=0,dac_data=0;
	

	spi_init();
	adc_init();
	lcd_init();
	while(j<15)
	{
		int_to_string(j,adc_id);


		adc_data=adc(j*4096);
		
		adc_data= (unsigned int)(3.225806452*adc_data) ;	// Converting to milli volt (3.3*1000*i/p / 1023)
	
		int_to_string(adc_data-1,adc_ip_data_ascii);			// Convering integer to string of ascii
		lcd_cmd(0x80);											// Move cursor to first line
		lcd_write_string(display_msg1);							// Display "ADC channel-00:" on first line of lcd
		lcd_write_string(adc_id);	
		lcd_write_string(":");	
		lcd_cmd(0xC0);										// Move cursor to 2nd line
		lcd_write_string(adc_ip_data_ascii);				// Print analog sampled input on lcd
		lcd_write_string(display_msg2);						// Display "XXXXX mV" on 2nd line of lcd
		msdelay(2000);
		j++;
	}
}

