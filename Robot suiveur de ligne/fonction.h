/*
 * Created: 26/10/2014
 *  Author: Adrien
 */  

#include <stdint.h>

#ifndef FONCTION_H_
#define FONCTION_H_

void setup();
uint8_t e_read(uint16_t adresse);
uint8_t e_write(uint16_t adresse, uint8_t valeur);
uint16_t analog_read(uint8_t chanelle, uint16_t old_value);
void pwm(int duty_cicle);


#endif /* FONCTION_H_ */