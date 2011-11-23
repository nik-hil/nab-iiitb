/*
 * c1.c
 *
 *  Created on: 04-Nov-2011
 *      Author: nikhil
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CONTOUR 250
typedef struct Contour{
	double area;
	CvSeq *seq;
}contour;

/*Use to multiply 3 layer RGB image to Gray layer Image.
	 * dest is gray layer image
	 * */
void imageMul(IplImage *rgb, IplImage *gray, IplImage *dest){
	int h = rgb->height, w=rgb->width;
	int i, j;
	if(!(rgb->width == gray->width && rgb->height == gray->height)){
		fprintf(stderr,"In imageMUl image dimension do not match. Exiting\n");
		exit(1);
	}
	if(dest->nChannels != 3){
		fprintf(stderr,"Dest is not gray scale. Exiting\n");
	}
	uchar *prgb   = ( uchar* )rgb->imageData;
	uchar *pgray  = ( uchar* )gray->imageData;
	uchar *pdest  = ( uchar* )dest->imageData;
	for(i=0; i< h; i++){
		for(j=0;j<w;j++){
			//R
			pdest[j*rgb->widthStep + j*rgb->nChannels + 2]= \
					prgb[i*rgb->widthStep + j*rgb->nChannels + 2] * \
					pgray[i*rgb->widthStep + j*rgb->nChannels + 0];
			//G
			pdest[j*rgb->widthStep + j*rgb->nChannels + 1]= \
					prgb[i*rgb->widthStep + j*rgb->nChannels + 1] * \
					pgray[i*rgb->widthStep + j*rgb->nChannels + 0];
			//B
			pdest[j*rgb->widthStep + j*rgb->nChannels + 0]= \
					prgb[i*rgb->widthStep + j*rgb->nChannels + 0] * \
					pgray[i*rgb->widthStep + j*rgb->nChannels + 0];
		}
	}
}

void findContour(IplImage *imgraw, contour *state){

	IplImage *imgseg = cvCreateImage( cvGetSize(imgraw), imgraw->depth,\
				imgraw->nChannels);

		CvMemStorage *storage = cvCreateMemStorage(0);
		CvSeq *comp = NULL;
		cvPyrSegmentation(imgraw, imgseg, storage, &comp, 5, 200,50);
		//cvSaveImage("Segemented.jpeg",imgseg,0);
		//cvShowImage("Segmented Image", imgseg);
		//cvWaitKey(0);

		IplImage *imggray = cvCreateImage( cvGetSize(imgraw), 8, 1 );
		storage = cvCreateMemStorage(0);

		int  athreshold=101;
		double statenew = 0;
		contour contourFree[250];

		cvCvtColor(imgseg, imggray, CV_RGB2GRAY);
		cvAdaptiveThreshold(imggray, imggray, 255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,\
				CV_THRESH_BINARY,athreshold,0);
		CvContourScanner scanner = cvStartFindContours(imggray, storage, \
				sizeof(CvContour), CV_RETR_LIST, CV_RETR_CCOMP, cvPoint(0,0));

		CvSeq *seq = cvFindNextContour(scanner);
		IplImage *imgdraw = cvCreateImage( cvGetSize(imgraw), 8, 1 );
		IplImage *imgdel = NULL;
		seq = cvFindNextContour(scanner);

		/*remove seqcount in final*/
		int seqcount=0;
		do{
			statenew = cvContourArea( seq, CV_WHOLE_SEQ, 0);
			contourFree[seqcount].area = statenew;
			contourFree[seqcount].seq = seq;
			if((statenew -state->area)> 0.0){
				state->area = statenew;
				state->seq = seq;
			}
			seqcount++;
			//cvZero(imgdraw);
			seq = cvFindNextContour(scanner);
		}while(seq);
		cvDrawContours(imgdraw, state->seq, cvScalarAll(255), cvScalarAll(100),\
				4, 1/*CV_FILLED*/, CV_AA, cvPoint(0,0));
		cvShowImage("Contour", imgdraw);
		cvDrawContours(imgdraw, state->seq, cvScalarAll(255), cvScalarAll(100),\
						4, CV_FILLED, CV_AA, cvPoint(0,0));
		cvShowImage("Contour1", imgdraw);

		printf("No of seq %d\n", seqcount);
		//		cvWaitKey(0);

		cvClearMemStorage (storage);
		cvClearSeq(&seq);
		return;

}

int captureCamera(){
	CvCapture* capture = cvCaptureFromCAM(0);
	IplImage *frame;
	contour state;
	while (1){
		frame = cvQueryFrame(capture);
		if(!frame){
			fprintf( stderr, "ERROR: Could not find frame\n" );
			exit(1);
		}
		if(frame->imageData==NULL) {
			fprintf(stderr,"Null encountered\n");
			continue;
		}
	//	cvShowImage("Frame", frame);
		state.area =0;
		findContour(frame, &state);
	//	printf("Size = %d\n", frame->nChannels);
		printf("Area %f , Percent = %f \n", state.area,100*state.area/(640*480));
		cvWaitKey(500);
	}
	cvReleaseCapture(&capture);
	return 0;
}
int main(int argc, char** argv){

	captureCamera();
	return 0;
	IplImage *imgraw1 = cvLoadImage("image2.jpeg",CV_LOAD_IMAGE_COLOR);
	//IplImage *imgraw2 = cvLoadImage("image1.jpeg",CV_LOAD_IMAGE_COLOR);

	//cvShowImage("Image",imgraw);
	//cvWaitKey(0);

	contour state1, state2;
	findContour(imgraw1,&state1);
	printf("----------------------\n");
	//findContour(imgraw2,&state2);

	//printf("Area %f n %f\n", state1.area, state2.area);
	//printf("Value = %f\n", cvMatchShapes(state1.seq,state2.seq,\
			CV_CONTOURS_MATCH_I3, 0));

	}
