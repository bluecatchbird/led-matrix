#include "stdio.h"
#include "stdlib.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>


#define COLOR_MAX 0xffff


void drawRect( Display *thedisplay,
               Window thewindow,
               GC thecontext,
               Colormap thecolormap,
               XColor xcolour,
               const unsigned int x,
               const unsigned int y,
               const unsigned int size,
               const unsigned int jitter)
{
    XAllocColor(thedisplay,thecolormap,&xcolour);
    XSetForeground(thedisplay,thecontext,xcolour.pixel);
    XFillRectangle(thedisplay,thewindow,thecontext,
                   x*size+jitter,y*size+jitter,
                   size-jitter*2,size-jitter*2);
    XFlush(thedisplay);
}

void initDraw( Display *thedisplay,
               Window thewindow,
               GC thecontext,
               Colormap thecolormap,
               XColor xcolour,
               const unsigned int x,
               const unsigned int y,
               const unsigned int size,
               const unsigned int jitter)
{
    unsigned int x_t, y_t;

    for( x_t=0; x_t<x; x_t++ ) {
        for( y_t=0; y_t<y; y_t++ ) {
            xcolour.red =   COLOR_MAX/x*x_t;
            xcolour.green = COLOR_MAX/x*x_t;
            xcolour.blue =  COLOR_MAX/y*y_t;

            drawRect( thedisplay, thewindow, thecontext, thecolormap, xcolour, x_t, y_t, size, jitter);
        }
    }
}

int getValue( unsigned int * input )
{
	char buf[100];
	
	if(feof(stdin))
		return 1;
	
   if (!fgets(buf, sizeof buf, stdin))
		return 0;
		
    // remove \n
    buf[strlen(buf) - 1] = 0;
    *input = strtol(buf, NULL, 10);
	
	return 0;
}

int main(int argc,char **argv)
{
    unsigned int x, y, size;
    unsigned int jitter;
    unsigned int window_width;
    unsigned int window_height;
    int blackColor,whiteColor,thescreen;
    XTextProperty textproperty;
    Display *thedisplay;
    GC thecontext;
    XEvent anevent;
    XColor xcolour;
    Colormap thecolormap;
    Window thewindow;
	unsigned int input, r, g, b;

	if( argc != 4 )
	{
		printf("Usage: %s xfields yfields size\n", argv[0]);
		return 1;
	}

	x    = strtol(argv[1], NULL, 10);
	y    = strtol(argv[2], NULL, 10);
	size = strtol(argv[3], NULL, 10);
	
	window_height = y * size;
	jitter = size/10;
    window_width = x * size;
    window_height = y * size;


    
    thedisplay  = XOpenDisplay(NULL);
    blackColor  = BlackPixel(thedisplay,DefaultScreen(thedisplay));
    whiteColor  = WhitePixel(thedisplay,DefaultScreen(thedisplay));
    thescreen   = DefaultScreen(thedisplay);
    thecolormap = DefaultColormap(thedisplay,thescreen);

    /* Create the window */
    thewindow = XCreateSimpleWindow(thedisplay,
                                    DefaultRootWindow(thedisplay),0,0,
                                    window_width,window_height,0,blackColor,blackColor);
    XSelectInput(thedisplay,thewindow,StructureNotifyMask | KeyPressMask );
    XMapWindow(thedisplay,thewindow);

    /* Label the window */
    XStringListToTextProperty(&(argv[0]),1,&textproperty);
    XSetWMName(thedisplay,thewindow,&textproperty);

    /* Get the context */
    thecontext = XCreateGC(thedisplay,thewindow,0,NULL);
    XSetBackground(thedisplay,thecontext,blackColor);
    XSetForeground(thedisplay,thecontext,whiteColor);

    /* Wait for the MapNotify event */
    for (;;) {
        XNextEvent(thedisplay, &anevent);
        if (anevent.type == MapNotify)
            break;
    }

    /* Erase the display (In the background colour) */
    XClearWindow(thedisplay,thewindow);

    initDraw( thedisplay, thewindow, thecontext, thecolormap, xcolour, x, y, size, jitter);

    for (;;) {
		if( feof(stdin) )
			break;
		
		getValue( &input);
		getValue( &r);
		getValue( &g);
		getValue( &b);
		
		xcolour.red   = r*0xff;
		xcolour.green = g*0xff;
		xcolour.blue  = b*0xff;
		drawRect(thedisplay, thewindow, thecontext, thecolormap, xcolour, input/y, input%y, size, jitter);

		//usleep(100);
    }

    /* Done */
    printf("wait 10s and close...\n");
    sleep(10);
    XFreeGC(thedisplay,thecontext);
    XUnmapWindow(thedisplay,thewindow);
    XDestroyWindow(thedisplay,thewindow);
    XCloseDisplay(thedisplay);
}
