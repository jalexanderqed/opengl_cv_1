//
// This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Richard Campbell '99)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.demonews.com/hosted/nehe 
// (email Richard Campbell at ulmont@bellsouth.net)
//
#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // Header file for sleeping.
#include <cstdio>
#include <cmath>
#include <iostream>

/* ascii code for the escape key */
#define ESCAPE 27

#define KEY_DOWN true
#define KEY_UP false

#define H_RAD_PER_SECOND 1.5708 // 90 degrees
#define V_RAD_PER_SECOND 1.5708

using namespace std;

/* The number of our GLUT window */
int window;

uint64_t glutTime = 0;
bool fullScreen = false;
float vLookDir = 1.5708;
float hLookDir = -1.5708;
bool keyState[128];

struct point3d {
    GLdouble x;
    GLdouble y;
    GLdouble z;
};


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)            // We call this right after our OpenGL window is created.
{
    glClearColor(0.f, 0.0f, 0.3f, 0.0f);        // This Will Clear The Background Color To Black
    glClearDepth(1.0);                // Enables Clearing Of The Depth Buffer

    glShadeModel(GL_SMOOTH);            // Enables Smooth Color Shading

    glDepthFunc(GL_LESS);                // The Type Of Depth Test To Do
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);            // Enables Depth Testing

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                // Reset The Projection Matrix

    gluPerspective(45.0f, (GLfloat) Width / (GLfloat) Height, 0.1f,
                   100.0f);    // Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height) {
    if (Height == 0)                // Prevent A Divide By Zero If The Window Is Too Small
        Height = 1;

    glViewport(0, 0, Width, Height);        // Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat) Width / (GLfloat) Height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

point3d modifyCamera(uint64_t delta) {
    float timeMultiplier = (delta / 1000.0f);
    float hChange = timeMultiplier * H_RAD_PER_SECOND;
    float vChange = timeMultiplier * V_RAD_PER_SECOND;

    if (keyState['a']) hLookDir -= hChange;
    if (keyState['d']) hLookDir += hChange;
    if (keyState['w']) vLookDir -= vChange;
    if (keyState['s']) vLookDir += vChange;

    float sinTheta = sin(vLookDir);

    point3d lookPoint = {
            5 * sinTheta * cos(hLookDir),
            5 * cos(vLookDir),
            5 * sinTheta * sin(hLookDir)
    };
    return lookPoint;
}

/* The main drawing function. */
void DrawGLScene() {
    usleep(30);
    uint64_t oldTime = glutTime;
    glutTime = glutGet(GLUT_ELAPSED_TIME);
    uint64_t delta = glutTime - oldTime;

    point3d lookPoint = modifyCamera(delta);

    GLenum errCode = glGetError();
    if (errCode != GL_NO_ERROR) {
        const GLubyte *errString = gluErrorString(errCode);
        fprintf(stderr, "OpenGL Error: %s/n", errString);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // Clear The Screen And The Depth Buffer

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                // Reset The View

    cout << lookPoint.x << ' ' << lookPoint.y << ' ' << lookPoint.z << endl;
    gluLookAt(0, 0, 5, lookPoint.x, lookPoint.y, lookPoint.z + 5, 0, 1, 0);
    glPushMatrix();

    glutSolidTeapot(1);

    glPopMatrix();

    // since this is double buffered, swap the buffers to display what just got drawn.
    glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) {
    if (key == ESCAPE) {
        /* shut down our window */
        glutDestroyWindow(window);

        /* exit the program...normal termination. */
        exit(0);
    }

    keyState[key] = KEY_DOWN;
}

void keyUp(unsigned char key, int x, int y) {
    keyState[key] = KEY_UP;
}

int main(int argc, char **argv) {
    /* Initialize GLUT state - glut will take any command line arguments that pertain to it or
       X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */
    glutInit(&argc, argv);

    /* Select type of Display mode:
       Double buffer
       RGBA color
       Alpha components supported
       Depth buffer */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    /* get a 640 x 480 window */
    glutInitWindowSize(640, 480);

    /* the window starts at the upper left corner of the screen */
    glutInitWindowPosition(0, 0);

    /* Open a window */
    window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99");

    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawGLScene);

    /* Go fullscreen.  This is as soon as possible. */
    if (fullScreen) glutFullScreen();

    /* Even if there are no events, redraw our gl scene. */
    glutIdleFunc(&DrawGLScene);

    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);

    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyPressed);

    glutKeyboardUpFunc(&keyUp);

    /* Initialize our window. */
    InitGL(640, 480);

    /* Start Event Processing Engine */
    glutMainLoop();

    return 1;
}

