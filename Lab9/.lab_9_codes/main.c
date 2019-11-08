#include <at89c5131.h>
#include <lcd.h>
#include <serial.c>

sbit LED=P1^7;
sbit LED6=P1^6;
sbit LED5=P1^5;
sbit LED4=P1^4;
sbit s3=P1^3;
sbit s2=P1^2;
sbit s1=P1^1;
sbit s0=P1^0;

//char s1,s2,s3,s4;

unsigned char intr_count = 0,rx_buf = 0;
//bit tx_complete = 0;
//bit rx_complete = 0;

void timer_ISR(void) interrupt 1
{
		intr_count++;
	  LED = ~LED;
}	

void gpio_test(void)
{
// Write your testing code here.
	
	LED = s3;
	LED6 = s2;
	LED5 = s1;
	LED4 = s0;
	
	msdelay(2000);
	LED = 0;
	LED6 = 0;
	LED5 = 0;
	LED4 = 0;
}	

void led_test(void)
{
// Write your testing code here.	
	lcd_init();
	lcd_write_string("starting LED");
	LED = 1;
	LED6 = 1;
	LED5 = 1;
	LED4 = 1;
	msdelay(2000);
	LED = 0;
	LED6 = 0;
	LED5 = 0;
	LED4 = 0;
	lcd_init();
	lcd_write_string("all LED tested");
}

void lcd_test(void)
{
// Write your testing code here.
	lcd_init();
	lcd_write_string("LCD is tested");
}	

void timer_test(void)
{
	while(!rx_complete){
	LED =1;
	msdelay(500);
	LED = 0;
	msdelay(500);
	}
}

void main(void)
{
	unsigned char ch=0;
	P1 = 0x0F;
	lcd_init();
	uart_init();
	transmit_string("************************\r\n");
	transmit_string("******8051 Tests********\r\n");
	transmit_string("************************\r\n");
	transmit_string("Press 1 for GPIO test\r\n");
	transmit_string("Press 2 for LED test\r\n");
	transmit_string("Press 3 for LCD test\r\n");

	lcd_write_string("doing something");
	while(1)
	{
			ch = receive_char();
			switch(ch)
			{
				case '1':gpio_test();
								 transmit_string("GPIO tested\r\n");
								 break;
				case '2':transmit_string("starting LED\r\n");
								 led_test();
								 transmit_string("LED tested for 2 secs\r\n");
								 break;
				case '3':lcd_test();
								 transmit_string("LCD tested\r\n");
								 break;
				case '4':timer_test();
								 transmit_string("Timer tested\r\n");
								 break;
								
				default:transmit_string("Incorrect test.Pass correct number\r\n");
								 break;
				
			}		
	}
}
