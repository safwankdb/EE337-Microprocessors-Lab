//Functions contained in this header file

void msdelay(unsigned int);																	//fn takes integer value as an input and generates corresponding delay in milli seconds
void lcd_init(void);																				//Initialize LCD
void lcd_cmd(unsigned int i);																//Sends commands to lcd
void lcd_char(unsigned char ch);														//display character on a lcd corresponding to input ascii
void lcd_write_string(unsigned char *s);										//takes pointer of a string which ends with null and display on a lcd 
void int_to_string(unsigned int,unsigned char *temp_string);//convert unsigned int to string of corresponding decimal value 

//Signals to LCD

sbit RS=P0^0;	//Register select
sbit RW=P0^1;	//Read from or write to register
sbit EN=P0^2;	//Enable pin of lcd

//Function defination

void lcd_init(void)
{
	P2=0x00;
	EN=0;
	RS=0;
	RW=0;
	
	lcd_cmd(0x38);
	msdelay(4);
	lcd_cmd(0x06);
	msdelay(4);
	lcd_cmd(0x0C);
	msdelay(4);
	lcd_cmd(0x01);
	msdelay(4);
	lcd_cmd(0x80);
}

void msdelay(unsigned int time)
{
	int i,j;
	for(i=0;i<time;i++)
	{
		for(j=0;j<382;j++);
	}
}

void int_to_string(unsigned int val,unsigned char *temp_str_data)
{	
   // char str_data[4]=0;
		temp_str_data[0]=48+(val/10000);
	  temp_str_data[1]=48+(val%10000/1000);
	  temp_str_data[2]=48+((val%1000)/100);
	  temp_str_data[3]=48+((val%100)/10);
	  temp_str_data[4]=48+(val%10);
   // return str_data;
}

void lcd_cmd(unsigned int i)
{
	RS=0;
	RW=0;
	EN=1;
	P2=i;
	msdelay(10);
	EN=0;
}
void lcd_write_char(unsigned char ch)
{
	RS=1;
	RW=0;
	EN=1;
	P2=ch;
	msdelay(10);
	EN=0;
}

	
void lcd_write_string(unsigned char *s)
{
	while(*s!='\0')
	{
		lcd_write_char(*s++);
	}
}
