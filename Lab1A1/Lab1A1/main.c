/*
 * main.c
 *
 * Created: 11/7/2024 6:43:37 PM
 *  Author: z004e6he
 */ 

#include <xc.h>

void aufgabe1(void);
void aufgabe2(void);
void aufgabe3(void);

short count = 0;
short flag0 = 0;
short flag7 = 0;


int main(void)
{
	/* TODO: Change PORT to the actual ports that the LEDs and Switches are connected to */
	//Set LED port as Output
	DDRB = 0xFF;
	//Set Switch port as Input
	DDRD = 0x00;
		
	aufgabe1();
	//aufgabe2();
	//aufgabe3();
}

void aufgabe1(void){
	while(1){
		//Set the LED port to be equal to the Switch Port
		/*TODO: Check if the Switch is active low and LED is active high (should be) */
		//PORT D is the Switch port
		//PORT B is the LED port
		PORTB = ~PIND;
	}
}

void aufgabe2(void){
	while(1){
		/*TODO: Implement debouncing algorithm? Just wait for 5-20 ms after registering a negative flank before actually reading it*/
		//Check for negative edge on Sw0, since Switches are active low
		while((~PIND)&(1<<PD0)){
			//only register switch once per keypress
			if(flag0 == 0){
				flag0 = 1;
				//Increment the counter
				count++;
			}
		}
		//reset the key pressed flag
		flag0 = 0;
		
		//Check for negative edge on Sw7
		while((~PIND)&(1<<PD7)){
			//only register switch once per keypress
			if(flag7 == 0){
				flag7 = 1;
				//decrement the counter
				count--;
			}
		}
		//reset the flag
		flag7 = 0;
		
		//Output the count on the LEDs
		/*TODO: Check if inversion is needed */
		PORTB = ~count;
	}
}

void aufgabe3(void){
	while(1){
		
	}
}