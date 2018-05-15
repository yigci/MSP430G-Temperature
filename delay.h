void delay(int);				// function def.
void delay(int i)
{
	__disable_interrupt();		// disable interrupt while in a delay loop.
	int k,m;					// variables. m is used as multiplier of k.
	for(m=i;m>0;m--){
		for(k=1000;k>0;k--){	//	k is set to 1000 each delay cycle.
			P1OUT ^= 0x01;		// toggle led. (purpose is to be aware of
								// program is in delay function. 
		}
	}
}
