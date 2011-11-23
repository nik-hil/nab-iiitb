/*
 * ColorObject.h
 *
 *  Created on: 06-Nov-2011
 *      Author: nikhil
 */

#ifndef COLOROBJECT_H_
#define COLOROBJECT_H_

// size of speak array
#define RESOLUTION 10
// vary limit on color in each small block
#define COLOR_LIMIT 30.0
// dist upto which camera can see
#define DIST 100
// dist from which camera can see
#define BUFFDIST 30
// make it 1 to enable debug
#define DEBUG 1
// min dist camera can see
//#define MINDIST 50
// Max dist camera can see
//#define MAXDIST 450
// Height of camera
#define HEIGHT 50


enum {BLACK=0, WHITE, GREY, RED, ORANGE, YELLOW, GREEN, AQUA, BLUE, PURPLE, PINK, NUM_COLOR_TYPES};
char* colorName[NUM_COLOR_TYPES] = {"Black", "White","Gray","Red","Orange","Yellow","Green","Aqua","Blue","Purple","Pink"};
uchar Hue[NUM_COLOR_TYPES] =    {0,       0,      0,     0,     20,      30,      55,    85,   115,    138,     161};
uchar Sat[NUM_COLOR_TYPES] =    {0,       0,      0,    255,   255,     255,     255,   255,   255,    255,     255};
uchar Val[NUM_COLOR_TYPES] =    {0,      255,    120,   255,   255,     255,     255,   255,   255,    255,     255};

/*Lookup table for distance Speaking*/

int speak10[10][10] = {
		{180,180,180,180,180,180,180,180,180,180},
		{120,120,120,120,120,120,120,120,120,120},
		{90,90,90,90,90,90,90,90,90,90},
		{60,60,60,60,60,60,60,60,60,60},
		{45,45,45,45,45,45,45,45,45,45},
		{30,30,30,30,30,30,30,30,30,30},
		{24,24,24,24,24,24,24,24,24,24},
		{16,16,16,16,16,16,16,16,16,16},
		{12,12,12,12,12,12,12,12,12,12},
		{07,07,07,07,07,07,07,07,07,07}
		};

//int speak10[RESOLUTION][RESOLUTION];

int getColor(int H, int S, int V);
void findPercentage(IplImage *imgRead, int *pixelCount);
int findMaxColor(int *pixelCount);
int speakDistance(int obj[RESOLUTION][RESOLUTION]);
void start(IplImage *imgRead);


#endif /* COLOROBJECT_H_ */
