#include <stdio.h>
#include "termlib.h"
#include "disdrv.h"
#include "joydrv.h"

#define THRESHOLD 40

int main(void)
{
	joy_init();
	disp_init();
	disp_clear();
	dcoord_t pos = {DISP_MAX_X>>1 , DISP_MAX_Y>>1};
	dcoord_t npos = pos;
	jcoord_t coord;
	do
	{
		disp_update();
		joy_update();
		
		coord = joy_get_coord();
		if(coord.x > THRESHOLD && npos.x < DISP_MAX_X)
			npos.x++;
		if(coord.x < -THRESHOLD && npos.x > DISP_MIN)
			npos.x--;
		if(coord.y > THRESHOLD && npos.y > DISP_MIN)
			npos.y--;
		if(coord.y < -THRESHOLD && npos.y < DISP_MAX_Y)
			npos.y++;
		
		disp_write(pos,D_OFF);
		disp_write(npos,D_ON);
		pos = npos;
		
		printf(CYAN_TEXT "Current coords: x = %d , y = %d\n" , coord.x, coord.y);	
	} while( joy_get_switch() == J_NOPRESS );
}