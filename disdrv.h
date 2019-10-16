/*
 * Display Driver
 *
 * Copyright 2019  Catedra Programacion I - ITBA
 *
 * Author(s):
 *  Daniel Jacoby
 *  Nicolas Magliola
 *  Pablo Vacatello
 */

#ifndef DISDRV_H
#define DISDRV_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define DISP_CANT_X_DOTS    16
#define DISP_CANT_Y_DOTS    16

#define DISP_MIN        0
#define DISP_MAX_X      (DIS_MIN+DIS_CANT_X_DOTS-1)		// = 15
#define DISP_MAX_Y      (DIS_MIN+DIS_CANT_Y_DOTS-1)		// = 15


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {
    uint8_t x;		//coordenada x del display  
    uint8_t y;		//coordenada y del display 
} dcoord_t;

typedef enum {D_OFF, D_ON} dlevel_t;	//Valores posibles para cada LED


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// Display Services

/**
 * @brief Inicializa el display. Debe llamarse UNA SOLA VEZ al empezar a usar el display. 
*/
void disp_init(void);

/**
 * @brief Borra enteramente tanto el display como el buffer (ver disp_write)
*/
void disp_clear(void);

/**
 * @brief Escribe al buffer, NO al display. 
 * @param coord Estructura de tipo dcoord_t, que contiene las coordenadas del punto al que se quiere escribir. 
				Recordar que (DISP_MIN < x < DISP_MAX_X) y que (DISP_MIN < y < DISP_MAX_Y). 
 * @param val	valor que se escribirá para el punto indicado por coord. Puede ser D_OFF o D_ON.
 * @return Descripcion valor que devuelve
*/
void disp_write(dcoord_t coord, dlevel_t val);

/**
 * @brief Actualiza todo el display con el contenido del buffer.
*/
void disp_update(void);


/*
EJEMPLO:
	dcoord_t myPoint = {};		//inicializa myPoint en (0,0). Recordemos que está arriba a la izquierda.
	disp_init();
	int i,j;
	for ( i = DISP_MIN; i < DISP_MAX_X ; i++ )
	{
		for (j = DISP_MIN; j < (DISP_MAX_Y >> 1); j++)
		{
			myPoint = { i , j };
			disp_write(myPoint, D_ON);			//prende la mitad de arriba en el buffer. Otra opción es: disp_write({i, j}, D_ON);
			printf( "LED {%d, %d} ON en el buffer.\n" , i, j );
			myPoint = { i , DISP_MAX_Y - j };
			disp_write(myPoint, D_OFF);			//apaga la mitad de abajo en el buffer
			printf("LED {%d, %d} OFF en el buffer\n", i, DISP_MAX_Y - j);
		}
		disp_update();							//ahora lo vemos en el display
	}
*/

/*******************************************************************************
 ******************************************************************************/

#endif // DISDRV_H
