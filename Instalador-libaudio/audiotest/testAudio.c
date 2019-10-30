//Compile with:
//		gcc testAudio.c ../audiolib/libaudio.so -I/usr/local/include -L/usr/local/lib -lSDL -o testAudio -lpthread
//Run with:
//		./testAudio

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "libaudio.h"
#include <SDL/SDL.h>

#define SONG_NAME "./Sound/mariobros.wav" //nombre del archivo de audio
int main(void)
{
	char mySong[] = SONG_NAME;
	printf("Audio player \n" );

	init_sound();						//inicializa el driver
	if (player_status() == READY)		//si la inicialización fue existosa sigo
	{
		set_file_to_play(mySong);		//carga el archivo a reproducir			
		play_sound(); 					//reproduce el archivo de audio (no bloqueante)
		printf("Playing: %s \n", mySong);
	}
	else
	{
		printf("System not ready. Did you run init_sound() ? \n" );
		return 1;
	}
	while (player_status() == PLAYING) {}	//espera que el archivo termine de reproducirse
	printf("Finished: %s\n", mySong);	
	return 0;
}
