unsigned int ReceiveByte;						// Number of bytes to receive.
unsigned int TXByteCtr;							// Number of bytes to transmit.
void i2c_set(int);								// function definition.

void i2c_set(int type)
{
	if(type==1){								// type 1 represents transmission.
		UCB0CTL1 |= UCSWRST;                    // Enable SW reset
		UCB0CTL0 = UCMST+UCMODE_3+UCSYNC;       // I2C Master, synchronous mode
		UCB0CTL1 = UCSSEL_2+UCSWRST;            // Use SMCLK, keep SW reset
		UCB0BR0 = 12;                           // fSCL = SMCLK/12 = ~100kHz
		UCB0BR1 = 0;
		UCB0I2CSA = 0x20;                       // Set slave address
		UCB0CTL1 &= ~UCSWRST;                   // Clear SW reset, resume operation
		IE2 = UCB0TXIE;                         // Enable TX interrupt
		TXByteCtr=1;
	}
	else if(type==2){							// type 2 represents receiver.
	    UCB0CTL1 |= UCSWRST;					// Enable SW reset
	    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;	// I2C Master, synchronous mode
	    UCB0CTL1 = UCSSEL_2+UCSWRST;			// Use SMCLK, keep SW reset
	    UCB0BR0 = 12;							// fSCL = SMCLK/12 = ~100kHz
	    UCB0BR1 = 0;
	    UCB0I2CSA = 0x48;						// Set slave address
	    UCB0CTL1 &= ~UCSWRST;					// Clear SW reset, resume operation
	    IE2 = UCB0RXIE; 						// Enable RX interrupt
	}
}
