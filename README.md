Robot-suiveur-de-ligne
======================
Se projet est en cours de dévloppement, au final les codes servirons à controler un robot suiveur de ligne piloter par atmega328p.
Si vous voyez des erreurs ou si vous avez des idées, conseilles, ... pour amméliorer mon code n'ésiter pas à me le faire parvenir.
Actuellement, le programmes fait fonctionner les fonctionalité suivante :

28/10/14
--------
- LCD 16*2 controler par PCF8574.
- Lecture et écriture de l'eeprom.
- Lecture de la valeur annoligique sur les 6 pins dédier à cette effet.
- Générateur PWM sur les pins coresspondant au timmer1, ce qui correspond au pins 5 et 6 de l'arduino, le support des autres sortie PWM viendra surement le jours dans des vertions futur.

29/10/14
--------
- Gestion du l298
