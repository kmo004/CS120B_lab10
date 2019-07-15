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

unsigned char Lights = 0x00;

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
		Lights= 0x01;
		break;
		
		case SM_2:
		Lights = 0x02;
		break;
		
		case SM_3:
		Lights = 0x04;
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

enum States2 {SpeakerOn, SpeakerOff} state3;
unsigned char makeSound = 0x00;

void Speakers()
{
	switch(state3){
	
		case SpeakerOn:
			state3 = SpeakerOff;
			break;
			
		case SpeakerOff:
			state3 = SpeakerOn;
			break;
	}
	switch(state3)
	{	
		case SpeakerOn:
		makeSound = 0x10;
		break;
		
		case SpeakerOff:
		makeSound = 0x00;
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
  DDRA = 0x00; PORTA = 0xFF;  
  DDRB = 0xFF; PORTB = 0x00;
  TimerSet(2);
  TimerOn();
	
	unsigned long onesec = 1000;
	unsigned long twosec = 300;
	unsigned long counter = 0x00;
	unsigned long counter2 = 0x00;
	unsigned char cnt = 0;
	
	state = Init;
	state1 = ON;
	state3 = SpeakerOn;
	
    /* Insert your solution below */
    
    while (1) {
		if (counter >= onesec){
			BlinkingLEDSM();
			counter = 0;
		}
		if (counter2 >= twosec){
			Tick();
			counter2 = 0;
		}
		if((~PINA & 0x04) == 0x04){
			cnt = 1;
		}
		if(cnt == 1){
			Speakers();
			cnt = 0;
		}
		counter = counter + 2;
		counter2 = counter2 + 2;
		PORTB = tmpB|Lights|makeSound;
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
