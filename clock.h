#include <stdio.h>
#include <math.h>
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
#include <pthread.h>
#include <time.h>


//variables
int x,y,p,q,min_label;
const int circle_x=21,circle_y=20,circle_r=20;
double theta;



//fucnctions
void drawClock(g15canvas *canvas,struct tm *realtime,int *coords);
void calcDial(int *coords);
