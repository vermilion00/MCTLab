//Set the CPU frequency, needed for the _delay_ms function
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

//#include <xc.h>
#include "lcd.h"
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void aufgabe1(void);
void aufgabe2(void);
void aufgabe3(void);

#define CONV_AMNT1 10
#define CONV_AMNT2 50

//Toggle the sliding window average
//#define SLIDING_WINDOW

//"Key Pressed" flag
short flag = 0;
//Sum of ADC conversion values
unsigned int val = 0;
//Sum of ADC1 conversion values
unsigned long int val1 = 0;
//Converted voltage value
unsigned long int voltage = 0;
//Converted second voltage value
unsigned long int voltage1 = 0;
//Buffer to store the output string
char TransmitBuffer[60];
//Goes to 1 if 50 samples have been converted
short startOutput = 0;
//Array of the last 50 ADC values
int values[CONV_AMNT2];

int main(void)
{
	//Set Switch register to Input
	DDRD = 0x00;
	
	//Enable the ADC (without starting conversion)
	ADCSRA |= (1 << ADEN);
	//Set ADC Ref voltage to AVCC
	ADMUX |= (1 << REFS0);
	//Set the prescaler to 128
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	//Enable interrupts
	sei();
	//Enable Interrupt 0 and 1
	GICR |= (1 << INT0) | (1 << INT1);
	//Set Interrupt 0 to check for falling edge
	MCUCR |= (1 << ISC01);
	MCUCR &= ~(1 << ISC00);
	//Set Interrupt 1 to check for falling edge
	MCUCR |= (1 << ISC11);
	MCUCR &= ~(1 << ISC10);
	
	//Initialize the Display
	lcd_init(LCD_DISP_ON);
	//Clear the screen just in case
	lcd_clrscr();
	//Output initial text
	lcd_puts("Press a button \nto start conversion");
	
	//aufgabe1();
	//aufgabe2();
	aufgabe3();
}

void aufgabe1(void){
	while(1){
		//only register switch once per keypress
		if(flag == 1){
			flag = 0;
				
			//Measure the ADC voltage 10 times
			for(int i = 0; i < CONV_AMNT1; i++){
				//Start the ADC conversion
				ADCSRA |= (1 << ADSC);
				//Wait until the conversion is finished
				while(ADCSRA & (1 << ADSC));
				//Add the converted value to the previous values
				val += ADC;
			}
			//Divide the sum by the amount of conversions
			val /= CONV_AMNT1;
			//Convert the value back to a voltage
			//10 bit means 5V = 1023
			voltage = (val * 10 * 5) / 1023;
			//Clear LCD screen
			lcd_clrscr();
			//Convert output message to char array:
			sprintf(TransmitBuffer, "Voltage: %lu.%luV\nValue: %u", 
			voltage/10, voltage%10, val);
			//Output message to display
			lcd_puts(TransmitBuffer);
		}
	}	
	//Reset the value
	val = 0;
}

//Sliding window has not been tested!
#ifdef SLIDING_WINDOW
void aufgabe2(void){
	int i = 0;
	while(1){
		//Start conversion
		ADCSRA |= (1 << ADSC);
		//Wait until conversion is finished
		while(ADCSRA & (1 << ADSC));
		//Add the converted value to the previous values
		val += ADC;
		//Also save the converted value in the array
		values[i] = ADC;
		//Check if end of array hasn't been reached
		if(i < (CONV_AMNT2 - 1)){
			//Subtract the oldest value in the array
			val -= values[i + 1];
			//If the end has been reached
			} else {
			//Reset the index
			i = 0;
			val -= values[0];
			//Enable the LCD output
			startOutput = 1;
		}
		//Increment the index variable
		i++;
		//Only output to display if 50 conversions have been done
		if(startOutput){
			//Clear display
			lcd_clrscr();
			//Calculate average value
			voltage = ((val * 5 * 10) / CONV_AMNT2) / 1023;
			//Convert output message to char array
			sprintf(TransmitBuffer, "Voltage: %lu.%luV", voltage/10, voltage%10);
			//Output to display
			lcd_puts(TransmitBuffer);
			//(Optional) delay
			_delay_ms(50);
		}
	}
}

#else
//No sliding window
void aufgabe2(void){
	while(1){
		for(int i = 0; i < CONV_AMNT2; i++){
			//Start conversion
			ADCSRA |= (1 << ADSC);
			//Wait until conversion is finished
			while(ADCSRA & (1 << ADSC));
			//Add the converted value to the previous values
			val += ADC;
		}
		//Clear display
		lcd_clrscr();
		//Calculate average value and convert to voltage
		voltage = ((val / CONV_AMNT2) * 10 * 5) / 1023;
		//Convert output message to char array
		sprintf(TransmitBuffer, "Voltage: %lu.%luV", voltage/10, voltage%10);
		//Output to display
		lcd_puts(TransmitBuffer);
		//Optional delay
		_delay_ms(50);
		
		//Reset the ADC value
		val = 0;
	}
}

#endif

void aufgabe3(void){
	while(1){
		//Change ADC channel to ADC0
		ADMUX &= ~(1 << MUX0);
		for(int i = 0; i < CONV_AMNT2; i++){
			//Start conversion
			ADCSRA |= (1 << ADSC);
			//Wait until conversion is finished
			while(ADCSRA & (1 << ADSC));
			//Add the converted value to the previous values
			val += ADC;
		}
		
		//Change ADC channel to ADC1
		ADMUX |= (1 << MUX0);
		for(int i = 0; i < CONV_AMNT2; i++){
			//Start conversion
			ADCSRA |= (1 << ADSC);
			//Wait until conversion is finished
			while(ADCSRA & (1 << ADSC));
			//Add the converted value to the previous values
			val1 += ADC;
		}
		
		//Clear display
		lcd_clrscr();
		//Calculate average value and convert to voltage
		voltage = ((val / CONV_AMNT2) * 10 * 5) / 1023;
		//Calculate second average value and voltage
		voltage1 = ((val1 * 100 / CONV_AMNT2) * 10 * 5) / 1023;
		//Convert output message to char array
		sprintf(TransmitBuffer, "Pin A0: %lu.%luV\nTemp: %lu.%luC", 
		voltage/10, voltage%10, voltage1/10, voltage1%10);
		//Output to display
		lcd_puts(TransmitBuffer);
		//(Optional) delay
		_delay_ms(50);
		
		//Reset the ADC value sums
		val = 0;
		val1 = 0;
	}
}

ISR(INT0_vect){
	flag = 1;
}

ISR(INT1_vect){
	flag = 1;
}

/*
Fragen:

a)
Ja, wenn die Batteriespannung als Referenzspannung des ADCs verwendet
wird oder die Referenzspannung von der Batteriespannung abh�ngt
(z.B. durch einen Spannungsteiler).
Da die Batteriespannung mit dem Ladestand sinkt, und die Referenzspannung
die obere Grenze des Messbereichs bestimmt, w�rde der ADC bei der selben
anliegenden Spannung einen h�heren Wert zur�ckliefern, je niedriger der
Ladestand der Batterie ist.

b)
Der Atmega32A hat einen ADC mit einem angeschlossenen Analogen 8-Kanal 
Multiplexer, sodass die Spannung an 8 verschiedenen Pins gemessen wird.

c)
F�r bestimmte Zwecke. Die ADCs in Mikrocontrollern sind n�tzlich f�r
viele Aufgaben, allerdings k�nnen f�r eine bestimmte Aufgabe z.B.
eine h�here Aufl�sung, mehr Kan�le, eine h�here Konvertierungsrate,
weniger Interferenz etc. ben�tigt werden. Auch kann bei manchen
Microcontrollern die Referenzspannung nicht angepasst werden, da
kein VREF-Pin ausgebrochen wird, beispielsweise beim
STM32F411CEU6 im 64-Pin-Format. Falls in so einem Fall z.B. ein
Spannungsbereich von 0-1V gemessen wird, w�rden 4/5 der Aufl�sung
verschwendet werden.
*/