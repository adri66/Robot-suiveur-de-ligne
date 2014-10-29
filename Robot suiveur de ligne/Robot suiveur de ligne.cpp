/*
 * Created: 26/10/2014
 *  Author: Adrien
 */ 
#define F_CPU 16000000UL
#define ADRESSE_PW_FUL 0x01
#define ADRESSE_PW_MID 0x02
#define ADRESSE_PW_LEFT_RIGHT 0x03
#define ADRESSE_PW_CENTER 0x04
#define BP_PIN PINC3
#define L298_ENA 5 //Pins arduino
#define L298_ENB 6 //Pins arduino
#define L298_IN1 PIND2
#define L298_IN2 PIND3
#define L298_IN3 PIND4
#define L298_IN4 PIND7

#define CAPTEUR_F_L _BV(1)
#define CAPTEUR_M_L _BV(2)
#define CAPTEUR_L _BV(3)
#define CAPTEUR_C _BV(4)
#define CAPTEUR_R _BV(5)
#define CAPTEUR_M_R _BV(6)
#define CAPTEUR_F_R _BV(7)

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


#include "lib/LCD.h"

#include "fonction.h"

LCD lcd;
PCF8574 pcf(0x41);
int pw_center = 100-e_read(ADRESSE_PW_CENTER), pw_mid = 100-e_read(ADRESSE_PW_MID),
	pw_l_r = 100-e_read(ADRESSE_PW_LEFT_RIGHT), pw_full = 100-e_read(ADRESSE_PW_FUL);//initialisation des variable PWM, 99 étant le min et que l'on veut que se soit le max on fait 100-x
uint8_t capteur_read;
char c[16];
int main(void)
{
	DDRC &= ~(1<<BP_PIN);
	DDRD |= (_BV(L298_IN1)|_BV(L298_IN2)|_BV(L298_IN3)|_BV(L298_IN4));
	lcd.display_init();
	lcd.clear();
	if(e_read(0x0000) != 1 && 1==2)//check first startup
		setup();
	lcd.clear();
    while(1)
    {
		lcd.set_cursor(0,1);
		if((PINC&(1<<BP_PIN)))
			setup();
		capteur_read = pcf.send(0x00, true);
		if(capteur_read&CAPTEUR_F_L)
		{
			lcd.print_text(" | X X X X X X ");
			pwm_timer1(pw_full, 6);
			pwm_timer1(pw_center, 5);
		}
		else if(capteur_read&CAPTEUR_M_L)
		{
			lcd.print_text(" X | X X X X X ");
			pwm_timer1(pw_mid, 6);
			pwm_timer1(pw_center, 5);
		}
		else if(capteur_read&CAPTEUR_L)
		{
			lcd.print_text(" X X | X X X X ");
			pwm_timer1(pw_l_r, 6);
			pwm_timer1(pw_center, 5);
		}
		else if(capteur_read&CAPTEUR_C)
		{
			lcd.print_text(" X X X | X X X ");
			pwm_timer1(pw_center, 6);
			pwm_timer1(pw_center, 5);
		}
		else if(capteur_read&CAPTEUR_R)
		{
			lcd.print_text(" X X X X | X X ");
			pwm_timer1(pw_l_r, 5);
			pwm_timer1(pw_center, 6);
		}
		else if(capteur_read&CAPTEUR_M_R)
		{
			lcd.print_text(" X X X X X | X ");
			pwm_timer1(pw_mid, 5);
			pwm_timer1(pw_center, 6);
		}
		else if(capteur_read&CAPTEUR_F_R)
		{
			lcd.print_text(" X X X X X X | ");
			pwm_timer1(pw_full, 5);
			pwm_timer1(pw_center, 6);
		}
		sprintf(c, "PWL:%d - PWR:%d", 100-((OCR0B/255)*100), 100-((OCR0A/255)*100));
		lcd.print_text(c);
    }
}
//Ses deux fonctions seront remplassée par des portes NOT, j'économiserais donc 2 pin
void avancer()
{
	PORTD |= (_BV(L298_IN1)|_BV(L298_IN3));
	PORTD &= ~(_BV(L298_IN2)|_BV(L298_IN4));
}
void reculer()
{
	PORTD &= ~(_BV(L298_IN1)|_BV(L298_IN3));
	PORTD |= (_BV(L298_IN2)|_BV(L298_IN4));
}