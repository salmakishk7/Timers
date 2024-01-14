/*
 * Challenge4.c
 *
 *  Created on: Oct 11, 2020
 *  Author: Mohamed Tarek
 */
#include <avr/io.h>
#include <avr/interrupt.h>

/* global variable contain the ticks count of the timer */
unsigned char g_tick = 0;

/* Interrupt Service Routine for timer0 compare mode */
ISR(TIMER1_COMPA_vect)
{
	g_tick++;
	if(g_tick == 1)
	{
		PORTC ^= (1<<PC0);     /* Toggle PC0 Pin */
		OCR1A = 125;           /* Set the Compare value to 125 ((125/500) * 100 = 25%) */
	}
	else if(g_tick == 2)
	{
		PORTC ^= (1<<PC0);     /* Toggle PC0 Pin */
		OCR1A = 375;           /* Set the Compare value to 375 ((375/500) * 100 = 75%) */
		g_tick = 0;
	}
}

/*
 * For clock=8Mhz and prescale F_CPU/8 every count will take 1us, so we just need 500 counts
 * to get 500us period. In case we need 25% duty cycle we need to make the timer count 500 counts
 * with 125 counts the value of the PC0 pin is high and 375 counts with low value.
 */
void Timer1_CTC_Square_Wave_Init(void)
{
	TCNT1 = 0;		/* Set timer1 initial count to zero */

	OCR1A = 125;    /* Set the Compare value to 125 ((125/500) * 100 = 25%) */

	TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */

	/* Configure timer control register TCCR1A
	 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. FOC1A=1 FOC1B=0
	 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
	 */
	TCCR1A = (1<<FOC1A);

	/* Configure timer control register TCCR1B
	 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
	 * 2. Prescaler = F_CPU/8 CS10=0 CS11=1 CS12=0
	 */
	TCCR1B = (1<<WGM12) | (1<<CS11);
}

int main(void)
{
	DDRC |= 0x01;   /* Configure PC0 as output pin */
	PORTC &= 0xFE;  /* Clear PC0 pin */

	SREG |= (1<<7); /* Enable global interrupts in MC */

	Timer1_CTC_Square_Wave_Init();

    while(1)
    {

    }
}


