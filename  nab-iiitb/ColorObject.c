/*
 * ColorObject.c
 *
 *  Created on: 31-Oct-2011
 *      Author: nikhil
 *ColorObject.C used to detect objects.
 *1. Take frame
 *2. Convert to HSV color
 *3. Find the color whose % is max. Note it.
 *4. Divide frame in small block. Find color content in each block.
 *5. If the color is not as Frame color, and block color content is significant, we say an object is present.
 *6. Done !!
 *TO DO
 *1. Caliberate the frame/block to tell at what distance object is present
 * */

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "ColorObject.h"

/*Compare HSV value and tell colour for each pixel
 * */
int getColor(int H, int S, int V)
{
	int color;
	if (V < 75)
		color = BLACK;
	else if (V > 190 && S < 27)
		color = WHITE;
	else if (S < 53 && V < 185)
		color = GREY;
	else {
		if (H < /*20*/ 15)
			color = RED;
		else if (H < /*30*/ 23)
			color = ORANGE;
		else if (H < /*55*/ 39)
			color = YELLOW;
		else if (H < /*85*/ 77)
			color = GREEN;
		else if (H < 102)
			color = AQUA;
		else if (H < /*127*/ 127)
			color = BLUE;
		else if (H < /*149*/ 145)
			color = PURPLE;
		else if (H < /*175*/ 165)
			color = PINK;
		else	// full circle
			color = RED;	// back to Red
	}

	return color;
}

/*Find percentage of each color in image.
 * store it in pixelcount
 * */
void findPercentage(IplImage *imgRead, int *pixelCount){
	IplImage *imgHsv = cvCreateImage(cvGetSize(imgRead),8, 3);
	cvCvtColor(imgRead, imgHsv,CV_BGR2HSV);
	IplImage *imgHsvDisp = cvCreateImage(cvGetSize(imgRead),8, 3);


	int height = imgHsv->height;
	int width = imgHsv->width;
	int widthStep = imgHsv->widthStep;
	char *imgHsvData = imgHsv->imageData;
	int y=0, x=0;

	for(x=0;x<NUM_COLOR_TYPES ;x++)
		pixelCount[x]=0;

	for(y=0; y<height;y++){
		for (x=0;x<width; x++){
			uchar H = *(uchar *)(imgHsvData + y*widthStep + x*3 + 0); //h
			uchar S = *(uchar *)(imgHsvData + y*widthStep + x*3 + 1); //s
			uchar V = *(uchar *)(imgHsvData + y*widthStep + x*3 + 2); //v

			pixelCount[getColor(H, S, V)]++;
		}

	}
	cvReleaseImage(&imgHsv);
	cvReleaseImage(&imgHsvDisp);
}

/*Max color
 * returns color value with max content
 * */
int findMaxColor(int *pixelCount){
	float max=0.0;
	int id,i;
	for(i=0;i<NUM_COLOR_TYPES; i++){
		if(pixelCount[i] > max){
			id = i;
			max = pixelCount[i];
		}
	}
	return id;
}


int speakDistance(int obj[RESOLUTION][RESOLUTION]){
	int i, j, right =0, middle=0, left=0;
	int r = RESOLUTION/10;
	char string[20], sayBuffer[100];


	//To use
	if(RESOLUTION%10 == 0){
	// fill the speak10 array;
		for (i = 0; i< RESOLUTION; i++){
			for (j = 0; j < RESOLUTION; j++){

				/*To use pre-filled speak10 array keep this commented.
				 * else in header file comment pre-filled array and uncomment the
				 * following two line. You can remove second if anytime.
				 */
				//speak10[i][j] = (DIST * (RESOLUTION - i ))/RESOLUTION;


				if (DEBUG)printf("%3d ", speak10[i][j]);
			}
			if (DEBUG)printf("\n");
		}

		// Camera Min Dist at which camera can see BUFFDIST. It is added to speak10.
		for(i= RESOLUTION-1; i>-1; i--){
			for(j= RESOLUTION-1; j>-1; j--){
				if(obj[i][j] == 1){
					if((j<=(9*r + r-1) && j >= (7*r)) && right != 1){
						strcpy(string, "right");
						right =1;
						sprintf(sayBuffer,"Object detected at distance %d centimetre at %s",
								speak10[i][j] + BUFFDIST,string);
						printf("%s \n",sayBuffer);
					}
					else if((j <= (6*r + r-1) && j >= 3*r )&& middle != 1) {
						strcpy(string, "middle");
						middle =1;
						sprintf(sayBuffer,"Object detected at distance %d centimetre at %s",
								speak10[i][j]+ BUFFDIST,string);
						printf("%s \n",sayBuffer);
					}
					else if((j<=(2*r + r-1) && j >= 0 )&& left != 1){
						strcpy(string, "left");
						left =1;
						sprintf(sayBuffer,"Object detected at distance %d centimetre at %s",
								speak10[i][j]+ BUFFDIST,string);
						printf("%s \n",sayBuffer);
					}
				}
			}
		}
		return 0;
	}
	else {
		puts("ERROR: RESOLUTION is not multiple of 10\n");
		exit(1);
	}
}


void start(IplImage *imgRead){

	IplImage *imgHsv = cvCreateImage(cvGetSize(imgRead),8, 3);
	cvCvtColor(imgRead, imgHsv,CV_BGR2HSV);
	IplImage *imgHsvDisp = cvCreateImage(cvGetSize(imgRead),8, 3);


	int height = imgHsv->height;
	int width = imgHsv->width;
	int widthStep = imgHsv->widthStep;
	char *imgHsvDispData = imgHsvDisp->imageData;
	char *imgHsvData = imgHsv->imageData;
	int pixelCount[NUM_COLOR_TYPES];
	int y=0, x=0;
	int color =0;
	for(x=0;x<NUM_COLOR_TYPES ;x++)
		pixelCount[x]=0;

	//Convert in HSV image
	for(y=0; y<height;y++){
		for (x=0;x<width; x++){
			uchar H = *(uchar *)(imgHsvData + y*widthStep + x*3 + 0); //h
			uchar S = *(uchar *)(imgHsvData + y*widthStep + x*3 + 1); //s
			uchar V = *(uchar *)(imgHsvData + y*widthStep + x*3 + 2); //v

			color = getColor(H, S, V);
			pixelCount[color]++; // store no of pixel for each color

			*(uchar*)(imgHsvDispData + (y)*widthStep + (x)*3 + 0) = Hue[color];	// Hue
			*(uchar*)(imgHsvDispData + (y)*widthStep + (x)*3 + 1) = Sat[color];	// Full Saturation (except for black & white)
			*(uchar*)(imgHsvDispData + (y)*widthStep + (x)*3 + 2) = Val[color];		// Full Brightness
		}

	}

	IplImage *imgHSV_RGB = cvCreateImage(cvGetSize(imgRead), 8, 3);
	cvCvtColor(imgHsvDisp, imgHSV_RGB, CV_HSV2BGR);
	if(DEBUG){
		cvNamedWindow("Colors", CV_WINDOW_NORMAL);
		cvShowImage("Colors", imgHSV_RGB);
		//cvWaitKey(1);
	}

	//Find max color content in each block
	int maxColor = findMaxColor(pixelCount);
	width=cvGetSize(imgRead).width/RESOLUTION -1;
	height=cvGetSize(imgRead).height/RESOLUTION -1;
	int i,j, maxColorCrop=0;
	int pixelCountCrop[NUM_COLOR_TYPES];
	IplImage *imgCrop = cvCreateImage(cvSize(width+1, height +1),8,3);
	int obj[RESOLUTION][RESOLUTION];

	//array to store block info if it contains any obj
	for(i=0; i< RESOLUTION; i++)
		for(j=0; j<RESOLUTION; j++)
			obj[i][j]=0;

	x=0, y=0;
	for(i=0; i<RESOLUTION; i++){
		for(j=0; j<RESOLUTION; j++){
			cvSetImageROI(imgHSV_RGB, cvRect(x, y, width+1, height+1));
			cvCopy(imgHSV_RGB, imgCrop, NULL);
			cvResetImageROI(imgHSV_RGB);
			findPercentage(imgCrop, pixelCountCrop);
			maxColorCrop = findMaxColor(pixelCountCrop);
			float val=100*pixelCountCrop[maxColorCrop]/((height+1)*(width+1));
			//printf("percentage %f\n", val);

			if((maxColorCrop !=maxColor)&&(val > COLOR_LIMIT))
				obj[i][j]= 1;
			x= x+ width;
		}
		x=0;
		y=y+height;
	}

	//Print Resolution

	system("clear");
	for(i=0; i<RESOLUTION; i++){
		for(j=0; j<RESOLUTION; j++){
			printf("%3d ", obj[i][j]);
		}
		printf("\n");
	}
	printf("-------------------------------------------------------\n");

	cvReleaseImage(&imgCrop);
	cvReleaseImage(&imgHsv);
	cvReleaseImage(&imgHsvDisp);
	cvReleaseImage(&imgHSV_RGB);

	speakDistance(obj);

	//Comment this for free flow of code
	//if (DEBUG) cvWaitKey(0);

}
/*
int getHeight(CvSize size, int j){
	static int first_time = 0;
	static int height[RESOLUTION];

	if(first_time == 0){
		first_time =1 ;
		double alpha, beta, gamma, temp[RESOLUTION+1], x;
		alpha = atan(MIN/HEIGHT);
		beta = atan(MAX/HEIGHT);
		gamma = beta - alpha;
		printf("alpha=  %f beta= %f gamma= %f\n", alpha, beta, gamma);

		temp[0] = MIN;
		temp[RESOLUTION] = MAX-MIN;
		int i;
		for (i=0; i< RESOLUTION; i++){
			x = tan(alpha + (i*gamma)/RESOLUTION)* HEIGHT ;
			temp[i+1] = (size.height * x)/MAX;//temp[RESOLUTION];
			printf("temp %d %f = %f\n",i, x, temp[i]);
		}
		for(i=0; i< RESOLUTION; i++){
			height[i] = (int)(temp[i+1] - temp[i]);
			printf("height %d\n", height[i]);
		}
	}
	return height[j];
}

void start(IplImage *imgRead){

	IplImage *imgHsv = cvCreateImage(cvGetSize(imgRead),8, 3);
	cvCvtColor(imgRead, imgHsv,CV_BGR2HSV);
	IplImage *imgHsvDisp = cvCreateImage(cvGetSize(imgRead),8, 3);


	int height = imgHsv->height;
	int width = imgHsv->width;
	int widthStep = imgHsv->widthStep;
	char *imgHsvDispData = imgHsvDisp->imageData;
	char *imgHsvData = imgHsv->imageData;
	int pixelCount[NUM_COLOR_TYPES];
	int y=0, x=0;
	int color =0;
	for(x=0;x<NUM_COLOR_TYPES ;x++)
		pixelCount[x]=0;

	//Convert in HSV image
	for(y=0; y<height;y++){
		for (x=0;x<width; x++){
			uchar H = *(uchar *)(imgHsvData + y*widthStep + x*3 + 0); //h
			uchar S = *(uchar *)(imgHsvData + y*widthStep + x*3 + 1); //s
			uchar V = *(uchar *)(imgHsvData + y*widthStep + x*3 + 2); //v

			color = getColor(H, S, V);
			pixelCount[color]++; // store no of pixel for each color

			*(uchar*)(imgHsvDispData + (y)*widthStep + (x)*3 + 0) = Hue[color];	// Hue
			*(uchar*)(imgHsvDispData + (y)*widthStep + (x)*3 + 1) = Sat[color];	// Full Saturation (except for black & white)
			*(uchar*)(imgHsvDispData + (y)*widthStep + (x)*3 + 2) = Val[color];		// Full Brightness
		}

	}

	IplImage *imgHSV_RGB = cvCreateImage(cvGetSize(imgRead), 8, 3);
	cvCvtColor(imgHsvDisp, imgHSV_RGB, CV_HSV2BGR);
	cvNamedWindow("Colors", CV_WINDOW_NORMAL);
	cvShowImage("Colors", imgHSV_RGB);
	//cvWaitKey(1);


	//Find max color content in each block
	int maxColor = findMaxColor(pixelCount);
	width=cvGetSize(imgRead).width/RESOLUTION -1;
	height=cvGetSize(imgRead).height/RESOLUTION -1;
	int i,j, maxColorCrop=0;
	int pixelCountCrop[NUM_COLOR_TYPES];
	IplImage *imgCrop = cvCreateImage(cvSize(width+1, height +1),8,3);
	int obj[RESOLUTION][RESOLUTION];

	//array to store block info if it contains any obj
	for(i=0; i< RESOLUTION; i++)
		for(j=0; j<RESOLUTION; j++)
			obj[i][j]=0;

	x=0, y=0;
	for(i=0; i<RESOLUTION; i++){
		for(j=0; j<RESOLUTION; j++){
			cvSetImageROI(imgHSV_RGB, cvRect(x, y, width+1, height+1));
			cvCopy(imgHSV_RGB, imgCrop, NULL);
			cvResetImageROI(imgHSV_RGB);
			findPercentage(imgCrop, pixelCountCrop);
			maxColorCrop = findMaxColor(pixelCountCrop);
			float val=100*pixelCountCrop[maxColorCrop]/((height+1)*(width+1));
			//printf("percentage %f\n", val);

			if((maxColorCrop !=maxColor)&&(val > COLOR_LIMIT))
				obj[i][j]= 1;
			x= x+ width;
		}
		x=0;
		y=y+height;
	}

	//Print Resolution
	for(i=0; i<RESOLUTION; i++){
		for(j=0; j<RESOLUTION; j++){
			printf("%3d ", obj[i][j]);
		}
		printf("\n");
	}

	cvReleaseImage(&imgCrop);
	cvReleaseImage(&imgHsv);
	cvReleaseImage(&imgHsvDisp);
	cvReleaseImage(&imgHSV_RGB);

	speakDistance(obj);
	//Comment this for free flow of code
	if (DEBUG) cvWaitKey(0);

}
*/
void captureCamera(int camera){
	//select camera 0 and create a frame to store
	CvCapture* capture = cvCaptureFromCAM(camera);
	IplImage *frame = NULL, *frame_blur=NULL;

	while(1){
		frame = cvQueryFrame(capture);
		if(!capture){
			fprintf(stderr,"Cannot find frame. Try other value of USB port.\n");
			fprintf(stderr,"Exiting...\n");
			exit(1);
		}
		frame_blur = cvCreateImage(cvGetSize(frame),8, 3);
		cvFlip(frame, NULL, 1);
		cvSmooth(frame,frame_blur,CV_MEDIAN, 11, 1,0,0);
		if(DEBUG){
			cvNamedWindow("Frame", CV_WINDOW_NORMAL);
			cvShowImage("Frame",frame_blur);
		}
		start(frame_blur);
		//getHeight(cvGetSize(frame),0);
		cvWaitKey(500);
		cvReleaseImage(&frame_blur);
	}

}

int main(int argc, char *argv[]){

	if(argc == 2)
		captureCamera(atoi(argv[1]));
	else
		captureCamera(0);
}
