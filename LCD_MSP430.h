#include <C:\Users\muham\CCS_workspace_v7\Term Project\delay.h>
#include <C:\Users\muham\CCS_workspace_v7\Term Project\i2c_settings.h>

////////////////////// FUNCTION DEFINITIONS ////////////////////////
void LCD_clear(void);
void LCD_initial(void);
void LCD_set_Point(unsigned int, unsigned int);
void LCD_DataToWrite (char*,char*);
void LCD_sentinfo(char, int);
char* itoa(unsigned int,char*);

/////////////////////// GLOBAL VARIABLES ///////////////////////////
int i2cData;
char str[3];
char line_1[16];
char line_2[16];

/********************* Initial settings for LCD *******************/
// Function codes for LCD obtained from function table of LCD.
void LCD_initial(void)
{
    LCD_sentinfo('c', 0x38);           // "00101000" 8 pin, 2 line, 5x7 character
    LCD_sentinfo('c', 0x02);           // "00000010" move to beginning
    LCD_sentinfo('c', 0x01);           // "00000001" LCD clear
    LCD_sentinfo('c', 0x0C);           // "00001000" hide cursor
    LCD_sentinfo('c', 0x06);           // "00000110" move cursor to right>>1
}

/*
 * LCD_DataToWrite function is an interface between LCD and data send function.
 * It has to input variables which are line1 and line2 strings. It uses LCD_sentinfo
 * function to send each character inside the string.
 */

void LCD_DataToWrite (char* row_1,char* row_2)
{
    unsigned int i = 0;					//variable to track current character to send.
    LCD_set_Point(1, 1);				//set position for first line.
    while(row_1[i]){					//loop for sending out first line.
    	LCD_sentinfo('d', row_1[i++]);	//send current character to LCD_sentinfo func.
    }
	i=0;
	
	LCD_set_Point(2, 1);				//set position for second line.
	while(row_2[i]){					//loop for sending out first line.
		LCD_sentinfo('d', row_2[i++]);	//send current character to LCD_sentinfo func.
	}
}

/*
 * LCD_sentinfo function is the interface between LCD and program. Each function call
   it sends out data.
 
 * LCD has two different inputs. One is data and the other is command. To recognize
   which kind of info is going to be sent we used an additional variable. 'c' means
   command and 'd' means data.
  
 * To send out data to I2C module of LCD we used microcontroller as Master Transmitter.
 */
void LCD_sentinfo(char type, int data)
{
	i2c_set(1);								//Master transmitter is selected.
    if(type == 'c')	P1OUT &= ~BIT1;			//Check if the info is command
    else if(type == 'd')	P1OUT |= BIT1;	//..else data.

    	i2cData = data;						//Global variable to hold current data.
        UCB0CTL1 |= UCTR + UCTXSTT;         // I2C TX, start condition
        __bis_SR_register(CPUOFF + GIE);    // CPU off, interrupts enabled
    	P1OUT |= BIT2;
    	P1OUT &= ~BIT2;						//Falling edge operation for LCD.
}

/*
 * LCD_set_Point function is used to set position of cursor.
 * It's purpose in our project to differentiate lines.
 * 0x80 means beginning of the first line
 * 0xC0 means beginning of the second line
 * Any addition to these numbers results position change in column.
 */

void LCD_set_Point(unsigned int row, unsigned int column)
{
  if(row == 1)
  {
    column--;						  //column 16 is represented as 15 in binary.
    LCD_sentinfo('c', 0x80 + column); //command to select first line. from command table
  }

  else if(row == 2)
  {
    column--;						  //column 16 is represented as 15 in binary.
    LCD_sentinfo('c', 0xC0 + column); //command to select second line.
  }
}

/*
 * LCD_clear functions is used to clear all data written into the LCD.
 * Command number obtained from function table.
 */

void LCD_clear(void)
{
  LCD_sentinfo('c', 0x01);			//Clear Command.
  LCD_set_Point(1, 1);				//set position Row=1 and Column=1.
}

char* itoa(unsigned int i, char b[])
{									//from stackoverflow
    char const digit[] = "0123456789";
    char* p = b;
    /*if(i<0){			//if temperature is below 0, then this part should be active
        *p++ = '-';		//do not forget to add minus to LCD output
        i *= -1;		//a global variable could help with that. (ie. isMinus)
    }*/
    int shifter = i;
    do{					//Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{					//Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}
