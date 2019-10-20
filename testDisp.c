#include <stdio.h>
#include "disdrv.h"
#include "termlib.h"

int main(void)
{
	
	dcoord_t myPoint = {};		//inicializa myPoint en (0,0). Recordemos que está arriba a la izquierda.
	disp_init();
	disp_clear();
	int i,j;
	for ( i = DISP_MIN; i <= (DISP_MAX_X) ; i++)
	{
		for (j = DISP_MIN; j <= (DISP_MAX_Y); j++)
		{
			myPoint.x = i;
			myPoint.y = j;
			disp_write(myPoint, D_ON);			//prende la mitad de arriba en el buffer. Otra opción es: disp_write({i, j}, D_ON);
			printf(YELLOW_TEXT "(%d,%d)\t" , i, j);
			disp_update();							//ahora lo vemos en el display
		}
		printf("\n");
	}
	return 1;
}