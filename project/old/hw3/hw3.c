/*Works Cited
http://gamejust4u.webs.com/apps/forums/topics/show/8127559-opengl-color-guide
https://gist.github.com/23ars/4545671
http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/
*/
/*
 *  3D Objects
 *
 *  Demonstrates how to draw objects in 3D.
 *
 *  Key bindings:
 *  m          Go from model view to first person
 *  x/X          Toggle axes
 *  c          Toggle light
 *  i          Stop Light
 *  []         Move light up and down
 *  a/A        Change ambient lighting
 *  d/D        Change difuse level
 *  s/S        Change specular level
 *  e/E        Change Emission level
 *  n/N        Change Shininess level
   
   //  Repitition
   else if (ch=='+')
      rep++;
   else if (ch=='-' && rep>1)
      rep--;
   //  Toggle light movement
   else if (ch == 'i' || ch == 'I')
      move = 1-move;
 *  page up    Raise Dim
 *  page down  Lower Dim
 *  arrows     Change view angle
 *  p          In model view, changes from perspective mode to orthogonal
 *  0          Reset view angle
 *  ESC        Exit
 */
//#include "shapes.h"
#include "CSCIx229.h"

int mode=0;       //  Texture mode
int m = 0;        //toggle full light
int move=1;       //  Move light
int inc = 10;
int smooth = 1;  // Smooth/Flat shading
int local = 0;  // Local Viewer Model
int ntex=0;       //  Cube faces
int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lightin
int rep=1;        //  Repitition
int p=0;        //  perspective or orthoganal
double dim=3.5;   // dimension
int fov = 60;     // feild of view
double asp = 1;   // Aspect ratio
int i,j;

// angle of rotation for the camera direction
float angle = 0.0f;
// vector representing the camera's direction
float lx=0.0f,lz=-1.0f,ly=0.0f;
// position of the camera
float x=0.0f, z=5.0f, y=0.0f;

// the key states. These variables will be zero
//when no key is being presses
float CameraAngle = 0.0f;
float CameraPos = 0;
int xOrigin = -2;

// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  20;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   2;  // Elevation of light
unsigned int texture[7]; // Texture names



//change from ortho and perspective
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   gluPerspective(fov,asp,0.1,4*dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}
/*
 *  calculates position
 */
void calc_position(float CameraPos) {

   x += CameraPos * lx * 0.1f;
   z += CameraPos * lz * 0.1f;
   y += CameraPos * ly * 0.1f;

}
/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   //  Front
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void box(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
   //glBindTexture(GL_TEXTURE_2D,texture[1]);
   //  Front
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
static void pillow(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   //  Front
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
static void lightWood(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   //  Front
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}
static void star(double x,double y,double z, double dx,double dy,double dz)
{

   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[5]);

//back top triangle
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_POLYGON);
   glNormal3f(0, 0, -1);
   glTexCoord2f(0,0) ; glVertex2f( 0.0, 0.25);
   glTexCoord2f(1,0) ; glVertex2f( 0.25,-0.25);
   glTexCoord2f(1,1) ; glVertex2f(-0.25,-0.25);
   glEnd();
   

//Back right triangle
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(0.25,-0.5,0);
   glBegin(GL_POLYGON);
   glNormal3f(0, 0, -1);
   glTexCoord2f(0,0) ; glVertex2f(0.0,0.25);
   glTexCoord2f(1,0) ; glVertex2f(0.5, 0.0);
   glTexCoord2f(1,1) ; glVertex2f(0.0,-.25);
   glEnd();

//back bottom triangle
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(-0.25,-0.25,0);
   glBegin(GL_POLYGON);
   glNormal3f(0, 0, -1);
   glTexCoord2f(0,0) ; glVertex2f( 0.25, 0.0);
   glTexCoord2f(1,0) ; glVertex2f( 0.0,-0.5);
   glTexCoord2f(1,1) ; glVertex2f(-0.25,0.0);
   glEnd();

//Back left triangle
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(-0.25,0.25,0);
   glBegin(GL_POLYGON);
   glNormal3f(0, 0, -1);
   glTexCoord2f(0,0) ; glVertex2f( 0.0, 0.25);
   glTexCoord2f(1,0) ; glVertex2f( 0.0,-0.25);
   glTexCoord2f(1,1) ; glVertex2f(-0.5,0.0);
   glEnd();

//front top triangle
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(0.25,0.5,.2);
   glBegin(GL_POLYGON);
   glNormal3f(0, 0, 1);
   glTexCoord2f(0,0) ; glVertex2f( 0.0, 0.25);
   glTexCoord2f(1,0) ; glVertex2f( 0.25,-0.25);
   glTexCoord2f(1,1) ; glVertex2f(-0.25,-0.25);
   glEnd();

//front right triangle
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(0.25,-0.5,0);
   glBegin(GL_POLYGON);
   glNormal3f(0, 0, 1);
   glTexCoord2f(0,0) ; glVertex2f(0.0,0.25);
   glTexCoord2f(1,0) ; glVertex2f(0.5, 0.0);
   glTexCoord2f(1,1) ; glVertex2f(0.0,-.25);
   glEnd();

//front bottom triangle
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(-0.25,-0.25,0);
   glBegin(GL_POLYGON);
   glNormal3f(0, 0, 1);
   glTexCoord2f(0,1) ; glVertex2f( 0.25, 0.0);
   glTexCoord2f(1,0) ; glVertex2f( 0.0,-0.5);
   glTexCoord2f(1,1) ; glVertex2f(-0.25,0.0);
   glEnd();

//front left triangle
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(-0.25,0.25,0);
   glBegin(GL_POLYGON);
   glNormal3f(0, 0, 1);
   glTexCoord2f(0,0) ; glVertex2f( 0.0, 0.25);
   glTexCoord2f(1,0) ; glVertex2f( 0.0,-0.25);
   glTexCoord2f(1,1) ; glVertex2f(-0.5,0.0);
   glEnd();


  //top left
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(0.0,0.5,-.1);
   glBegin(GL_QUADS);
   glNormal3f( -.6, .4 ,0);
   glTexCoord2f(0,0) ; glVertex3f(+0,-.25,+.1);
   glTexCoord2f(1,0) ; glVertex3f(+0,-.25,-.1);
   glTexCoord2f(1,1) ; glVertex3f(+.25,+.25,-.1);
   glTexCoord2f(0,1) ; glVertex3f(+.25,+.25,+.1);
   glEnd();

//top right
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(.25,0,0);
   glBegin(GL_QUADS);
   glNormal3f( .6, .4 ,0);
   glTexCoord2f(0,0) ; glVertex3f(+.25,-.25,+.1);
   glTexCoord2f(1,0) ; glVertex3f(+.25,-.25,-.1);
   glTexCoord2f(1,1) ; glVertex3f(+0,+.25,-.1);
   glTexCoord2f(0,1) ; glVertex3f(+0,+.25,+.1);
   glEnd();

//right top
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(.5,-.5,0);
   glBegin(GL_QUADS);
   glNormal3f( .4, .6, 0);
   glTexCoord2f(0,0); glVertex3f(-.25,+.25,+.1);
   glTexCoord2f(1,0); glVertex3f(-.25,+.25,-.1);
   glTexCoord2f(1,1); glVertex3f(+.25,0,-.1);
   glTexCoord2f(0,1); glVertex3f(+.25,0,+.1);
   glEnd();

   //left bottom
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(-1,-.25,0);
   glBegin(GL_QUADS);
   glNormal3f( -.4, -.6, 0);
   glTexCoord2f(0,0); glVertex3f(-.25,+.25,+.1);
   glTexCoord2f(1,0); glVertex3f(-.25,+.25,-.1);
   glTexCoord2f(1,1); glVertex3f(+.25,0,-.1);
   glTexCoord2f(0,1); glVertex3f(+.25,0,+.1);
   glEnd();

   //right bottom
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(1,0,0);
   glBegin(GL_QUADS);
   glNormal3f( .4, -.6, 0);
   glTexCoord2f(0,0); glVertex3f(-.25,0,+.1);
   glTexCoord2f(1,0); glVertex3f(-.25,0,-.1);
   glTexCoord2f(1,1); glVertex3f(+.25,.25,-.1);
   glTexCoord2f(0,1); glVertex3f(+.25,.25,+.1);
   glEnd();

   //left top
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(-1,.25,0);
   glBegin(GL_QUADS);
   glNormal3f( -.4, .6, 0);
   glTexCoord2f(0,0); glVertex3f(-.25,0,+.1);
   glTexCoord2f(1,0); glVertex3f(-.25,0,-.1);
   glTexCoord2f(1,1); glVertex3f(+.25,.25,-.1);
   glTexCoord2f(0,1); glVertex3f(+.25,.25,+.1);
   glEnd();

//bottom left
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(.25,-.5,0);
   glBegin(GL_QUADS);
   glNormal3f( -.6, -.4 ,0);
   glTexCoord2f(0,0); glVertex3f(+.25,-.25,+.1);
   glTexCoord2f(1,0); glVertex3f(+.25,-.25,-.1);
   glTexCoord2f(1,1); glVertex3f(+0,+.25,-.1);
   glTexCoord2f(0,1); glVertex3f(+0,+.25,+.1);
   glEnd();

   //bottom right
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(.25,0,0);
   glBegin(GL_QUADS);
   glNormal3f( .6, -.4 ,0);
   glTexCoord2f(0,0); glVertex3f(+0,-.25,+.1);
   glTexCoord2f(1,0); glVertex3f(+0,-.25,-.1);
   glTexCoord2f(1,1); glVertex3f(+.25,+.25,-.1);
   glTexCoord2f(0,1); glVertex3f(+.25,+.25,+.1);
   glEnd();

    //  Front square
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(0,.5,-.15);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-.25,-.25, .25);
   glTexCoord2f(1,0); glVertex3f(+.25,-.25, .25);
   glTexCoord2f(1,1); glVertex3f(+.25,+.25, .25);
   glTexCoord2f(0,1); glVertex3f(-.25,+.25, .25);
   glEnd();

   //back square
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glTranslatef(0,0,-.2);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, -1);
   glTexCoord2f(0,0); glVertex3f(-.25,-.25, .25);
   glTexCoord2f(1,0); glVertex3f(+.25,-.25, .25);
   glTexCoord2f(1,1); glVertex3f(+.25,+.25, .25);
   glTexCoord2f(0,1); glVertex3f(-.25,+.25, .25);
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   
}



void display()
{
   if (CameraPos)
      calc_position(CameraPos);
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

   if (mode){
      gluLookAt(x, 1.0f, z,x+lx, 1.0f,  z+lz,0.0f, 1.0f,  0.0f);
   }

   else
      {
         double Ex = -2*dim*Sin(th)*Cos(ph);
         double Ey = +2*dim        *Sin(ph);
         double Ez = +2*dim*Cos(th)*Cos(ph);
         gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
      }

    //smooth shading
   glShadeModel(GL_SMOOTH);
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);

//Draw stuff

   //bed
      double a = 0.04;
      double b = 0.3;
      double c = 0.05;

      //box frame
      box(1.02,.57,0 , .7,0.1,0.5 , 0);

      //head board
      cube(.30,.4,0 , .01,.4,0.5 , 0);

      //pillow
      pillow(.5,.67,0  , .15,.05,.3 , 0);

      //legs
      lightWood(1.65,.32,.4  , a,b,c , 0);
      lightWood(.35,.32,-.4  , a,b,c , 0);
      lightWood(1.65,.32,-.4 , a,b,c , 0);
      lightWood(.35,.32,.4  , a,b,c , 0);

      
      //box frame
      box(1.02,.57+.6,0 , .7,0.1,0.5 , 0);

      //head board
      cube(.30,.62+.55,0 , .01,.4,0.5 , 0);

      //pillow
      pillow(.5,.67+.6,0  , .15,.05,.3 , 0);

      //legs
      lightWood(1.65,.32+.6,.4  , a,b,c , 0);
      lightWood(.35,.32+.6,-.4  , a,b,c , 0);
      lightWood(1.65,.32+.6,-.4 , a,b,c , 0);
      lightWood(.35,.32+.6,.4  , a,b,c , 0);

      //ladder
      lightWood(1.3,.67,.55  , 0.02,.65,.04 , 0);
      lightWood(1.6,.67,.55  , 0.02,.65,.04 , 0);
      //rungs
      lightWood(1.45,.67,.55, 0.15,.03,.04 , 0);
      lightWood(1.45,.67*.4,.55, 0.15,.03,.04 , 0);
      lightWood(1.45,.67*.2,.55, 0.15,.03,.04 , 0);
      lightWood(1.45,.67*.6,.55, 0.15,.03,.04 , 0);
      lightWood(1.45,.67*.8,.55, 0.15,.03,.04 , 0);
      lightWood(1.45,.67*1.2,.55, 0.15,.03,.04 , 0);
      lightWood(1.45,.67*1.4,.55, 0.15,.03,.04 , 0);
      lightWood(1.45,.67*1.6,.55, 0.15,.03,.04 , 0);
      lightWood(1.45,.67*1.8,.55, 0.15,.03,.04 , 0);
     
     /* for(int k = 0; k < 3; k++) 
      {
         for (int l = -5; l < 5; ++l)
         {
            for (int dd = -5; dd < 5; ++dd)
            { 

               glPushMatrix();
               glTranslatef(l*2,k*2,dd*2);
               star(3,3,0,.2,.2,.2);
               glPopMatrix();
            }
         }

      }
      */
      star(3,1.5,0,1,1,1);


      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture[4]);
      for (int ii = -20; ii < 20; ++ii)
         for (int jj = -20; jj < 20; ++jj)
           {
              {
               glBegin(GL_QUADS);
               glNormal3f(0, +1, 0);
               glTexCoord2f(0,0); glVertex3f(-.5+ii,0, -.5+jj);
               glTexCoord2f(1,0); glVertex3f(-.5+ii,0, .5+jj);
               glTexCoord2f(1,1); glVertex3f(.5+ii ,0, .5+jj);
               glTexCoord2f(0,1); glVertex3f(.5+ii ,0, -.5+jj);
               glEnd();
              }
         
           }

      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE); 
      glBindTexture(GL_TEXTURE_2D,texture[6]);
      for (int ii = 0; ii < 4; ++ii){
         for (int jj = 0; jj < 4; ++jj){ 
            
            glBegin(GL_QUADS);
            glNormal3f(0, +1, 0);
            glTexCoord2f(0,0); glVertex3f(-.5+ii,.01, -.5+jj);
            glTexCoord2f(2,0); glVertex3f(-.5+ii,.01, .5+jj);      
            glTexCoord2f(2,2); glVertex3f(.5+ii ,.01, .5+jj);
            glTexCoord2f(0,2); glVertex3f(.5+ii ,.01, -.5+jj);
            glEnd();
         }
         }
         
         
      //glEnd();
      glDisable(GL_TEXTURE_2D);


   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);

   //  White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");

   }


   //**** Only used for error checking ****
   //int error = glGetError();
   //printf("%i\n",error );
  

   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   //  Print the text string
   Print("Angle=%d,%d",th,ph);
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }

   //  Render the scene
   ErrCheck("display");
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}


/*
 *  GLUT calls this routine when an arrow key is released
 */
 void releaseKey(int key, int x, int y) {    

        switch (key) {
             case GLUT_KEY_UP :
             case GLUT_KEY_DOWN : CameraPos = 0;break;
             case GLUT_KEY_LEFT : CameraPos = 0; break;
        }
}

//special glut call for camera
void special(int key,int x,int y)
{
  if(mode){

   switch (key) {
             case GLUT_KEY_UP : CameraPos = 0.5f; break;
             case GLUT_KEY_DOWN : CameraPos = -0.5f; break;
             case GLUT_KEY_LEFT :
                  angle -= 0.05f;
                  lx = sin(angle);
                  lz = -cos(angle);
                  break;
             case GLUT_KEY_RIGHT :
                  angle += 0.05f;
                  lx = sin(angle);
                  lz = -cos(angle);
                  break;
               }
         }

   else{
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
         th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
         th -= 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP)
         ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN)
         ph -= 5;
      //  PageUp key - increase dim
      else if (key == GLUT_KEY_PAGE_UP)
         dim += 0.1;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
         dim -= 0.1;
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
      //update projection
      Project(45,asp,dim);
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm')
      mode ^= 1;
   //for perspective and ortho views
   else if (ch == 'c')
      m ^= 1;
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+')
      rep++;
   else if (ch=='-' && rep>1)
      rep--;
   //  Toggle light movement
   else if (ch == 'i' || ch == 'I')
      move = 1-move;


   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Tell GLUT it is necessary to redisplay the scene
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (mode){
      gluPerspective(fov,asp,dim/4,dim*4);
   }
   else{
      //  Reproject
   Project(45,asp,dim);
   }
   glMatrixMode(GL_MODELVIEW);
   glutIdleFunc(move?idle:NULL);

   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
//if in 1st person we need a differant resize function
   if(mode){
   if (height == 0)
         height = 1;
      float ratio =  width * 1.0 / height;
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glViewport(0, 0, width, height);
      gluPerspective(45.0f, ratio, 0.1f, 100.0f);
      glMatrixMode(GL_MODELVIEW);
   }
//else were in ortho mode
   else{
      //  Ratio of the width to the height of the window
      asp = (height>0) ? (double)width/height : 1;
      //  Set the viewport to the entire window
      glViewport(0,0, width,height);
      //  Set projection
      Project(45,asp,dim);
   }
}


/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Evan Brookens - HW3");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   glutSpecialUpFunc(releaseKey);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);

   //  Load textures
   texture[0] = LoadTexBMP("crate.bmp");
   texture[1] = LoadTexBMP("img1.bmp");
   texture[2] = LoadTexBMP("img2.bmp");
   texture[3] = LoadTexBMP("img3.bmp");
   texture[4] = LoadTexBMP("img4.bmp");
   texture[5] = LoadTexBMP("img5.bmp");
   texture[6] = LoadTexBMP("img6.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
