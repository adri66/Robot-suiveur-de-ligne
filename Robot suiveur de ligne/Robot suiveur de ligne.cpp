/*
 * Created: 26/10/2014
 *  Author: Adrien
 */ 
#define F_CPU 16000000UL
#define BP_PIN PINC3

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


#include "lib/LCD.h"

#include "fonction.h"

LCD lcd;
uint16_t analog_old = 512;
uint16_t analog_new;
int duty;
int main(void)
{

	DDRC &= ~(1<<BP_PIN);
	DDRB |= _BV(PINB5);
	lcd.display_init();
	lcd.clear();
	if(e_read(0x0000) != 1 && 1==2)//check first startup
	{
		setup();	
	}
    while(1)
    {
		lcd.set_cursor(0,1);
		lcd.print_text("          ");
		lcd.set_cursor(0,1);
		if((PINC&(1<<BP_PIN)))
			setup();
		analog_new = analog_read(1, analog_old); //Attention, il faut positioner le curseur du pot à 50% pour démarer !
		if(analog_new <=1024)
			analog_old = analog_new;
		duty = analog_old/10.24;
		pwm_timer1(duty, 5);
		lcd.print_number(duty);
		_delay_ms(20);
    }
}