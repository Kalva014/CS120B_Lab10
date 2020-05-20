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

unsigned char threeLEDs;
unsigned char blinkingLED;


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
                        if(threeLEDs == 0x01) {
				threeLEDs = 0x02;
			}
			else if(threeLEDs == 0x02) {
				threeLEDs = 0x04;
			}
			else {
				threeLEDs = 0x01;
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
                        if(blinkingLED == 0x00) {
                                blinkingLED = 0x08; //to get the 4th bit
                        }
                        else {
                                blinkingLED = 0x00;
                        }
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
                        PORTB = threeLEDs | blinkingLED;
                        break;
                default:
                        break;
        }
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(1000);
	TimerOn();

	ThreeLed_state = Start1;
	BlinkingLed_state = Start2;
	Combine_state = Start3;	

	threeLEDs = 0x01;
	blinkingLED = 0x00;

    /* Insert your solution below */
    while (1) {
	ThreeLEDsSM_Tick();
	BlinkingLEDSM_Tick();
	CombineLEDsSM_Tick();
    
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
