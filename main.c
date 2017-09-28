#include "AT91SAM7L128.h"
#include "lcd.h"
#include "keyboard.h"

//Perrin Jones, Felix Lien, Melanie Sawyer, Shyan Asson

int checkForInput(char in){ //Checks if the input key has been entered
	if (in == '\r'){
		return 1;
	}
	return 0;
}
int doOperation(char operator, int number1, int number2){ //Takes operation and performs it on the two inputed numbers
	if(operator=='*'){
		return number1*number2;
	}
	else if(operator=='/'){
		return number1/number2;
	}
	else if(operator=='-'){
		return number1-number2;
	}
	else{
		return number1+number2;
	}
}
int main()
{
  
  struct entry user_entry;
  // Disable the watchdog timer
  *AT91C_WDTC_WDMR = AT91C_WDTC_WDDIS;
int stack[4]={0};
int index = 0; //Position of next empty spot

  lcd_init();
  keyboard_init();
  int underflow = 0;
  int overflow = 0;
  //lcd_print7("PRESS");
  while(1){
	keyboard_get_entry(&user_entry);			   //Gets entry 
	if(checkForInput(user_entry.operation)==1){ //checking if input has been pressed
	  if(index==4){ 					   //catches stack overflow
		  overflow = 1;
	  }else{ 							   //adds number to stack
		  stack[index]=user_entry.number;
		  index++;
	  }
	}else{       //otherwise, an operator has been entered and an operation should be performed.
		if(index==0){                      //catches stack underflow
		  underflow = 1;
		}else{
		  index--;						  //Indication of stack pop
		  int result = doOperation(user_entry.operation, stack[index-1],stack[index]); //Takes operation and does it
		  stack[index] = 0;				  //Sets index to 0, this is where the next input will go
		  stack[index-1] = result;		  //Sets the position right before index equal to the result
		}
	}
	if(overflow) {
		lcd_print7("OVERFLOW   ");
		overflow = 0;
	} else if(underflow) {
		lcd_print7("UNDERFLOW   ");
		underflow = 0;
	} else {
		lcd_print_int(stack[index-1]);    //Prints highest number on the stack
	}
  }
  return 0;
}
