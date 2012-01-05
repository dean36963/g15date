//Time and date
//Dean Birch

//todo, get rid of implicit definition of clock.h functions in this file?

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libg15.h>
#include <ctype.h>
#include <g15daemon_client.h>
#include <libg15render.h>
#include <sched.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

pthread_cond_t wake_now = PTHREAD_COND_INITIALIZER;
const int MAX_STR_LENGTH=256;
const int DEFAULT_WAIT_TIME=1;
const int FONT_WIDTH=6;


int getDatestring(struct tm *realtime,char *string,int type);

void g15_wait(int seconds);

int main(int argc, char *argv[])
{
	g15canvas *canvas;
	int i,offset;
	int g15screen_fd;
	int wait_time=DEFAULT_WAIT_TIME;
	time_t rawtime;
	struct tm *realtime;
	char timestr[80];
	int analogue=0;
	int *dialCoords;




	//copes with any amount of arguements and processes them in a logical way!
	for(i=0;i<argc;i++)
	{
		if(0==strncmp(argv[i],"-h",2)||0==strncmp(argv[i],"--help",6))
		{	//prints the help page
			printf("G15 date and time.\n");
			printf("-h,--help    \tShows this help page.\n");
			printf("-v,--version \tPrints version number.\n");
			printf("-a,--analogue\tDisplay analogue clock.\n");
			printf("Send bug reports dean.birch0@gmail.com\n");
			return 0;
		}
		else if(0==strncmp(argv[i],"-v",2)||0==strncmp(argv[i],"--version",9))
		{	//prints version number
			printf("0.2\n");
			return 0;
		}
		else if(0==strncmp(argv[i],"-a",2)||0==strncmp(argv[i],"--analogue",10))
		{	//prints version number
			printf("Using analogue clock.\n");
			analogue=1;
		}
	}


	//connecting to g15daemon
	if((g15screen_fd=new_g15_screen(G15_G15RBUF))<0)
	{
		printf("Sorry cannot connect to the G15daemon\n");
		return -1;
	}

	//getting canvas
	canvas = (g15canvas *) malloc (sizeof (g15canvas));
	if(canvas!=NULL) g15r_initCanvas(canvas);
	else return -1;


	//Displays Date and time on a loop
	if(!analogue)
	{
		while(1)
		{
			time(&rawtime);
			realtime = gmtime(&rawtime);

			g15r_clearScreen(canvas,0x00);

			//render time
			sprintf(timestr,"%2d:%02d",  (realtime->tm_hour)%24,realtime->tm_min);
			offset =( G15_LCD_WIDTH - FONT_WIDTH*strlen(timestr))/2;
			g15r_renderString(canvas,(unsigned char*)timestr,0,G15_TEXT_HUGE,offset,0);

			//render date
			getDatestring(realtime,timestr,0);
			offset =( G15_LCD_WIDTH - FONT_WIDTH*strlen(timestr))/2;
			g15r_renderString(canvas,(unsigned char*)timestr,0,G15_TEXT_HUGE,offset,12);

			//render year (much point?)
			sprintf(timestr,"%d",realtime->tm_year+1900);
			offset =( G15_LCD_WIDTH - FONT_WIDTH*strlen(timestr))/2;
			g15r_renderString(canvas,(unsigned char*)timestr,0,G15_TEXT_HUGE,offset,26);


			//send to screen
			g15_send(g15screen_fd,(char*)canvas->buffer,G15_BUFFER_LEN);

			//waits so doesn't hog resources
			g15_wait(wait_time);
		}
	}
	else
	{
		dialCoords = (int*) malloc( 48*sizeof(int) );
		calcDial(dialCoords);
		
		while(1)
		{
			time(&rawtime);
			realtime = gmtime(&rawtime);

			g15r_clearScreen(canvas,0x00);

			//draw clock
			drawClock(canvas,realtime,dialCoords);

			//slap the date on too
			getDatestring(realtime,timestr,1);
			offset =( G15_LCD_WIDTH - FONT_WIDTH*strlen(timestr))/2;
			g15r_renderString(canvas,(unsigned char*)timestr,0,G15_TEXT_HUGE,offset+21,0);
			getDatestring(realtime,timestr,2);
			offset =( G15_LCD_WIDTH - FONT_WIDTH*strlen(timestr))/2;
			g15r_renderString(canvas,(unsigned char*)timestr,0,G15_TEXT_HUGE,offset+21,12);
			sprintf(timestr,"%d",realtime->tm_year+1900);
			offset =( G15_LCD_WIDTH - FONT_WIDTH*strlen(timestr))/2;
			g15r_renderString(canvas,(unsigned char*)timestr,0,G15_TEXT_HUGE,offset+21,26);

			//send to screen
			g15_send(g15screen_fd,(char*)canvas->buffer,G15_BUFFER_LEN);

			//waits so doesn't hog resources
			g15_wait(wait_time);

		}
	}

	return 0;
	
}

//function shamelessly stolen from g15stats program!
//thank you to whoever wrote it! :D
void g15_wait(int seconds) {
    pthread_mutex_t dummy_mutex;
    pthread_mutexattr_t   mta;
    struct timespec timeout;
      /* Create a dummy mutex which doesn't unlock for sure while waiting. */
    pthread_mutexattr_init(&mta);

    pthread_mutex_init(&dummy_mutex, &mta);
    pthread_mutex_lock(&dummy_mutex);

    if (clock_gettime(CLOCK_REALTIME, &timeout) != 0) {
        perror("clock_gettime");
    }

    timeout.tv_sec += seconds;

    pthread_cond_timedwait(&wake_now, &dummy_mutex, &timeout);
    pthread_mutex_unlock(&dummy_mutex);
    pthread_mutex_destroy(&dummy_mutex);
    pthread_mutexattr_destroy(&mta);
}

//type: 0=print day date month, 1=print day, 2=print date month
int getDatestring(struct tm *realtime,char *string,int type)
{
	int i=0;
	if(type<2)
	{
		switch(realtime->tm_wday)
		{
			case 0:
				sprintf(string,"Sunday");
				break;
			case 1:
				sprintf(string,"Monday");
				break;
			case 2:
				sprintf(string,"Tuesday");
				break;
			case 3:
				sprintf(string,"Wednesday");
				break;
			case 4:
				sprintf(string,"Thursday");
				break;
			case 5:
				sprintf(string,"Friday");
				break;
			case 6:
				sprintf(string,"Saturday");
				break;
			default:
				printf("Date is wrong?\n");
				break;
		}
		i = strlen(string);
	}

	if(type==1) return 0;

	sprintf(&string[i]," %d",realtime->tm_mday);
	i = strlen(string);
	switch(string[i-1])
	{
		case '1':
			sprintf(&string[i],"st");
			break;
		case '2':
			sprintf(&string[i],"nd");
			break;
		case '3':
			sprintf(&string[i],"rd");
			break;
		default:
			sprintf(&string[i],"th");
			break;
	}
	i = strlen(string);
	switch(realtime->tm_mon)
	{
		case 0:
			sprintf(&string[i]," January");
			break;
		case 1:
			sprintf(&string[i]," February");
			break;
		case 2:
			sprintf(&string[i]," March");
			break;
		case 3:
			sprintf(&string[i]," April");
			break;
		case 4:
			sprintf(&string[i]," May");
			break;
		case 5:
			sprintf(&string[i]," June");
			break;
		case 6:
			sprintf(&string[i]," July");
			break;
		case 7:
			sprintf(&string[i]," August");
			break;
		case 8:
			sprintf(&string[i]," September");
			break;
		case 9:
			sprintf(&string[i]," October");
			break;
		case 10:
			sprintf(&string[i]," November");
			break;
		case 11:
			sprintf(&string[i]," December");
			break;
	}
	return 0;
}

