/*	Author: kennethalvarez
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "timer.h"

enum ThreeLEDsSM_States {Start1, Wait1} ThreeLed_state;
enum BlinkingLEDSM_States {Start2, Wait2} BlinkingLed_state;
enum CombineLEDsSM_States {Start3, Wait3} Combine_state;
enum Speaker_States {Start4, Off, /*OnPressed, OnReleased*/ On1, On2} Speaker_state;

unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned short i;
unsigned short j;
unsigned char tmpA;
unsigned char speaker;
unsigned char k;

/*
tydef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
}*/

void ThreeLEDsSM_Tick() {
	switch(ThreeLed_state) { //transitions
		case Start1:
			ThreeLed_state = Wait1;
			break;
		case Wait1:
			ThreeLed_state = Wait1; //loops
			break;
		default:
			break;
	}

	switch(ThreeLed_state) { //actions
		case Start1:
                        break;
                case Wait1:
                        if((threeLEDs == 0x01) && (i == 150)) {
				threeLEDs = 0x02;
				i = 0;
			}
			else if((threeLEDs == 0x02) && (i == 150)) {
				threeLEDs = 0x04;
				i = 0;
			}
			else if((threeLEDs == 0x04) && (i == 150)) {
				threeLEDs = 0x01;
				i = 0;
			}
			else {
				i = i + 1;
			}
                        break;
                default:
                        break;

	}
}


void BlinkingLEDSM_Tick() {
	switch(BlinkingLed_state) { //transitions
                case Start2:
			BlinkingLed_state = Wait2;
                        break;
                case Wait2:
                        BlinkingLed_state = Wait2; //loops
                        break;
                default:
                        break;
        }

	switch(BlinkingLed_state) { //actions
                case Start2:
                        break;
                case Wait2:
                        if((blinkingLED == 0x00) && (j == 500)) {
                                blinkingLED = 0x08; //to get the 4th bit
				j = 0;
                        }
                        else if((blinkingLED == 0x08) && (j == 500)) {
                                blinkingLED = 0x00;
				j = 0;
                        }
			else {
				j = j + 1;
			}
                        break;
                default:	
                        break;
	}
}


void SpeakerSM_Tick() {
	switch(Speaker_state) { //transitions
                case Start4:
                        Speaker_state = Off;
                        break;
                case Off:
                        if((tmpA & 0x04) == 0x04) {
				Speaker_state = On1;
			}
			else {
				Speaker_state = Off;
			}
                        break;
		/*case OnPressed:
			if((tmpA & 0x04) == 0x04) {
				Speaker_state = OnPressed;
			}
			else {
				Speaker_state = OnReleased;
			}
			break;
		case OnReleased:
			Speaker_state = Off;
			break;*/

		case On1:
			if((tmpA & 0x04) == 0x04) {
                                Speaker_state = On2;
                        }
                        else {
                                Speaker_state = Off;
                        }
			break;
		case On2:
			if((tmpA & 0x04) == 0x04) {
                                Speaker_state = On1;
                        }
                        else {
                                Speaker_state = Off;
                        }

			break;
                default:
                        break;
        }
                
        switch(Speaker_state) { //actions
                case Start4:
                        break;
                case Off:
			speaker = 0x00;
                        break;
		/*case OnPressed:
			if((speaker == 0x10) && (k == 1)) {
				speaker = 0x00;
				k = 0;
			}
			else if((speaker == 0x00) && (k == 1)) {
				speaker = 0x10;
				k = 0;
			}
			else {
				k = k + 1;
			}
			
			if(speaker == 0x10) {
				speaker = 0x00;
			}
			else {
				speaker = 0x10;
			}
			break;
		case OnReleased:
			break;*/
		case On1:
			speaker = 0x10;
			break;
		case On2:
			speaker = 0x00;
			break;
                default:
                        break;
        }
}


void CombineLEDsSM_Tick() {
	switch(Combine_state) { //transitions
                case Start3:
                        Combine_state = Wait3;
                        break;
                case Wait3:
                        Combine_state = Wait3; //loops
                        break;
                default:
                        break;
        }

        switch(Combine_state) { //actions
                case Start3:
                        break;
                case Wait3:
                        PORTB = threeLEDs | blinkingLED | speaker;
                        break;
                default:
                        break;
        }
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(2);
	TimerOn();

	ThreeLed_state = Start1;
	BlinkingLed_state = Start2;
	Speaker_state = Start4;
	Combine_state = Start3;	

	threeLEDs = 0x01;
	blinkingLED = 0x00;
	i = 0;
	j = 0;
	k = 0;
	speaker = 0x00;

    /* Insert your solution below */
    while (1) {
	tmpA = ~PINA;
	ThreeLEDsSM_Tick();
	BlinkingLEDSM_Tick();
	SpeakerSM_Tick();
	CombineLEDsSM_Tick();
    
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
