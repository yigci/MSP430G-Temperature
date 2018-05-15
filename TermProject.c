/*
 * DOKUZ EYLUL UNIVERSITY - Department of Electrical and Electronics engineering
 * This project is designed for EED3018 Microprocessor Systems laboratory work.
 * Last Edit: 15/05/2018- 15:42
 * Project: Thermometer with Digital Temperature Sensor and LCD using I2C interface.
 * Written by: Muhammet YÝÐCÝ and Salim ÖNDER
 * Purpose: Aim of this project is to create a thermometer system that reads temperature
 * from digital temperature sensor via I2C data interface and write it to LCD via I2C
 * data interface. Also project is required to calculate the average of last 10
 * measurements. The average is also must be shown in LCD.
 */

#include <msp430.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <C:\Users\muham\CCS_workspace_v7\Term Project\LCD_MSP430.h>
#include <C:\Users\muham\CCS_workspace_v7\Term Project\Temperature.h>


/*
 * In Main function necessary outputs, interrupts and variables are defined.
 */
void main( void )
{
	WDTCTL = WDTPW + WDTHOLD;				// Stop WDT
      P1DIR  = 0x06;						// Output ports for RS and E pins of LCD
      P1OUT &= 0x00;						// Initially low.
      P1SEL |= BIT6 + BIT7;					// I2C port selection.
      P1SEL2|= BIT6 + BIT7;					// I2C port selection.
      P2IE  |= BIT2 + BIT3;					// Port2 interrupt pins selected.
      P2IES |= BIT2 + BIT3;					// Interrupt mode selected.(falling edge)
      P2IFG &= 0X00;						// Initially clear interrupt flags.
      TACCTL0 = CCIE;						// TACCR0 interrupt enabled
      TACTL = TASSEL_2 + MC_2;				// SMCLK, contmode
      Q_LENGTH=10;							// Maximum queue length limited to 10.
      first=last=NULL;						// Empty queue condition.
      LCD_initial();						// Set initial settings for LCD.
      LCD_DataToWrite("EED3018 Term Prj","Group: C-10");	// Start message.
	  __bis_SR_register(CPUOFF + GIE);		// Enter LPM0, enable interrupts.
      while(1);
}

/*
 * Timer interrupt vector is used to exit LPM0. This prevents program to stop
 * in unexpected LPM0 condition.
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
}

/*
 * Port2 interrupts are used to get new data or clear queue memory.
 */
#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
	__disable_interrupt();						// disable interrupts.
	////////RECEIVE OPERATION//////////
	if(P2IFG==0x08){							//	If interrupt at BIT3 occurs...
		P2IFG &= 0x00;							//...clear interrupt flag.
		LCD_clear();							// clear LCD.
		i2c_set(2);								// activate I2C for receive operation.
		ReceiveByte = 2;						// Number of bytes to receive.
		Temperature=0;							// set temperature initially zero.
		UCB0CTL1 |= UCTXSTT;					// I2C start condition.
		__bis_SR_register(CPUOFF + GIE);		// Enter LPM0, enable interrupts.
	}
	///////MEMORY CLEAR OPERATION/////////
	if(P2IFG==0x04){							// If interrupt at BIT2 occurs...
		P2IFG &= 0x00;							// clear interrupt flag.
		qcount=0;								// set necessary variables to zero.
		total=0;
		average=0;
		denominator=0;
		while(first!=NULL){						// clear queue memory.
			temp=first;
			first=first->ptr;
			free(temp);							// free pointer address.
		}
		LCD_clear();							// clear LCD and send message about operation
		LCD_DataToWrite("Memory Cleared","");
		LCD_clear();
		LCD_DataToWrite("Press button to","get new data.");
	}
	P2IFG &= 0x00;								// Clear interrupt flag.
}

/*
 * This interrupt vector is used to either receive or transmit data using I2C interface.
 */
#pragma vector = USCIAB0TX_VECTOR				// I2C interrupt vector
__interrupt void USCIAB0TX_ISR(void)
{
	__disable_interrupt();
	if( IFG2 & UCB0RXIFG){						// If Receive operation...
  ReceiveByte--;								// Decrement Receivebyte counter
  if (ReceiveByte)								// if it is the first byte to be obtained
  {
    Temperature = (int)UCB0RXBUF << 8;  		// Get received byte, and left shift tempereature by 8.
    if (ReceiveByte == 1)                     	// Only one byte left to obtain then...
    	UCB0CTL1 |= UCTXSTP;                	// Generate I2C stop condition to get rest of the data.
  	  }
  	  else										// if it is the second byte to be obtained...
  	  {
  		  Temperature |= (int) UCB0RXBUF;		// get the temperature by or operation.
  		  Temperature=(Temperature>>8);			// In LM75B, first 5 bit is always zero. While sensor has...
  		  	  	  	  	  	  	  	  	  	  	//...0.125 error we need to divide decimal representation...
  		  	  	  	  	  	  	  	  	  	  	//...of binary code by 8 to get actual temperature.
  		  	  	  	  	  	  	  	  	  	  	// We left shifted data by 5 to get rid of first 5 bits and
  		  	  	  	  	  	  	  	  	  	  	// also left shifted data by 3 to divide it by 8. Total shift is 8.
  		  enq(Temperature,Q_LENGTH);			// send new temperature to enq function.
  		  Get_Average();						// call get average function
  		  IFG2 &= ~UCB0RXIFG;					// clear interrupt flag.
  		  __bic_SR_register_on_exit(CPUOFF); 	// Exit LPM0
  	  }
	}
	else if( IFG2 & UCB0TXIFG){					// If Transmissin operation...
        if (TXByteCtr==1)						// If 1 Byte left to transfer.
        {
            UCB0TXBUF = i2cData;				// Write data to shift register.
            while(!(IFG2 & UCB0TXIFG));			// Wait until all data written into shift register.
            TXByteCtr--;						// Decrement of byte counter.
        }
        else									// If no byte to transmit
        {  										//...Done with transmission
            UCB0CTL1 |= UCTXSTP;     			// Send stop condition
            IFG2 &= ~UCB0TXIFG;					// clear interrupt flag.
        }
	}
__bic_SR_register_on_exit(CPUOFF);   			// Exit LPM0
}
