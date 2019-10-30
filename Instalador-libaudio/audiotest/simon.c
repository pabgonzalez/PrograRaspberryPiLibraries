

/* TEST OF Sound Library libaudio by dany*/
// gcc simon.c ../audiolib/libaudio.so -I/usr/local/include -L/usr/local/lib -lSDL -o simon -lpthread
// Run with ./simon


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "libaudio.h"
#include <SDL/SDL.h>

char AudioFile_RED[]="./Sound/red.wav";
char AudioFile_GREEN[]="./Sound/green.wav";
char AudioFile_BLUE[]="./Sound/blue.wav";
char AudioFile_YELLOW[]="./Sound/yellow.wav";
char AudioFile_BUZZ[]="./Sound/buzz.wav";


void do_something(void);

void play_color(char * AudioFile); 

#define PLAY_TIME 1			// Change do_something() delay 

void main(void) 
{
	
	printf( "Audio player \n"  );
	
	
    init_sound(); // Init sound driver
    
    play_color(AudioFile_RED); 
   	do_something();					  // Waste some time while playing
 	play_color(AudioFile_GREEN); 
 	do_something();					  // Waste some time while playing
 	play_color(AudioFile_BLUE); 
 	do_something();					  // Waste some time while playing
 	play_color(AudioFile_YELLOW); 
 	do_something();					  // Waste some time while playing
 	play_color(AudioFile_BUZZ); 
 	do_something();					  // Waste some time while playing
}


void play_color(char * AudioFile) // Non blocking Audio File
{
	
 	stop_sound();						  // stop previous audio track (Just in case)
 	
	if(player_status() == READY)
	{
			
		printf( "Playing: %s \n", AudioFile); 	
	
		set_file_to_play(AudioFile);      // Load file 			
	
		play_sound(); 					  // Play File (Non Blocking)

		return;
	}
	else
	{
			
		printf( "System not ready did you run init_sound() ? \n"  );
				
		exit(-1);		
	}
	
}




void do_something(void)
{
	int i;
				
	for(i=0;i<PLAY_TIME;i++)  // Do someting else for a while (change i)
	{
		
		printf( "Playing ...\n" ); // show this every 300 ms while playing the sound in background
	    SDL_Delay(300);
	}
	
	
}
