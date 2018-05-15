#include <C:\Users\muham\CCS_workspace_v7\Term Project\queue.h>
int Temperature,total,average;			// necessary variables to get temperature and average.
void Get_Average(void)					
{
	__disable_interrupt();
	total=0;							// Set total to zero.
	temp=first;							// Hold first element of queue in a temporary pointer.
	
	while(temp!=NULL){					// Until temp is NULL...
		total+=temp->info;				// Do summation.
		temp=temp->ptr;					// Move to next node.
	}
	if(total==0){						// If no data obtained...
		average=0;						// Average and denominator are equal to zero.
		denominator=0;
	}
	else average=total/denominator;		// else obtain average.
	
	  itoa(Temperature,str);			// Transform Temperature to string.
	  strcpy(line_1,"Temp: ");			// Create first line of the LCD
	  strcat(line_1,str);				// add temperature at the end of string.
	  
	  itoa(average,str);				// Transform Average to string.
	  strcpy(line_2,"Average: ");		// Create second line of the LCD.
	  strcat(line_2,str);				// add average at the end of string.
	  
	  LCD_DataToWrite(line_1,line_2);	//Write Lines to the LCD.
}
