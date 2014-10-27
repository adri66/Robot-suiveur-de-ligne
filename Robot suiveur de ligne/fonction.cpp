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
#define fail 100
#define scale 1024/100


#include "fonction.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib/LCD.h"


void setup()
{
	LCD lcd;
	uint16_t va_old;
	uint16_t va_new;
	char c[16];
	char *texte[16];
	uint8_t adresse_eeprom;
	lcd.set_cursor(0,1);
	lcd.print_text("Setup ...");
	_delay_ms(3000);
	for (int menu = 0 ; menu < 4 ; menu++)
	{
		switch(menu)
		{
			case 0:
				*texte = " X X X | X X X ";
				adresse_eeprom = ADRESSE_PW_CENTER;
				va_old = e_read(adresse_eeprom)*10.24;
			break;
			case 1:
				*texte = " X X | X | X X ";
				adresse_eeprom = ADRESSE_PW_LEFT_RIGHT;
				va_old = e_read(adresse_eeprom)*10.24;
			break;
			case 2:
				*texte = " X | X X X | X ";
				adresse_eeprom = ADRESSE_PW_MID;
				va_old = e_read(adresse_eeprom)*10.24;
			break;
			case 3:
				*texte = " | X X X X X | ";
				adresse_eeprom = ADRESSE_PW_FUL;
				va_old = e_read(adresse_eeprom)*10.24;
			break;
		}
		if(va_old > 1023)
			va_old = 512;
		for(int b = 0; !(PINC&(1<<BP_PIN)) ; b++)
		{
			lcd.set_cursor(0,1);
			lcd.print_text(*texte);
			lcd.set_cursor(0,2);
			lcd.print_text("                ");
			lcd.set_cursor(0,2);
			va_new = analog_read(0x01, va_old);
			if(va_new == 1025)
			{
				lcd.print_number((va_old/10.24));
				lcd.send_leter(0b01111110);
			}
			else if(va_new == 1026)
			{
				lcd.send_leter(0b01111111);
				lcd.print_number((va_old/10.24));
			}
			else if(va_new <= 1024)
			{
				va_old = va_new;
				lcd.print_number((va_old/10.24));
			}
			lcd.set_cursor(15,2);
			if(b<=50)
				lcd.send_leter('#');
			else if(b<100)
			{
				if(b==51)
				lcd.clear();
			}
			else
				b = 0;
		}
		e_write(adresse_eeprom, va_old/10.24);
		lcd.set_cursor(0,2);
		sprintf(c, "%d - ", (int)va_old/10.24);
		lcd.print_text(c);
		sprintf(c, "%d - ",e_read(adresse_eeprom));
		lcd.print_text(c);
		_delay_ms(2000);
		while ((PINC&(1<<BP_PIN)))_delay_ms(1);
	}
	lcd.clear();
	lcd.set_cursor(0,1);
	lcd.print_text("set-up ok");
	e_write(0x0000, 0x01);
}
uint8_t e_read(uint16_t adresse)
{
	for(int i = 0 ; (EECR & (1<<EEPE)) && i< fail ; i++ ) _delay_us(10);//Wait for completion of previous write
	EEARH = (adresse&0xff00)>>8;
	EEARL = (adresse&0x00ff);
	EECR |= (1<<EERE); //check Read enable
	return EEDR;
}
uint8_t e_write(uint16_t adresse, uint8_t valeur)
{
	for(int i = 0 ; (EECR & (1<<EEPE)) && i< fail ; i++ ) _delay_us(10);//Wait for completion of previous write
	EEARH = (adresse&0xff00)>>8;
	EEARL = (adresse&0x00ff);
	EECR &= ~(1<<EEPM1|1<<EEPM0); //Force Erase and write
	EEDR = valeur;
	EECR |= 1<<EEMPE; //Enable first step for write
	EECR |= 1<<EEPE; //Enable write
	for(int i = 0 ; (EECR & (1<<EEPE)) && i< fail ; i++);//wait write completed
}
uint16_t analog_read(uint8_t chanelle, uint16_t old_value)
{
	ADMUX = ((1<<REFS0)|0x01); //Set Vcc Ref and Channel
	ADCSRA = (1<<ADEN);//Enable ADC
	ADCSRA |= 1<<ADPS0|1<<ADPS1|1<<ADPS2; //16Mhz/128 = 125Khz
	ADCSRA |= (1<<ADSC);//Start convert
	while((ADCSRA&(1<<ADSC)));//Waiting convert
	ADCSRA &= ~(1<<ADEN);//Disable ADC
	if(ADC >= (old_value+(scale*10)))
		return 1025;
	else if(ADC <= (old_value-(scale*10)) && old_value >= scale*10)
		return 1026;
	else if(ADC <= old_value-scale)
		return ADC;
	else if(ADC >= old_value+scale)
		return ADC;
	else
		return old_value;
}