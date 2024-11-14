#include <xc.h>
#include <util/delay.h>

//Set the CPU frequency, needed for the _delay_ms function
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

void aufgabe1(void);
void aufgabe2(void);

int main(void)
{
	//No infinite loop, as those are present in the exercise functions
	//Set the entire LED register as Output
    DDRB = 0xFF;
	
	aufgabe1();
	//aufgabe2();
}

void aufgabe1(void){
	while(1){
		//LED indexes range from 0 to 7
		//Reverse order, since the LEDs should light up left to right
		for(int i = 7; i >= 0; i--){
			//Set the register bit to 1 to turn the LED on
			PORTB |= (1 << i);
			//Wait 200 ms
			_delay_ms(200);
			//Set the register bit to 0 to turn the LED off
			PORTB &= ~(1 << i);
		}
	}
}

void aufgabe2(void){
	while(1){
		//LED indexes range from 0 to 7
		//Reverse order, since the LEDs should light up left to right
		for(int i = 7; i >= 0; i--){
			//Set the register bit to 1 to turn the LED on
			PORTB |= (1 << i);
			
			//Wait 200 ms = 50000 counts at /64 prescaler
			//Overflow value, (2^16 - 1) - 50000 counts
			TCNT1 = 0x3CAF;
			//Load Timer1 in normal mode with /64 prescaler, Timer starts immediately
			TCCR1B = (1 << CS11) | (1 << CS10);
			//Wait until Timer 1 Overflow is detected
			while(!(TIFR & (1 << TOV1)));
			//Deactivate Timer 1
			TCCR1B = 0;
			//Clear Timer 1 Overflow flag
			TIFR &= ~(1 << TOV1);
			
			//Set the register bit to 0 to turn the LED off
			PORTB &= ~(1 << i);
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
anderer Aufgaben, solange der Timer läuft.
*/