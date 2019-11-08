/*
We set Date and Time in RTC IC and read the same from it and Display it on LCD
You can change Date and Time according to your own need and observe auto-updation of date and time by RTC.

DS1307 works in 100kHz mode by default.

We are using AT89C5131A in MASTER Transmit/Recieve mode ONLY !, to use the device in Slave mode refer the Datasheet.

Check the connections of SDA/SCL lines and Donot forget to give 5V to DS1307 apart from 3V battery back-up.

Students are encouraged to use this code fragment, modify, and craft out other possibilities beyond what they understand from this code.
*/

#include "at89c5131.h"
#include "math.h"
#include <stdio.h>
#include <string.h>

#define LCD_data P2 // LCD Data port

//------ TWI register Definition ---------

// Refer page 102 of the Datasheet to fill-in the values of following variables

// #define TWI_Config
// #define start_comm
#define stop_comm 0x10
#define bitrate 0x01
#define clear_TWI_start_stop_interrupt 0xC7
// #define send_Ack
#define send_Nack 0xFB
// #define slave_add
// #define read_mode
// #define write_mode

// ------ RTC Register Definition ------
// #define clear_ext_interrupt
// #define seconds
// #define minutes
// #define hours
// #define day
// #define date
// #define month
// #define year
#define SQWE 0X10

//sbit SDA = 		assign pins
//sbit SCL = 		assign pins

sbit LCD_rs = P0 ^ 0; // LCD Register Select
sbit LCD_rw = P0 ^ 1; // LCD Read/Write
sbit LCD_en = P0 ^ 2; // LCD Enable

sbit LCD_busy = P2 ^ 7; // LCD Busy Flag

//---------- LCD Function Definition ----------
void LCD_Init();
void LCD_DataWrite(char dat);
void LCD_CmdWrite(char cmd);
void LCD_StringWrite(unsigned char *str);
void LCD_Ready(void);
void sdelay(int delay);

//----------- TWI Function Definition ----------

void TWI_init(void);
void Interrupt_init(void);
void start(void);
void Stop(void);
void Ack(void);
void Nack(void);
void display_data(void);
void write_one_time(void);
void read_one_time(void);

unsigned char conf[8] = {seconds, minutes, hours, day, date, month, year, SQWE}; // for RTC registers
unsigned char mode[2] = {(slave_add | write_mode), (slave_add | read_mode)};	 // To start communication in read/write mode
unsigned char Data[7], count = 0, read, ext_int = 0, high_nibble, low_nibble;	// Data[7] array stores the data recieved from RTC
																				 // 7 Bytes corresponding to seconds,minutes,hours,day,date,month,year
char MSB_read, LSB_read, flag;
int conf_index = 0, mode_index = 0, Data_index = 0;
int clk, AM, PM;
char check;

/////////////////////////////////////////////////      Initialize Two wired Communication     //////////////////////////

void TWI_init(void)
{
	// set Data and Clock Lines
	// set the SSCON to set Pt-51 in Master Transmitter Mode
	SSCON |= bitrate; // set speed (baud rate)
}

/////////////////////////////////////////////////     Interrupt handeler for Two wired Communication     //////////////////////////

void Interrupt_subroutine(void) interrupt 8 // TWI Interrupt subroutine
{
	switch (SSCS) /********* SSCS takes different values depending upon the event that occurs ************/
	{
	case 0x08:									 /////// Start has been recieved ///////
		SSDAT = mode[mode_index++];				 // SSDAT = slave address + write mode followed by slave address + read mode
		SSCON &= clear_TWI_start_stop_interrupt; // clear start flag, stop flag and Interrupt flag
		count = 0;
		break;

	case 0x10: /////// Repeat start has been recieved ///////

		SSDAT = mode[mode_index++]; // SSDAT = write slave address + read mode
		count = 0;
		SSCON &= clear_TWI_start_stop_interrupt; //clear start flag, stop flag and Interrupt flag
		break;

	case 0x18:									 /////// Slave add + write mode Ack is recieved ///////
		SSDAT = 0X00;							 // SSDAT = write register address byte
		SSCON &= clear_TWI_start_stop_interrupt; // clear start flag, stop flag and Interrupt flag
		break;

	case 0x28: /////// Data written & Ack is recieved ///////

		if (!read) // If we start the communication in write mode
		{
			if (conf_index < 8) // Write untill all 8 bytes are written to RTC
			{
				SSDAT = conf[conf_index++];				 // SSDAT <-- write next data byte
				SSCON &= clear_TWI_start_stop_interrupt; // clear start flag, stop flag and Interrupt flag
			}
			else
			{
				SSCON &= clear_TWI_start_stop_interrupt; // clear start flag, stop flag and Interrupt flag
				SSCON |= stop_comm;
				flag = 1; // flag clear to indicate write_one_time complete
			}
		}
		else
		{
			SSCON &= clear_TWI_start_stop_interrupt; // clear start flag, stop flag and Interrupt flag
			SSCON |= stop_comm;
			flag = 1; // flag clear to indicate write portion during read is complete
		}
		break;

	case 0x40: /////// Slave add + Read mode Ack is recieved ///////

		SSCON &= clear_TWI_start_stop_interrupt; //clear start flag, stop flag and Interrupt flag
		SSCON |= send_Ack;						 // send acknowledge
		break;

	case 0x48: /////// Slave add + Read mode Nack is recieved ///////

		SSCON &= clear_TWI_start_stop_interrupt; //clear start flag, stop flag and Interrupt flag
		SSCON |= stop_comm;
		break;

	case 0x50: /////// Data Read & Ack is recieved ///////

		if (Data_index < 7)
		{
			SSCON &= clear_TWI_start_stop_interrupt; //clear start flag, stop flag and Interrupt flag
			Data[Data_index++] = SSDAT;				 // Read data into Data array
			SSCON |= send_Ack;
		}
		else
		{
			Data_index = 0;							 // Reset the Data_index after reading all 7 bytes from RTC, for next read
			SSCON &= clear_TWI_start_stop_interrupt; //clear start flag, stop flag and Interrupt flag
			SSCON |= stop_comm;
			flag = 1;
		}
		break;

	case 0x58: /////// Data Read & Nack is recieved ///////

		SSCON &= clear_TWI_start_stop_interrupt; //clear start flag, stop flag and Interrupt flag
		SSCON |= stop_comm;						 // Stop the communication
		break;

	default:
		SSCON &= clear_TWI_start_stop_interrupt; //clear start flag, stop flag and Interrupt flag
		SSCON |= stop_comm;
		break;
	}
}

/////////////////////////////////////////////////     Initialize External Interrupt and global Interrupt     //////////////////////////

void Interrupt_init(void)
{
	//  Make P3.3 as Input to receive External Interrupt
	IEN0 |= 0x84; // Enable all interrupts/Serialport Int/External Int 0
	IPL0 |= 0x04; // Set the External interrupt priority as highest
	IPH0 |= 0x04;
	// Enable TWI interrupt - Refer IEN1 register
	IPL1 = 0x00; // Set the TWI interrupt priority as second highest
	IPH1 = 0x02;
	// External interrupt falling edge triggered - Refer TCON register
}

/////////////////////////////////////////////////     External Interrupt Handler Subroutine     //////////////////////////

void Ext_interrupt_subroutine(void) interrupt 2 // Interrupt vector location for external interrupt
{
	ext_int = 1;				 // Instead of doing everything in interrupt, we set a flag and do everything in main()
	TCON &= clear_ext_interrupt; // clear external interrupt on INT1 (P3.3)
}

/////////////////////////////////////////////////     Send Start on Two Wire Interface     //////////////////////////

void start(void)
{
	//  start communication -- Refer SSCON register
}

/////////////////////////////////////////////////     LCD Initialization     //////////////////////////

void LCD_Init()
{
	LCD_CmdWrite(0x38); // LCD 2lines, 5*7 matrix
	sdelay(100);
	LCD_CmdWrite(0x0E); // Display ON cursor ON  Blinking off
	sdelay(100);
	LCD_CmdWrite(0x01); // Clear the LCD
	sdelay(100);
	LCD_CmdWrite(0x80); // Cursor to First line First Position
	sdelay(100);
}

/////////////////////////////////////////////////     LCD Busy Bit Check     //////////////////////////

void LCD_Ready()
{
	LCD_data = 0xFF;
	LCD_rs = 0;
	LCD_rw = 1;
	LCD_en = 0;
	sdelay(5);
	LCD_en = 1;
	while (LCD_busy == 1)
	{
		LCD_en = 0;
		LCD_en = 1;
	}
	LCD_en = 0;
}

/////////////////////////////////////////////////     LCD Command Write     //////////////////////////

void LCD_CmdWrite(char cmd)
{
	LCD_Ready();
	LCD_data = cmd; // Send the command to LCD
	LCD_rs = 0;		// Select the Command Register by pulling LCD_rs LOW
	LCD_rw = 0;		// Select the Write Operation  by pulling RW LOW
	LCD_en = 1;		// Send a High-to-Low Pusle at Enable Pin
	sdelay(1);
	LCD_en = 0;
	sdelay(1);
}

/////////////////////////////////////////////////     LCD Character Write     //////////////////////////

void LCD_DataWrite(char dat)
{
	LCD_Ready();
	LCD_data = dat; // Send the data to LCD
	LCD_rs = 1;		// Select the Data Register by pulling LCD_rs HIGH
	LCD_rw = 0;		// Select the Write Operation by pulling RW LOW
	LCD_en = 1;		// Send a High-to-Low Pusle at Enable Pin
	sdelay(1);
	LCD_en = 0;
	sdelay(1);
}
/////////////////////  Function to split the string into individual characters and call the LCD_DataWrite function   ////////////////

void LCD_StringWrite(unsigned char *str)
{
	int i = 0;
	while (str[i] != 0)
	{
		LCD_DataWrite(str[i]);
		i++;
	}
	return;
}

/////////////////////////////////////////////////     Delay loop     //////////////////////////

void sdelay(int delay)
{
	char d = 0;
	while (delay > 0)
	{
		for (d = 0; d < 5; d++)
			;
		delay--;
	}
}

///////////////////////////////////////////////// BCD to ASCII ////////////////////////////////////////////////////

// Since we read from DS1307(BCD numbers) and display it to LCD (ASCII Values)
void BCD_ASCII(unsigned char value)
{
	// write the function to covert the BCD values to ASCII
}

///////////////////////////////////////////////// Display date and time //////////////////////////////////////////

//Display date and time on LCD Display
void Disp_time(void)
{
	// Data register contains all data read from the slave device
	// 1ine 1-format: 	Time HH:MM:SS AM (or) Time HH:MM:SS PM [(or) Time HH:MM:SS Hr - in case of 24 hr format]
}

void Disp_date(void)
{
	// Data register contains all data read from the slave device
	// 1ine 2-format: 	Date WW DD/MM/YY
}
/////////////////////////////////////////////////     Display Data    //////////////////////////

void display_data(void)
{
	Disp_time();
	Disp_date();
}

/////////////////////////////////////////////////     Write_one_time     //////////////////////////

void write_one_time()
{
	flag = 0;
	mode_index = 0;
	conf_index = 0;
	read = 0;
	start();
	while (!flag)
		;
}
/////////////////////////////////////////////////     Read_one_time     //////////////////////////

void read_one_time()
{
	flag = 0;
	mode_index = 0;
	Data_index = 0;
	read = 1;
	start();
	while (!flag)
		;
	flag = 0;
	start();
	while (!flag)
		;
}

/////////////////////////////////////////////////     Main Programme     //////////////////////////
// Understand the main program
void main(void)
{

	P2 = 0x00; // Make Port 2 output for LCD
	// One time Initialisations
	LCD_Init();		  // Initialize LCD
	Interrupt_init(); // Initialize Interrupts for both I2C/TWI and External Interrupt INT1 connected to P3.3
	TWI_init();

	write_one_time(); // Write the Data to slave device

	LCD_CmdWrite(0x80);
	LCD_StringWrite("Time ");
	LCD_CmdWrite(0xC0);
	LCD_StringWrite("Date ");

	while (1)
	{
		if (ext_int)
		{
			ext_int = 0;
			// Read the Data from slave device
			// Display the recieved data
		}
	}
}
