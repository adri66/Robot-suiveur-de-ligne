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
int main(void)
{

	DDRC &= ~(1<<BP_PIN);
	DDRB |= _BV(PINB5);
	lcd.display_init();
	lcd.clear();
	if(e_read(0x0000) != 1 )//check first startup
	{
		setup();	
	}
    while(1)
    {
		lcd.set_cursor(0,1);
		lcd.print_text("set-up ok");
		if((PINC&(1<<BP_PIN)))
			setup();
    }
}