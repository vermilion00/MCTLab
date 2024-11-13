#include <xc.h>
#include "lcd.h"
#include <stdio.h>
#include <util/delay.h>

//Set the CPU frequency, needed for the _delay_ms function
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void aufgabe1(void);
void aufgabe2(void);
void aufgabe3(void);

#define CONV_AMNT1 10
#define CONV_AMNT2 50

//Toggle the sliding window average
#define SLIDING_WINDOW

//"Key Pressed" flag
short flag = 0;
//Sum of ADC conversion values
int val = 0;
//Sum of ADC1 conversion values
int val1 = 0;
//Converted voltage value
float voltage = 0;
//Converted second voltage value
float voltage1 = 0;
//Buffer to store the output string
char TransmitBuffer[60];
//Goes to 1 if 50 samples have been converted
short startOutput = 0;
//Array of the last 50 ADC values
int values[CONV_AMNT2];

int main(void)
{
	//Set Switch register to Input
	DDRB = 0xFF;
	
	//Enable the ADC (without starting conversion)
	ADCSRA |= (1 << ADEN);
	//Set ADC Ref voltage to AVCC
	ADMUX |= (1 << REFS0);
	
	//Initialize the Display
	lcd_init(LCD_DISP_ON);
	//Clear the screen just in case
	lcd_clrscr();
	//Output initial text
	lcd_puts("Press a button to start conversion");
	
	aufgabe1();
	//aufgabe2();
	//aufgabe3();
}

void aufgabe1(void){
	while(1){
		//Any button is allowed
		while(~PIND){
			//only register switch once per keypress
			if(flag == 0){
				flag = 1;
				
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
				voltage = (val / 1023) * 5;
				//Clear LCD screen
				lcd_clrscr();
				//Convert output message to char array:
				sprintf(TransmitBuffer, "The voltage is %.2fV\r\nThe value is %i", 
				voltage, val);
				//Output message to display
				lcd_puts(TransmitBuffer);
			}
		}
		//Reset the key pressed flag
		flag = 0;
		
		//Reset the value
		val = 0;
	}
}

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
			voltage = ((val / CONV_AMNT2) / 1023.0) * 5;
			//Convert output message to char array
			sprintf(TransmitBuffer, "Voltage: %.2fV", voltage);
			//Output to display
			lcd_puts(TransmitBuffer);
			//Optional delay
			//_delay_ms(16);
		}
	}
}

#else
//No sliding window
void aufgabe2(void){
	while(1){
		for(int i = 0; i < 50; i++){
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
		voltage = ((val / CONV_AMNT2) / 1023.0) * 5;
		//Convert output message to char array
		sprintf(TransmitBuffer, "Voltage: %.2fV", voltage);
		//Output to display
		lcd_puts(TransmitBuffer);
		//Optional delay
		//_delay_ms(16);
		
		//Reset the ADC value
		val = 0;
	}
}

#endif

void aufgabe3(void){
	while(1){
		//Change ADC channel to ADC0
		ADMUX &= ~(1 << MUX0);
		for(int i = 0; i < 50; i++){
			//Start conversion
			ADCSRA |= (1 << ADSC);
			//Wait until conversion is finished
			while(ADCSRA & (1 << ADSC));
			//Add the converted value to the previous values
			val += ADC;
		}
		
		//Change ADC channel to ADC1
		ADMUX |= (1 << MUX0);
		for(int i = 0; i < 50; i++){
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
		voltage = ((val / CONV_AMNT2) / 1023.0) * 5;
		//Calculate second average value and voltage
		voltage1 = ((val1 / CONV_AMNT2) / 1023.0) * 5;
		//Convert output message to char array
		sprintf(TransmitBuffer, "Pin A0: %.2fV\r\nPin A1: %.2fV\r\nTemp: %.1fC", 
		voltage, voltage1, voltage1/10.0);
		//Output to display
		lcd_puts(TransmitBuffer);
		//Optional delay
		//_delay_ms(16);
		
		//Reset the ADC value
		val = 0;
		val1 = 0;
	}
}

/*
Fragen:

a)
Ja, wenn die Batteriespannung als Referenzspannung des ADCs verwendet
wird oder die Referenzspannung von der Batteriespannung abhängt.
Da die Batteriespannung mit dem Ladestand sinkt, und die Referenzspannung
die obere Grenze des Messbereichs bestimmt, würde der ADC bei der selben
anliegenden Spannung einen höheren Wert zurückliefern, je niedriger der
Ladestand der Batterie ist.

b)
Der Atmega32A hat einen ADC mit einem angeschlossenen Analogen 8-Kanal 
Multiplexer, sodass die Spannung an 8 verschiedenen Pins gemessen wird.

c)
Für bestimmte Zwecke. Die ADCs in Mikrocontrollern sind nützlich für
viele Aufgaben, allerdings können für eine bestimmte Aufgabe z.B.
eine höhere Auflösung, mehr Kanäle, eine höhere Konvertierungsrate,
weniger Interferenz etc. benötigt werden. Auch kann bei manchen
Microcontrollern die Referenzspannung nicht angepasst werden, da
kein VREF-Pin ausgebrochen wird, beispielsweise beim
STM32F411CEU6 im 64 Pin Format. Falls in so einem Fall z.B. ein
Spannungsbereich von 0-1V gemessen wird, würden 4/5 der Auflösung
verschwendet werden.
*/