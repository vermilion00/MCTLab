#include <xc.h>
#include <util/delay.h>
#include "lcd.h"
#include <stdio.h>

/* I didn't know how bad the switch chatter would be initially, so I decided to
   implement a simple debounce algorithm. Exercises 2 and 3 are duplicated, with
   the first version not having the debounce logic incorporated. */

//Set the debounce time in milliseconds, or comment out to disable debouncing   
#define DEBOUNCE 10

//Set the CPU frequency, needed for the _delay_ms function
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

void aufgabe1(void);
void aufgabe2(void);
void aufgabe3(void);
void aufgabe4(void);

//Counts the current number
uint8_t count = 0;
//Stores the count value for comparison
uint8_t currentcount = 0;
//Flag equals 1 if the switch has been pressed
short flag = 0;
//Flag equals 1 if the switch has been debounced
short debounce = 0;
//Stores the output message of the sprintf function
char TransmitBuffer[20] = {0};


int main(void)
{
	//No infinite loop, as those are included in the individual functions
	
	//Set entire LED register as Output
	DDRB = 0xFF;
	//Set entire Switch register as Input
	DDRD = 0x00;
		
	//aufgabe1();
	//aufgabe2();
	aufgabe3();
}

void aufgabe1(void){
	while(1){
		//Set the LED port to be equal to the Switch Port
		//PORT D is the Switch port
		//PORT B is the LED port
		PORTB = PIND;
	}
}

#ifndef DEBOUNCE
//No debounce enabled
void aufgabe2(void){
	while(1){
		//Check for negative edge on Sw0, since Switches are active low
		while((~PIND) & (1 << PD0)){
			//only register switch once per keypress
			if(flag == 0){
				flag = 1;
				//Increment the counter
				count++;
			}
		}
		//reset the key pressed flag
		flag = 0;
		
		//Check for negative edge on Sw7
		while((~PIND)&(1<<PD7)){
			//only register switch once per keypress
			if(flag == 0){
				flag = 1;
				//decrement the counter
				count--;
			}
		}
		//reset the key pressed flag
		flag = 0;
		
		//Output the count on the LEDs. Since the LEDs and the switches have
		//opposing logic, an inversion is needed. 
		
		PORTB = ~count;
	}
}

void aufgabe3(void){
	//Initialize the LCD with invisible cursor
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_puts("Count = 0");
	
	while(1){
		//Check for negative edge on Sw0, since Switches are active low
		while((~PIND) & (1 << PD0)){
			//only register switch once per keypress
			if(flag == 0){
				flag = 1;
				//Increment the counter
				count++;
			}
		}
		//reset the key pressed flag
		flag = 0;
		
		//Check for negative edge on Sw7
		while((~PIND) & (1 << PD7)){
			//only register switch once per keypress
			if(flag == 0){
				flag = 1;
				//decrement the counter
				count--;
			}
		}
		//reset the key pressed flag
		flag = 0;
		
		//Output the count on the LEDs. Since the LEDs and the switches have
		//opposing logic, an inversion is needed. 
		PORTB = ~count;
		
		//Check if the count has changed
		if(currentcount != count){
			//Clear screen
			lcd_clrscr();
			//Convert count to String
			sprintf(TransmitBuffer, "Count %i", count);
			//Output count on Display
			lcd_puts(TransmitBuffer);
			currentcount = count;
		}
	}
}

#else
//Debounce enabled
void aufgabe2(void){
	while(1){
		//Check for negative edge on Sw0, since Switches are active low
		while((~PIND) & (1 << PD0)){
			//Wait for 10 ms if switch hasn't been debounced yet
			if(debounce == 0){
				//Wait for the specified time
				_delay_ms(DEBOUNCE);
				//Set the "key debounced" flag
				debounce = 1;
				//Restart from the inner while loop
				continue;
			} else {
				//only register switch once per keypress
				if(flag == 0){
					//Set the "key pressed" flag
					flag = 1;
					//Increment the counter
					count++;
				}
			}
		}
		//reset the "key pressed" flag
		flag = 0;
		//reset the "key debounced" flag
		debounce = 0;
		
		//Check for negative edge on Sw7
		while((~PIND) & (1 << PD7)){
			//Wait for 10 ms if switch hasn't been debounced yet
			if(debounce == 0){
				//Wait for the specified time
				_delay_ms(DEBOUNCE);
				//Set the "key debounced" flag
				debounce = 1;
				//Restart from the inner while loop
				continue;
			} else {
				//only register switch once per keypress
				if(flag == 0){
					//Set the "key pressed" flag
					flag = 1;
					//Decrement the counter
					count--;
				}
			}
		}
		//reset the "key pressed" flag
		flag = 0;
		//reset the "key debounced" flag
		debounce = 0;
		
		//Output the count on the LEDs. Since the LEDs and the switches have
		//opposing logic, an inversion is needed. 
		PORTB = ~count;
	}
}

void aufgabe3(void){
	//Initialize the LCD with invisible cursor
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_puts("Count = 0");
	
	while(1){
		//Check for negative edge on Sw0, since Switches are active low
		while((~PIND) & (1 << PD0)){
			//Wait for 10 ms if switch hasn't been debounced yet
			if(debounce == 0){
				//Wait for the specified time
				_delay_ms(DEBOUNCE);
				//Set the "key debounced" flag
				debounce = 1;
				//Restart from the inner while loop
				continue;
			} else {
				//only register switch once per keypress
				if(flag == 0){
					//Set the "key pressed" flag
					flag = 1;
					//Increment the counter
					count++;
				}
			}
		}
		//reset the "key pressed" flag
		flag = 0;
		//reset the "key debounced" flag
		debounce = 0;
		
		//Check for negative edge on Sw7
		while((~PIND) & (1 << PD7)){
			//Wait for 10 ms if switch hasn't been debounced yet
			if(debounce == 0){
				//Wait for the specified time
				_delay_ms(DEBOUNCE);
				//Set the "key debounced" flag
				debounce = 1;
				//Restart from the inner while loop
				continue;
			} else {
				//only register switch once per keypress
				if(flag == 0){
					//Set the "key pressed" flag
					flag = 1;
					//Decrement the counter
					count--;
				}
			}
		}
		//reset the "key pressed" flag
		flag = 0;
		//reset the "key debounced" flag
		debounce = 0;
		
		//Output the count on the LEDs. Since the LEDs and the switches have
		//opposing logic, an inversion is needed. 
		PORTB = ~count;
		
		//Check if the count has changed
		if(currentcount != count){
			//Clear screen
			lcd_clrscr();
			//Convert count to String
			sprintf(TransmitBuffer, "Count = %i", count);
			//Output count on Display
			lcd_puts(TransmitBuffer);
			currentcount = count;
		}
	}
}
//End of the "Debounce activated" check
#endif