#include <stdio.h>
#include "44b.h"
#include "button.h"
#include "leds.h"
#include "utils.h"
#include "D8Led.h"
#include "gpio.h"

struct RLstat {
	int moving;
	int speed;
	int iter;
	int direction;
	int position;
};

static struct RLstat RL = {
	.moving = 0,
	.speed = 5,
	.iter = 0,
	.direction = 0,
	.position = 0,
};

int setup(void)
{
	leds_init();
	D8Led_init();
	D8Led_segment(RL.position);

	/* Port G: configuraci�n para espera activa */
	//COMPLETAR: utilizando el interfaz para el puerto G definido en gpio.h
	//hay que configurar los pines 6 y 7 del puerto G como pines de entrada y
	//activar las correspondientes resistencias de pull-up
	portG_conf(6, INPUT);
	portG_conf(7, INPUT);
	portG_conf_pup(6, ENABLE);
	portG_conf_pup(7, ENABLE);
	/*******************************************/

	Delay(0);
	return 0;
}

int loop(void)
{
	unsigned int buttons = read_button();

	if (buttons & BUT1) {
		led1_switch();
		if(RL.direction==0){
			RL.direction=1;
		}
		else RL.direction=0;
	// COMPLETAR: utilizando la interfaz para los leds definida en leds.h
	// hay que conmutar el led1, si esta encendido apagarlo y viceversa
	// Tambi�n hay que comutar la direcci�n del movimiento del led rotante
	// representado por el campo direction de la variable RL


	}

	if (buttons & BUT2) {
		led2_switch();
		if(RL.moving==0){
			RL.moving=1;
			RL.iter = RL.speed;
		}
		else RL.moving=0;
		// COMPLETAR: utilizando la interfaz para los leds definida en leds.h
		// hay que conmutar el led2
		// Tambi�n hay que comutar el estado de movimiento del led rotante
		// representado por el campo moving de la variable RL, y en caso de
		// ponerlo en marcha debemos reiniciar el campo iter al valor del campo speed.


	}

	if (RL.moving) {

		RL.iter--;
		if (RL.iter == 0) {

			if(RL.direction == 1){
				if(RL.position > 5) RL.position %= 6;
				else RL.position++;
			}

			else if (RL.direction == 0){
				if(RL.position < 0) RL.position %=6;
				else RL.position--;

			}
			RL.iter=RL.speed;
			D8Led_segment(RL.position);
		}
		// COMPLETAR: debemos hacer avanzar el led rotante una posici�n en
		// la direcci�n indicada por el campo direction de la variable RL.
	    // La posici�n actual est� representada en el campo position.
	   // Recordar que queremos un movimiento circular, representado por
	   // las 6 primeras posiciones en el array Segmentes del display de 8
	   // segmentos, por lo que position debe estar siempre entre 0 y 5.




		}


	Delay(2000); // espera de 200ms para que el bucle se repita 5 veces por segundo
	return 0;
}


int main(void)
{
	setup();

	while (1) {
		loop();
	}
}

