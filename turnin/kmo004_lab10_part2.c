/*	Author: mwen004
 *  Partner(s) Name: Moker
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn()
{
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;

  _avr_timer_cntcurr = _avr_timer_M;
  
  SREG |= 0x80;
}

void TimerOff()
{
  TCCR1B = 0x00;
}

void TimerISR()
{
  TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
  _avr_timer_cntcurr--;
  if (_avr_timer_cntcurr == 0)
    {
      TimerISR();
      _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M)
{
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;
}

enum States {Init,SM_1,SM_2,SM_3} state;
void Tick()
{
	switch(state)
	{
		case Init:
		state = SM_1;
		break;
		
		case SM_1:
		state = SM_2;
		break;
		
		case SM_2:
		state = SM_3;
		break;
		
		case SM_3:
		state = SM_1;
		break;
		
		default: state = Init;
		
	}
	
	switch(state)
	{
		case SM_1:
		PORTB = 0x01;
		break;
		
		case SM_2:
		PORTB = 0x02;
		break;
		
		case SM_3:
		PORTB = 0x04;
		break;
		
		case Init:
		break;
	}
}

enum States1 {OFF,ON} state1;
unsigned char tmpB = 0x00;

void BlinkingLEDSM(){
	switch(state1){
		case OFF:
		state1 = ON;
		break;
		
		case ON:
		state1 = OFF;
		break;
	}
	switch(state1){
		case OFF:
		tmpB = 0x00;
		break;
		
		case ON:
		tmpB = 0x08;
		break;
	}
}
void soundTick(){
	
}

int main(void) {
    /* Insert DDR and PORT initializations */
  DDRA = 0xFF; PORTA = 0x00;  
  DDRB = 0xFF; PORTB = 0x00;
  TimerSet(300);
  TimerOn();
	
	unsigned long onesec = 1000;
	unsigned long counter = 0x00;
	
	state = Init;
	state1 = ON;
    /* Insert your solution below */
    
    while (1) {
		if (counter >= onesec){
			BlinkingLEDSM();
			counter = 0;
		}
		Tick();
		PORTB = tmpB|PORTB;
		counter = counter + 300;
		SoundTick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
