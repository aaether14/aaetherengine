#include<stdio.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>





Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;




void DrawAQuad() {
    glClearColor(0.0, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



int main(int argc, char *argv[]) {



         dpy = XOpenDisplay(NULL);
   	 if(dpy == NULL)
	 {
  	      printf("\n\tcannot connect to X server\n\n");
              return -1;
   	 }




    root = DefaultRootWindow(dpy);
    vi = glXChooseVisual(dpy, 0, att);





    if(vi == NULL) {
        printf("\n\tno appropriate visual found\n\n");
        return -1;
    }
    else {
        printf("\n\tvisual %p selected\n", (void *)vi->visualid);
    }




    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;


    win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, 
			InputOutput, 
			vi->visual, 
			CWColormap | CWEventMask, &swa);

	
    	XMapWindow(dpy, win);
    	XStoreName(dpy, win, "Aaether Engine 0.03a");



    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);



    while(1) {


        XNextEvent(dpy, &xev);


        if(xev.type == Expose) {
            XGetWindowAttributes(dpy, win, &gwa);
            glViewport(0, 0, gwa.width, gwa.height);
            DrawAQuad();
            glXSwapBuffers(dpy, win);
        }



        else if(xev.type == KeyPress) {
            if (xev.xkey.keycode == 9)
                break;
        }



    }


    glXMakeCurrent(dpy, None, NULL);
    glXDestroyContext(dpy, glc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);


}
