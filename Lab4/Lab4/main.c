//Set the CPU frequency, needed for the _delay_ms function
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <xc.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void aufgabe1(void);
void aufgabe2(void);
void aufgabe3(void);

//"Key Pressed" flag
uint8_t flag = 0;
//Active time of the PWM cycle in ms
float duty_cycle = 1;
//Set if the PWM output is high
uint8_t pwm_active = 1;
//Set if a timer is running
uint8_t timer_running = 0;
//Set if long duty cycle is used for A1
uint8_t long_duty_cycle = 0;

int main(void)
{
	//Set Pin D5 to Output
	DDRD = 0x10;
	
	//Enable all interrupts
	/*TODO: Check if this enables all interrupts or just
	allows for them to happen (Probably 2)*/
	sei();
	//Sw2 = 5% duty cycle, Sw3 = 7,5% duty cycle
	//Enable Interrupt 0 and 1
	GICR |= (1 << INT0) | (1 << INT1);
	//Set Interrupt 0 to check for falling edge
	MCUCR |= (1 << ISC01);
	MCUCR &= ~(1 << ISC00);
	//Set Interrupt 1 to check for falling edge
	MCUCR |= (1 << ISC11);
	MCUCR &= ~(1 << ISC10);
	
	aufgabe1();
	//aufgabe2();
	//aufgabe3();
}

//Simple delay PWM
void aufgabe1(void){
	while(1){
		if(long_duty_cycle){
			//Turn on Pin D5
			PORTD |= (1 << 5);
			//Wait for the active length of the signal
			_delay_ms(1.5);
			//Turn off Pin D5
			PORTD &= ~(1 << 5);
			//Wait for the inactive length of the signal
			_delay_ms(18.5);
		} else {
			//Turn on Pin D5
			PORTD |= (1 << 5);
			//Wait for the active length of the signal
			_delay_ms(1);
			//Turn off Pin D5
			PORTD &= ~(1 << 5);
			//Wait for the inactive length of the signal
			_delay_ms(19);
		}
	}
}

//Timer CTC PWM
void aufgabe2(void){
	//Set the Timer mode to CTC
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	//Enable CTC Interrupt
	TIMSK |= (1 << OCIE1A);
	//Start timer with /64 prescaler
	TCCR1B |= (1 << CS11) | (1 << CS10);
	
	/*TODO: DO more in the ISR? */
	while(1){
		if(pwm_active && !timer_running){
			//Load the active time length into the CCR
			/*TODO: Are floats allowed here? */
			ICR1 = 250.0 * duty_cycle - 1;
			//Activate Pin D5
			PORTD |= (1 << 5);
			//Reset the active phase flag
			pwm_active = 0;
			//Set the timer running flag
			timer_running = 1;
		}
		if(!pwm_active && !timer_running){
			//Load the timer value into the CC register
			//250 counts per ms with /64 prescaler
			ICR1 = 250 * (20.0 - duty_cycle) - 1;
			//Deactivate Pin D5
			PORTD &= ~(1 << 5);
			//Set the active phase flag
			pwm_active = 1;
			//Set the timer running flag
			timer_running = 1;
		}  
	}
}

/*TODO: Check if COMPA is correct */
ISR(TIMER1_COMPA_vect){
	//Reset the timer running flag
	timer_running = 0;
}

void aufgabe3(void){
	//Set Timer 1 to Fast PWM mode
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |= (1 << WGM11);
	//Output PWM to Pin D5
	TCCR1A |= (1 << COM1A1);
	//Start Timer with /64 prescaler
	TCCR1B |= (1 << CS11) | (1 << CS10);
	while(1){
		//Load the first register with the high phase length
		OCR1A = (duty_cycle * 250) - 1;
		//Load the second register with the low phase length
		/*TODO: Check if this needs the full 20 ms or 20 ms - duty cycle*/
		ICR1 = (20 * 250) - 1;
	}
}

ISR(INT0_vect){
	//Set the active delay to 1 ms = 5%
	duty_cycle = 1;
	long_duty_cycle = 0;
}

ISR(INT1_vect){
	// Set the active delay to 1,5 ms = 7,5%
	duty_cycle = 1.5;
	long_duty_cycle = 1;
}

/*
Fragen:

a)
1. ist einfach und schnell zu programmieren und eignet sich daher für simple Anwendungen.
2. ist etwas komplizierter, lässt aber den Prozessor für andere Anwendungen frei.
3. ist für PWM spezifisch sehr einfach und effektiv. Außer im esotherischen Fall, dass
   außer dem PWM-Signal der selbe Timer noch für eine andere Funktion verwendet werden
   muss, sehe ich keinen Grund, 2. zu verwenden.
   
b)
Mit einem PWM-Output und einem Kondensator kann eine beliege Spannung zwischen 0V und 
der PWM-Ausgangsspannung erzeugt werden, indem die volle Ausgangsspannung nur für eine
bestimmte Zeitspanne ausgegeben wird, und die entstehende Rechteckige Spannungsform
durch den Kondensator halbwegs geglättet wird. Um eine 1V Ausgangsspannung bei einem
5V PWM-Signal zu erzeugen, muss der duty cycle auf 20% (1V/5V) gestellt werden.
Für das beste Ergebnis sollte eine möglichst kurze Periode genommen werden, da so 
die Kondensatorspannung am stabilsten ist.
*/