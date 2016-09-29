#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


int window_width  = 200;
int window_height = 200;

bool active = true;
bool fullscreen = false;


void display () 
{
  // check if there have been any openGL problems
  GLenum errCode = glGetError();
  if (errCode != GL_NO_ERROR) 
  {
      const GLubyte* errString = gluErrorString(errCode);
      fprintf( stderr, "OpenGL error: %s\n", errString );
  }

  // clear the frame buffer
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( 0, 0, 5, 0, 0, 0, 0, 1, 0 );
  glPushMatrix();

  // draw stuff
  glutSolidTeapot( 1 );

  glPopMatrix();

  // swap buffers
  glutSwapBuffers();  
}

GLvoid killWindow(){
  if(fullscreen){

  }
}


void reshape ( int width, int height ) 
{
  // set window height and width
  window_width  = width;
  window_height = height; 

  glViewport( 0, 0, width, height );  

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f );


  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void keyboard_func ( unsigned char key, int x, int y )
{
  switch( key )
  {
  case 'q':
  case 'Q':
    exit( 0 );
    break;

  default:
    break;
  }
}


void mouse_click_func ( int button, int state, int x, int y )
{
}


void mouse_move_func ( int x, int y )
{
}


void idle_func ()
{
}


int init ()
{
  glClearColor( 0.9f, 0.0f, 0.9f, 0.0f );
  glShadeModel( GL_SMOOTH );
  
  glClearDepth(1.0f);
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_NORMALIZE );

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  // callbacks
  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutKeyboardFunc( keyboard_func );
  glutMouseFunc( mouse_click_func );
  glutMotionFunc( mouse_move_func );
  glutIdleFunc( idle_func );

  return true;
}


int main ( int argc, char **argv )
{
  // set up the window
  glutInit( &argc, &argv[0] ); 
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowPosition( 100, 100 );
  glutInitWindowSize( window_width, window_height );
  glutCreateWindow( "template" );
 
  // initialize parameters
  init();

  // wait for something to happen
  glutMainLoop();

  return 0;
}
