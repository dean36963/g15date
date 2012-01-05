#include "clock.h"

void drawClock(g15canvas *canvas,struct tm *realtime,int *coords)
{
	//draw circle (0=no fill)
	g15r_drawCircle(canvas,circle_x,circle_y,circle_r,0,G15_COLOR_BLACK);

	//draw hour hand - get coordinates
	theta = (double)realtime->tm_hour*(double)M_PI/6.0;
	x = circle_r*sin(theta)/2 + circle_x;
	y = circle_y - circle_r*cos(theta)/2;
	g15r_drawLine(canvas,circle_x,circle_y,x,y,G15_COLOR_BLACK);
	
	//draw min hand - get coordinates
	theta = (double)realtime->tm_min*(double)M_PI/30.0;
	x = (int)circle_r*sin(theta)/1.2 + circle_x;
	y = circle_y - (int)circle_r*cos(theta)/1.2;
	g15r_drawLine(canvas,circle_x,circle_y,x,y,G15_COLOR_BLACK);

	//draw seconds need for diagnosis!
	theta = (double)realtime->tm_sec*(double)M_PI/30.0;
	x = (int)circle_r*sin(theta)/1.2 + circle_x;
	p = (int)circle_r*sin(theta) + circle_x;
	y = circle_y - (int)circle_r*cos(theta)/1.2;
	q = circle_y - (int)circle_r*cos(theta);
	g15r_drawLine(canvas,x,y,p,q,G15_COLOR_BLACK);
	
	//need to be able to see centre!
	g15r_setPixel(canvas,circle_x,circle_y,G15_COLOR_WHITE);

	//draw dials
	for(min_label=0;min_label<12;min_label++) 
		g15r_drawLine(canvas,coords[min_label*4+2],coords[min_label*4+3],coords[min_label*4],coords[min_label*4+1],G15_COLOR_BLACK);
	
}

void calcDial(int *coords)
{
	for(min_label=0;min_label<12;min_label++)
	{
		theta = 5.0*(double)min_label*(double)M_PI/30.0;
		coords[min_label*4] = (int)circle_r*sin(theta)/1.1 + circle_x;
		coords[min_label*4+2] = (int)circle_r*sin(theta)/1.05 + circle_x;
		coords[min_label*4+1] = circle_y - (int)circle_r*cos(theta)/1.1;
		coords[min_label*4+3] = circle_y - (int)circle_r*cos(theta)/1.05;
	}
}

