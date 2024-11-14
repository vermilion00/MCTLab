#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"

void aufgabe1(void);
void aufgabe2(void);

//"Switch 2 pressed" flag set by ISR
uint8_t sw2_flag = 0;
//"Switch 3 pressed" flag set by ISR
uint8_t sw3_flag = 0;
//"Timer 1 finished" flag set by ISR
uint8_t timer_running = 0;
//Stores the counter
int count = 0;
//Stores count to check if display needs to update
int prevcount = 0;
//Stores the output message
char TransmitBuffer[20];

int main(void){
    //Set the entire Switch register as Input
	//INT0 = SW2 = PD2
	//INT1 = SW3 = PD3
    DDRD = 0x00;
	
	//Enable Interrupts globally
	sei();
	//Enable Interrupt 0 and 1
	GICR |= (1 << INT0) | (1 << INT1);
	//Set the Interrupt to check for falling edge
	MCUCR |= (1 << ISC01);
	MCUCR &= ~(1 << ISC00);
	
	//Set the LED 1 pin as Output
	DDRB = 0x01;
	
	//Initialize display
	lcd_init(LCD_DISP_ON);
	//Clear the display
	lcd_clrscr();
	//Output initial text
	lcd_puts("Count: 0");
    
    aufgabe1();
    //aufgabe2();
}

void aufgabe1(void){
	while(1){
		//Check if switch 2 has been pressed
		if(sw2_flag){
			//Reset the flag
			sw2_flag = 0;
			//Increment the counter
			count++;
		}
		//Check if switch 3 has been pressed
		if(sw3_flag){
			//Reset the flag
			sw3_flag = 0;
			//Decrement the counter
			count--;
		}
		if(count != prevcount){
			//Clear the display
			lcd_clrscr();
			//Convert the output message to char array
			sprintf(TransmitBuffer, "Count: %i", count);
			//Output the message to the display
			lcd_puts(TransmitBuffer);
			//Equalize the counters
			prevcount = count;
		}
	}
}

void aufgabe2(void){
	while(1){
		if(!timer_running){
			//Set the timer running flag
			timer_running = 1;
			//Enable Timer 1 Overflow Interrupt
			TIMSK = (1 << TOIE1);
			//Set overflow value for 500ms timer
			//31250 counts at /256 prescaler
			TCNT1 = 0x85EE; //34286
			//Start Timer 1 with /256 prescaler
			TCCR1B = (1 << CS12);
			//Deactivate Timer 1 (Check if necessary)
			TCCR1B = 0;
			//Clear Timer 1 Overflow flag
			TIFR &= ~(1 << TOV1);
		}
	}
}

//Interrupt 0 Service Routine
//IR0_vect instead?
ISR(INT0_vect){
	//Set the "Switch pressed" flag
	sw2_flag = 1;
	//Or count up directly
	//count++;
}

//Interrupt 1 Service Routine
ISR(INT1_vect){
	//Set the "Switch pressed" flag
	sw3_flag = 1;
	//Or count down directly
	//count--;
}

ISR(TIMER1_OVF_vect){
	//Toggle LED 0 each time the ISR is called
	PORTB ^= (1 << 0);
	//Reset the timer running flag
	timer_running = 0;
}

/*
Fragen:

a)
Interrupts können nur durch andere Interrupts mit einer höheren Priorität
unterbrochen werden, ansonsten wird zuerst die ISR abgearbeitet und danach
mit dem nächsten Interrupt weitergemacht. Wenn in der ISR lange Berechnungen
ablaufen, kann es dazu führen, dass wichtige Signale verzögert wargenommen
werden. Dies kann umgangen werden, indem man nur "Flags" in den ISRs setzt,
und in der Main loop den Status der Flags abfragt.

b)
Wenn der zweite Interrupt eine höhere Priorität hat, wird die erste ISR
unterbrochen und die zweite zuerst ausgeführt, und erst danach mit der
ersten ISR weitergemacht. Ansonsten wartet die zweite ISR, bis die erste
ausgeführt wurde.
*/