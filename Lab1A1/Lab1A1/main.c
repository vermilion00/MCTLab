/*
 * main.c
 *
 * Created: 11/7/2024 6:43:37 PM
 *  Author: z004e6he
 */ 

#include <xc.h>
#include <util/delay.h>
#include "lcd.h"
#include <stdio.h>

#define DEBOUNCE 10

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

void aufgabe1(void);
void aufgabe2(void);
void aufgabe3(void);
void aufgabe4(void);

uint8_t count = 0;
uint8_t currentcount = 0;
short flag = 0;
short debounce = 0;

char TransmitBuffer[20] = {0};


int main(void)
{
	//Set LED port as Output
	DDRB = 0xFF;
	//Set Switch port as Input
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
		//reset the flag
		flag = 0;
		
		//Output the count on the LEDs
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
		//reset the flag
		flag = 0;
		
		//Output the count on the LEDs
		PORTB = ~count;
		
		//Check if the count has changed
		if(currentcount != count){
			//Output the count on the Display
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
				_delay_ms(DEBOUNCE);
				debounce = 1;
				continue;
			} else {
				//only register switch once per keypress
				if(flag == 0){
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
				_delay_ms(DEBOUNCE);
				debounce = 1;
				continue;
			} else {
				//only register switch once per keypress
				if(flag == 0){
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
		
		//Output the count on the LEDs
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
				_delay_ms(DEBOUNCE);
				debounce = 1;
				continue;
			} else {
				//only register switch once per keypress
				if(flag == 0){
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
				_delay_ms(DEBOUNCE);
				debounce = 1;
				continue;
			} else {
				//only register switch once per keypress
				if(flag == 0){
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
		
		//Output the count on the LEDs
		PORTB = ~count;
		
		//Check if the count has changed
		if(currentcount != count){
			//Output the count on the Display
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
#endif
	