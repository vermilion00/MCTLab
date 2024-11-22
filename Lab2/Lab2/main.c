//Set the CPU frequency, needed for the _delay_ms function
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <xc.h>
#include <util/delay.h>

void aufgabe1(void);
void aufgabe2(void);

int main(void)
{
	//No infinite loop, as those are present in the exercise functions
	//Set the entire LED register as Output
	DDRB = 0xFF;
	//Turn off all LEDs
	PORTB = 0xFF;
	
	//aufgabe1();
	aufgabe2();
}

void aufgabe1(void){
	while(1){
		//LED indexes range from 0 to 7
		//Right
		for(int i = 7; i >= 1; i--){
			//Set the register bit to 0 to turn the LED on
			PORTB &= ~(1 << i);
			//Wait 200 ms
			_delay_ms(200);
			//Set the register bit to 1 to turn the LED off
			PORTB |= (1 << i);
		}
		//Left
		for(int i = 0; i <= 6; i++){
			//Set the register bit to 0 to turn the LED on
			PORTB &= ~(1 << i);
			//Wait 200 ms
			_delay_ms(200);
			//Set the register bit to 1 to turn the LED off
			PORTB |= (1 << i);
		}
	}
}

void aufgabe2(void){
	//Wait 200 ms = 50000 counts at /64 prescaler
	//Start Timer 1 in normal mode with /64 prescaler
	TCCR1B = (1 << CS11) | (1 << CS10);
	while(1){
		//LED indexes range from 0 to 7
		//Right to Left
		for(int i = 6; i >= 0; i--){
			//Set the register bit to 0 to turn the LED on
			PORTB &= ~(1 << i);
						
			//Wait until Timer 1 Count value reaches 50000
			while(TCNT1 <= 0xC34F);
			//Reset Timer 1 count value
			TCNT1 = 0;
			
			//Set the register bit to 1 to turn the LED off
			PORTB |= (1 << i);
		}
		//Left to Right
		for(int i = 1; i <= 7; i++){
			//Set the register bit to 0 to turn the LED on
			PORTB &= ~(1 << i);
			
			//Wait until Timer 1 count value has been reached
			while(TCNT1 <= 0xC34F);
			//Reset Timer 1 count value
			TCNT1 = 0;
			
			//Set the register bit to 1 to turn the LED off
			PORTB |= (1 << i);
		}
	}
}

/*
Timer-Einstellungen:
-Zählerwert = 50000
-Vorteiler = 64 (256 und 1024 auch möglich)
-CPU-Frequenz = 16 MHz

Fragen:

a)
Wenn ein einfaches Delay ausreicht und keine andere Funktion des Programms dabei warten muss.

b)
Der Nachteil ist, dass während der Wartezeit die CPU nichts machen kann und somit auch
andere Aufgaben nicht verrichtet werden können, selbst wenn diese theoretisch nicht
vom Delay abhängig sind.

c)
Wie schon erwähnt, wird die CPU durch das Delay blockiert und kann die Zeit nicht nutzen,
um andere unabhängige Aufgaben zu verrichten. Ein besserer Weg wäre, einen Timer so einzustellen,
dass beim Erreichen der gewünschten Zeit ein Interrupt gesetzt wird. Dies ermöglicht das Ausführen
anderer Aufgaben, solange der Timer läuft, da nicht ständig der Zählwert abgefragt werden muss.
*/