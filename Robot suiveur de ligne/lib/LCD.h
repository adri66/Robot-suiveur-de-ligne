/*
 * Created: 26/10/2014
 *  Author: Adrien
 */  

#ifndef LCD_H_
#define LCD_H_


#include <stdlib.h>
#include <stdio.h>


class PCF8574
{
	private:
		uint8_t adresse;
	public:
		uint8_t send(uint8_t data, bool read);
		PCF8574(uint8_t adresse_pcf);
};
class LCD
{
	private:
		PCF8574 pcf;
		bool check_busy_flag();
	public:
		LCD();
		void send(uint8_t data, bool BF);
		void display_init();
		void send_leter(uint8_t lettre);
		void print_text(char *txt);
		void print_number(int num);
		void clear();
		void set_cursor(int x, int y);
};


#endif