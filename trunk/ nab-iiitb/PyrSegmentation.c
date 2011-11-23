/*
 * PyrSegmentation.c
 *
 *  Created on: 21-Sep-2011
 *      Author: nikhil
 */

/*Things to do
 * 1. plot the contour using findcountor, use cvcanny
 * 2. find the place of contour remains same or no
 * 3. if contour not possible try to plot different recct in for loop and observe its sequence no and are associated with it
 *
 * */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void seg(IplImage *src, IplImage *dest){
	/*segmentation using Image Pyramids
	 * */

	IplImage *gray=NULL;
	CvMemStorage *storage = cvCreateMemStorage(0);
	//CvMemStorage *storage = NULL; use it with only contour
	CvSeq *comp = NULL;

	CvSeq *contours =0;
	double threshold = 100; // by trial-error 100 is fixed, should be changed as needed
	int  athreshold=101; // at 21/100 - no of contour are very large, they change rapidly
						// at 200 no of contour are small, they dont change rapidly
	cvPyrSegmentation(src, dest, storage, &comp, 5, 200,50);
	cvNamedWindow("Dest- Segementation - B4", CV_WINDOW_NORMAL);
	cvShowImage("Dest- Segementation - B4", dest);
/*
	printf("Total= %d\n", comp->total);
	for(int i=0; i < comp->total;i++){
		CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );

		if(cc) cvRectangleR(dest,cc->rect,CV_RGB(255,0,0),1,8,0);
		printf("%lf\n", cc->area);
	}
	cvShowImage("Dest- Segmentation - AT", dest);
*/
	storage = NULL;

	if( storage==NULL ) {
		gray = cvCreateImage( cvGetSize(src), 8, 1 );
		storage = cvCreateMemStorage(0);
	} else {
		cvClearMemStorage( storage );
	}
	cvCvtColor(dest,gray,CV_BGR2GRAY);
//	cvThreshold( gray, gray, threshold, 255, CV_THRESH_BINARY );
	cvAdaptiveThreshold(gray, gray, 255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,athreshold,0);

	cvFindContours(gray, storage, &contours, sizeof(CvContour), CV_RETR_LIST,  CV_CHAIN_APPROX_NONE, cvPoint(0,0));
	cvZero(gray);
	if(contours){
		//cvDrawContours(gray, contours, cvScalarAll(255),cvScalarAll(100), 4,1, CV_AA,cvPoint(0,0) );
		cvDrawContours(gray, contours, cvScalarAll(255),cvScalarAll(100), 4,CV_FILLED, CV_AA,cvPoint(0,0) );
	}
	cvNamedWindow("Contours", CV_WINDOW_NORMAL);
	cvShowImage("Contours",gray);
	IplImage *dest2 = cvCreateImage( cvGetSize(src), 8, 1 );
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&gray);


}

int main(int argc, char** argv){

	//select camera 0 and create a frame to store
	CvCapture* capture = cvCaptureFromCAM(1);

	char key =1 ;

	while (key != 'q'){

		//Do not release the frame n source
		IplImage *frame = cvQueryFrame(capture);
		if(!frame){
			fprintf( stderr, "ERROR: Could not capture frame\n" );
			exit(1);
		}
		IplImage *src=NULL, *dest=NULL;

		cvNamedWindow("camera", CV_WINDOW_NORMAL);
		cvShowImage("camera", frame);
		src = frame;
		dest = cvCreateImage( cvGetSize(src), src->depth, src->nChannels);
		seg(src, dest);

		cvReleaseImage(&dest);
		key = cvWaitKey(100);
		// Press Esc =27 key to exit

	}
	cvReleaseCapture(&capture);
	return 0;
}


