#include "shapes.h"


void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offsetm
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left

   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top

   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();

   //  Undo transformations
   glPopMatrix();
}


void box(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offsetm
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(0.0f, 1.0f, 0.0f);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
  glColor3f(0.0f, 1.0f, 0.0f);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1, 1, 1);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0.0f, 1.0f, 0.0f);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0.0f, 1.0f, 0.0f);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,1,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End

   glEnd();

   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

/*
 *  Draw a sphere (version 1)
 *     at (x,y,z)
 *     radius (r)
 */
void star(double x,double y,double z, double dx,double dy,double dz)
{
 
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);

   //glScaled(r,r,r);

   glBegin(GL_POLYGON);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex2f( 0.0, 0.25);
   glVertex2f( 0.25,-0.25);
   glVertex2f(-0.25,-0.25);
   glEnd();
   

   //Back right triangle
   glTranslatef(0.25,-0.5,0);
   glBegin(GL_POLYGON);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex2f(0.0,0.25);
   glVertex2f(0.5, 0.0);
   glVertex2f(0.0,-.25);
   glEnd();

//back bottom triangle
   glTranslatef(-0.25,-0.25,0);
   glBegin(GL_POLYGON);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex2f( 0.25, 0.0);
   glVertex2f( 0.0,-0.5);
   glVertex2f(-0.25,0.0);
   glEnd();

//Back left triangle
   glTranslatef(-0.25,0.25,0);
   glBegin(GL_POLYGON);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex2f( 0.0, 0.25);
   glVertex2f( 0.0,-0.25);
   glVertex2f(-0.5,0.0);
   glEnd();

//front top triangle
   glTranslatef(0.25,0.5,.2);
   glBegin(GL_POLYGON);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex2f( 0.0, 0.25);
   glVertex2f( 0.25,-0.25);
   glVertex2f(-0.25,-0.25);
   glEnd();

//front right triangle
   glTranslatef(0.25,-0.5,0);
   glBegin(GL_POLYGON);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex2f(0.0,0.25);
   glVertex2f(0.5, 0.0);
   glVertex2f(0.0,-.25);
   glEnd();

//front bottom triangle
   glTranslatef(-0.25,-0.25,0);
   glBegin(GL_POLYGON);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex2f( 0.25, 0.0);
   glVertex2f( 0.0,-0.5);
   glVertex2f(-0.25,0.0);
   glEnd();

//front left triangle
   glTranslatef(-0.25,0.25,0);
   glBegin(GL_POLYGON);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex2f( 0.0, 0.25);
   glVertex2f( 0.0,-0.25);
   glVertex2f(-0.5,0.0);
   glEnd();


  //top left
   glTranslatef(0.0,0.5,-.1);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(+0,-.25,+.1);
   glVertex3f(+0,-.25,-.1);
   glVertex3f(+.25,+.25,-.1);
   glVertex3f(+.25,+.25,+.1);
   glEnd();

//top right
   glTranslatef(.25,0,0);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(+.25,-.25,+.1);
   glVertex3f(+.25,-.25,-.1);
   glVertex3f(+0,+.25,-.1);
   glVertex3f(+0,+.25,+.1);
   glEnd();

//right top
   glTranslatef(.5,-.5,0);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(-.25,+.25,+.1);
   glVertex3f(-.25,+.25,-.1);
   glVertex3f(+.25,0,-.1);
   glVertex3f(+.25,0,+.1);
   glEnd();

   //left bottom
   glTranslatef(-1,-.25,0);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(-.25,+.25,+.1);
   glVertex3f(-.25,+.25,-.1);
   glVertex3f(+.25,0,-.1);
   glVertex3f(+.25,0,+.1);
   glEnd();

   //right bottom
   glTranslatef(1,0,0);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(-.25,0,+.1);
   glVertex3f(-.25,0,-.1);
   glVertex3f(+.25,.25,-.1);
   glVertex3f(+.25,.25,+.1);
   glEnd();

   //right bottom
   glTranslatef(-1,.25,0);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(-.25,0,+.1);
   glVertex3f(-.25,0,-.1);
   glVertex3f(+.25,.25,-.1);
   glVertex3f(+.25,.25,+.1);
   glEnd();

   glTranslatef(.25,-.5,0);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(+.25,-.25,+.1);
   glVertex3f(+.25,-.25,-.1);
   glVertex3f(+0,+.25,-.1);
   glVertex3f(+0,+.25,+.1);
   glEnd();

   //top left
   glTranslatef(.25,0,0);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(+0,-.25,+.1);
   glVertex3f(+0,-.25,-.1);
   glVertex3f(+.25,+.25,-.1);
   glVertex3f(+.25,+.25,+.1);
   glEnd();

    //  Front
   glTranslatef(0,.5,-.15);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(-.25,-.25, .25);
   glVertex3f(+.25,-.25, .25);
   glVertex3f(+.25,+.25, .25);
   glVertex3f(-.25,+.25, .25);
   glEnd();

   //back square
   glTranslatef(0,0,-.2);
   glBegin(GL_QUADS);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f(-.25,-.25, .25);
   glVertex3f(+.25,-.25, .25);
   glVertex3f(+.25,+.25, .25);
   glVertex3f(-.25,+.25, .25);
   glEnd();
   //  Undo transformations
   glPopMatrix();
   
}



