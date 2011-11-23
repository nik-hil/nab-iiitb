Make sure you have installed all the openCV libraries.
We were running this code on Ubuntu 1104.

To compile the code use
	gcc -L/usr/lib -o "NAB" -lcvaux -lhighgui -lcv ColorObject.c

To run give command line argument the USB port no at which Camera
is connected. 0 is given as default. For e.g.
	./NAB 1

In ColorObject.h file,
 speak10 array has distance filled in it.
Distance measurement is not constant because of perspective Distortion.
These values are filled after small experimental setup using A4 size sheet.

Make DEBUG =1 to see output on screen.

To use Resolution other than 10, comment the pre-initialized speak10 array.
And uncomment array declaration below it.
And in ColorObject.c file, uncomment at line 125 in speakDistance for 
		speak10[i][j] = (DIST * (RESOLUTION - i ))/RESOLUTION;

	