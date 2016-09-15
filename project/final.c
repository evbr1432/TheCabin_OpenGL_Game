/*Works Cited
http://gamejust4u.webs.com/apps/forums/topics/show/8127559-opengl-color-guide
https://gist.github.com/23ars/4545671
http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/
*/
/*
 *  3D Objects
 *
 *  Demonstrates how to draw objects in 3D.
 *If you get caught by running into an object, you have to 
 *back up and turn.(in proccess of fixing.)
 *  Key bindings:
 *  m          Go from model view to first person
 *  l          Toggle light
 *  []         Move light up and down
 *  a/A        Change ambient lighting
 *  d/D        Change difuse level
 *  s/S        Change specular level
 *  e/E        Change Emission level
 *  n/N        Change Shininess level
 *  Arrows     Move around
 *  page up    Raise Dim
 *  page down  Lower Dim
 *  arrows     Change view angle
 *  p          changes view
 *  0          Reset view angle
 *  ESC        Exit
 */
//#include "shapes.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "CSCIx229.h"
#define NUM_OBJ 100
int mode=1;       //  1st/view angle
int m = 0;        
int move=1;       //  Move light
int inc = 10;
int smooth = 1;   // Smooth/Flat shading
int local = 0;    // Local Viewer Model
int ntex=0;       //  Cube faces
int axes=0;       //  Display axes
int th=0;       //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
int p=0;          //  perspective or orthoganal
double dim=3.5;   // dimension
int fov = 60;     // feild of view
double asp = 1;   // Aspect ratio
double spc=5;     //  Specular intensity
double scale=.01; //  Image scale
float Ylight=2;   //  Light elevation

//new
float sco=15;    //  Spot cutoff angle
float Exp=50;      //  Spot exponent
int Th=0,Ph=30;   //  Light angles
int at0=100;      //  Constant  attenuation %
int at1=0;        //  Linear    attenuation %
int at2=0;        //  Quadratic attenuation %

int num       =   90;  // Number of quads
int distance  =   5;  // Light distance


int init = 1;
int obj_index = 0;


//start of collision detection
typedef struct _object
{
   float pos[2];
   float xrad;
   float zrad;
   float th;
}Object;

typedef struct _Player
{
   float pos[2];
   float col_radius;
   int   wall[4];
}Player;

Object obj_list[NUM_OBJ];
Player player1;


int obj;          //  Object display list
int obj1;          //  Object display list
int obj2;          //  Object display list
int obj3;          //  Object display list
int obj4;          //  Object display list
int obj5;          //  Object display list
int obj6;          //  Object display list
int obj7;          //  Object display list



int i,j;

// angle of rotation for the camera direction
float angle = 0.0f;
// vector representing the camera's direction
float lx=0.0f,lz=-1.0f,ly=0.0f;
// position of the camera
float x=12.0f, z=-23.0f, y=0.0f;

// the key states. These variables will be zero
//when no key is being presses
float CameraAngle = 0.0f;
float CameraPos = 0;
int xOrigin = -2;

// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  100;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int emission1  =   0;  // Emission intensity (%)
int ambient1   =  -10;  // Ambient intensity (%)
int diffuse1   = 0;  // Diffuse intensity (%)
int specular  =  0;  // Specular intensity (%)
int specular1  =  0;  // Specular intensity (%)
int lost = -1;



int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  10;  // Light azimuth
float ylight  =   .5;  // Elevation of light
float gr = 1;         //Ghost radius

int end = 0;         //game is over
int zh2 = 0;
int obtain = 0;
int draw_gob = 1;

unsigned int texture[11]; // Texture names

float RGBA[4] = {1,1,1,1};  //  Colors

void obj_setup(double x, double z, double dx, double dz, double th){

   obj_list[obj_index].pos[0]=x;
   obj_list[obj_index].pos[1]=z;
   obj_list[obj_index].th=th;
   obj_list[obj_index].xrad=dx;
   obj_list[obj_index].zrad=dz;
   obj_index++;

}

int collision = 0;
int test_player_col(Player* p, Object* o){
   float x1= abs(p->pos[0]) - abs(o->pos[0]);
   float z1= abs(p->pos[1]) - abs(o->pos[1]);
   float objth = (atan2(x1,z1)/PI)*180;
   if (abs(p->pos[0] - o->pos[0]) <= abs(p->col_radius + o->xrad*Cos(objth))){
      if (abs(p->pos[1] - o->pos[1]) <= abs(p->col_radius + o->zrad*Sin(objth))){
         
         printf("You hit something!\n" );   
         return 1;
      }
   }
   collision = 0;
   return 0;

}


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

//headboard
static void cube(double x,double y,double z,
 double dx,double dy,double dz,
 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};

   if (init){
      obj_setup(x,z,dx,dz,th);
   }
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
//beads
static void box(double x,double y,double z,
 double dx,double dy,double dz,
 double th)
{
   if (init){
      obj_setup(x,z,dx,dz,th);
   }
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
//pillow
static void pillow(double x,double y,double z,
 double dx,double dy,double dz,
 double th)
{
   if (init){
      obj_setup(x,z,dx,dz,th);
   }
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

//legs and ladder
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
   glBindTexture(GL_TEXTURE_2D,texture[9]);
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

//flashlight
static void ball(double x,double y,double z,double r)
{
   
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  Yellow ball
   //glColor3f(1,1,0);
   glutSolidSphere(.5,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

//front and back sides of house
static void side_house(double x, double y, double z, double dx, double dy, double dz, double th){
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
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
   glBindTexture(GL_TEXTURE_2D,texture[10]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glNormal3f( 0, 0, -1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
//left and right sides of house
static void side_house2(double x, double y, double z, double dx, double dy, double dz, double th){
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
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
   glBindTexture(GL_TEXTURE_2D,texture[10]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
//biggest trees
void big_tree(double x, double y, double z,double dx, double dy, double dz, double th){
   if (init){
         obj_setup(x,z,dx,dz,th);
      }
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();
         glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
         //glColor3f(1,1,1);
         glBindTexture(GL_TEXTURE_2D,texture[7]);
         glTranslated(x,y,z);
         glScaled(dx,dy,dz);
         glRotated(th,0,1,0);
         glCallList(obj1); 
      glPopMatrix();
       glDisable(GL_TEXTURE_2D);
      
}
void med_tree(double x, double y, double z,double dx, double dy, double dz, double th){

   glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
      glColor3f(1,1,1);
      glBindTexture(GL_TEXTURE_2D,texture[8]);
         //glColor3f(0,1,1);
      glTranslated(x,y,z);
      glScaled(scale,scale,scale);
      glRotated(th,0,1,0);
      glCallList(obj);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();
   }
void small_tree(double x, double y, double z,double dx, double dy, double dz, double th){
   glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
      glColor3f(1,1,1);
      glBindTexture(GL_TEXTURE_2D,texture[5]);
      glTranslated(x,y,z);
      glScaled(dx,dy,dz);
      glRotated(th,0,1,0);
      glCallList(obj2);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void rocks(double x, double y, double z){
   glPushMatrix();
      glEnable(GL_TEXTURE_2D);
     glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
      glColor3f(1,1,1);
      glBindTexture(GL_TEXTURE_2D,texture[5]);
      glTranslated(x,y,z);
      glScaled(.5,.5,.5);
      glRotated(0,0,1,0);
      glCallList(obj3);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}
void table(double x, double y, double z){
   glPushMatrix();
      glEnable(GL_TEXTURE_2D);
     glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
      glColor3f(1,1,1);
      glBindTexture(GL_TEXTURE_2D,texture[5]);
         //glColor3f(0,1,1);
      glScaled(.03,.03,.03);
      glTranslated(x,y,z);
      glCallList(obj4);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void ghost(double x, double y, double z,double dx, double dy, double dz, double th){

  if (init){
      obj_setup(x,z,dx,dz,th);
   }

   glPushMatrix();
      //  Offset, scale and rotate
      glTranslated(x,y,z);
      glScaled(dx,dy,dz);
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      //glColor3f(1,1,1);
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      glRotated(th,1,0,0);
      glCallList(obj5);
      glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}
void fence(double x, double y, double z,double dx, double dy, double dz, double th){

   glPushMatrix();

  if (init){
      obj_setup(x,z,dx,dz,th);
   }
      //  Offset, scale and rotate
      glTranslated(x,y,z);
      glRotated(th,0,1,0);
      glScaled(dx,dy,dz);
      //glColor3f(1,1,1);
      glCallList(obj6);
   glPopMatrix();
}
void fence2(double x, double y, double z,double dx, double dy, double dz, double th){

   glPushMatrix();

  if (init){
      obj_setup(x,z,dx,dz,th);
   }
      //  Offset, scale and rotate
      glTranslated(x,y,z);
      glScaled(dx,dy,dz);
      glRotated(th,0,1,0);
      glCallList(obj6);
   glPopMatrix();
}

void goblet(double x, double y, double z,double dx, double dy, double dz, double th){

   glPushMatrix();

  if (init){
      obj_setup(x,z,dx,dz,th);
   }

   glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      //glColor3f(1,1,1);
      glBindTexture(GL_TEXTURE_2D,texture[3]);
      //  Offset, scale and rotate
      glTranslated(x,y,z);
      glScaled(dx,dy,dz);
      glRotated(-90,1,0,0);
      glCallList(obj7);
       glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}
void begin(){
   if(player1.pos[1] <= -22){
      glWindowPos2d(100,300);
      Print("You awake to a noise in the woods.");
   }
   }
void story(){
   if(player1.pos[1] <= -20 && player1.pos[1] >=-22 )
   {
      glWindowPos2d(100,300);
      Print("A Demon Stirs About.");
   }
   }
void last(){
   if(player1.pos[1] <= -18 && player1.pos[1] >=-20 )
   {
      glWindowPos2d(100,300);
      Print("Find the goblet in time before the demon finds you.");
   }
   }


void obtained(){
   if (player1.pos[0] >= -12 && player1.pos[0] <= -10 )
         {
        
            if (player1.pos[1] >= 13 && player1.pos[1] <= 16 ){
            
               glWindowPos2d(100,300);
               Print("Kill The Demon!");
               obtain = 1;
               draw_gob = 0;
               end = 0;

            }
         }
}

void win(){
  
   glWindowPos2d(100,300);
   Print("You Won!!!! Press Arrow Keys:Left,Right,or Down to exit");
   printf("%s\n","You Won!!!" );
   end = 1;
   lost = 0;

}
void loss(){
  
   glWindowPos2d(100,300);
   Print("You Lost. Press Arrow Keys:Left,Right,or Down to exit");
   end = 1;
   lost=1;

}

void display()
{
   if (CameraPos)
      calc_position(CameraPos);

   const double len=1.5;  //  Length of axes

   

   float Position[]  = {lx+x,ylight,lz+z,1};
 
   float ti = glutGet(GLUT_ELAPSED_TIME)/10000.0;
   float mod = fmod(ti,8.5);
   //printf("%f\n",mod);

   float ghost_pos[]  = {(mod+gr)*Cos(zh),ylight,(mod+gr)*Sin(zh),.1};

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

   if (mode){
      gluLookAt(x, 1.0f, z, x+lx, 1.0f, z+lz,0.0f, 1.0f,  0.0f);
      player1.pos[0] = x;
      player1.pos[1] = z;
      player1.col_radius = .5;
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
      
      //  Spotlight color and direction
      float yellow[] = {1.0,1.0,0.0,1.0};
      float Direction[] = {lx,Sin(Th)*Sin(Ph),lz};
      
      //  Draw light position as ball (still no lighting here)
      //glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2],.1);

      //flash(lx+x,ylight-.1,lz+z,.1);
      //ball(lx+x,ylight,lz+z,1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);

      //  Set specular colors
      glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
      glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);

      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);

      //  Set spotlight parameters
      glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
      glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,sco);
      glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,Exp);

      //  Set attenuation
      glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,at2/100.0);

      //

      //  Translate intensity to color vectors
      float Ambient1[]   = {0.01*ambient1 ,0.01*ambient1 ,0.01*ambient1 ,1.0};
      float Diffuse1[]   = {0.01*diffuse1 ,0.01*diffuse1 ,0.01*diffuse1 ,1.0};
      float Specular1[]  = {0.01*specular1,0.01*specular1,0.01*specular1,1.0};
      
      float Position1[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};

      
      //  Draw light position as ball (still no lighting here)
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position1[0],Position1[1],Position1[2],0);
      //ball(lx+x,ylight,lz+z,1);
     //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
        //  Enable lighting
      glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
      glEnable(GL_LIGHT1);
        //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient1);
      glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse1);
      glLightfv(GL_LIGHT1,GL_SPECULAR,Specular1);
      glLightfv(GL_LIGHT1,GL_POSITION,Position1); 
   }

   else
      glDisable(GL_LIGHTING);



//Draw stuff

begin();
story();
last();
   //bed
   double a = 0.04;
   double b = 0.3;
   double c = 0.05;
      //legs
   float xx = 12;
   float zz = -25.5;

      //box frame
   box(1.02+xx,.57,0+zz , .7,0.1,0.5 , 0);

      //head board
   cube(.30+13.45,.4,0+zz , .01,.4,0.5 , 0);

      //pillow
   pillow(.5+13,.67,0+zz  , .15,.05,.3 , 0);


   lightWood(1.65+xx,.32,.4+zz  , a,b,c , 0);
   lightWood(.35+xx,.32,-.4+zz  , a,b,c , 0);
   lightWood(1.65+xx,.32,-.4+zz , a,b,c , 0);
   lightWood(.35+xx,.32,.4+zz  , a,b,c , 0);


      //box frame
   box(1.02+xx,.57+.6,0+zz , .7,0.1,0.5 , 0);

      //head board
   cube(.30+13.45,.62+.55,0+zz , .01,.4,0.5 , 0);

      //pillow
   pillow(.5+13,.67+.6,0+zz  , .15,.05,.3 , 0);

      //legs
   lightWood(1.65+xx,.32+.6,.4+zz  , a,b,c , 0);
   lightWood(.35+xx,.32+.6,-.4+zz  , a,b,c , 0);
   lightWood(1.65+xx,.32+.6,-.4+zz , a,b,c , 0);
   lightWood(.35+xx,.32+.6,.4+zz  , a,b,c , 0);

      //ladder
   lightWood(1.3+xx,.67,.55+zz  , 0.02,.65,.04 , 0);
   lightWood(1.6+xx,.67,.55+zz  , 0.02,.65,.04 , 0);
      //rungs
   lightWood(1.45+xx,.67    ,.55+zz, 0.15,.03,.04 , 0);
   lightWood(1.45+xx,.67* .4,.55+zz, 0.15,.03,.04 , 0);
   lightWood(1.45+xx,.67* .2,.55+zz, 0.15,.03,.04 , 0);
   lightWood(1.45+xx,.67* .6,.55+zz, 0.15,.03,.04 , 0);
   lightWood(1.45+xx,.67* .8,.55+zz, 0.15,.03,.04 , 0);
   lightWood(1.45+xx,.67*1.2,.55+zz, 0.15,.03,.04 , 0);
   lightWood(1.45+xx,.67*1.4,.55+zz, 0.15,.03,.04 , 0);
   lightWood(1.45+xx,.67*1.6,.55+zz, 0.15,.03,.04 , 0);
   lightWood(1.45+xx,.67*1.8,.55+zz, 0.15,.03,.04 , 0);


for (int i = 0; i < 11; ++i)
{
   big_tree(12.5+(i%2),0,-20+i*4,.01,.01,.01,0+i*.15);
}
for (int i = 0; i < 10; ++i)
{
   big_tree(5+(i%2),0,-15+i*4,.01,.01,.01,0+i*.15);
}
for (int i = 0; i < 13; ++i)
{
   big_tree(-5+(i%2),0,-27+i*4,.01,.01,.01,0+i*15);
}
for (int i = 0; i < 13; ++i)
{
   big_tree(-13.5+(i%2),0,-27+i*4,.01,.01,.01,0+i*15);
}

for (int i = 0; i < 8; ++i){
   for (int j= 0; j < 7; ++j){
      med_tree(-13.5+i*4,0,20-j*6,1,1,1,1);
   }
}


for (int i = 0; i < 10; ++i)
   for (int j= 0; j < 8; ++j)
{
   {
      small_tree(-13.5+i*3,0,20-j*5,.03,.03,.03,0);
   }
}

rocks(5,0,-5);
rocks(10,0,10);
rocks(-10,0,-15);
rocks(0,0,-20);
table(300,1,-800);

obtained();

if(!end)
{
   ghost(ghost_pos[0],ghost_pos[1], ghost_pos[2],.5,.5,.5,-50);
   float apprx_ghost_up0 = ghost_pos[0]+2;
   float apprx_ghost_dn0 = ghost_pos[0]-2;
   float apprx_ghost_up1 = ghost_pos[2]+2;
   float apprx_ghost_dn1 = ghost_pos[2]-2;
   if (apprx_ghost_up0 >= player1.pos[0] && apprx_ghost_dn0 <= player1.pos[0])
   {
      if (apprx_ghost_up1 >= player1.pos[1] && apprx_ghost_dn1 <= player1.pos[1])
      {
          if(obtain)
          {
            ghost(player1.pos[0]+lx*3,ghost_pos[1], player1.pos[1]+lz*3,1,1,1,-50);
            win();
          }
          else
          {
            ghost(player1.pos[0]+lx*3,ghost_pos[1], player1.pos[1]+lz*3,1,1,1,-50);
            loss();
          }
      }
   }
}


else{

   if(lost)
   {
      ghost(player1.pos[0]+lx*3,ghost_pos[1], player1.pos[1]+lz*3,2,2,2,-50);
      glWindowPos2d(100,300);
      Print("You Lost. Press Arrow Keys:Left,Right,or Down to exit");
   
   }
   if(!lost)
   {
      ghost(player1.pos[0]+lx*3,ghost_pos[1], player1.pos[1]+lz*3,.1,.1,.1,-50);
      glWindowPos2d(100,300);
      Print("You Won!!!! Press Arrow Keys:Left,Right,or Down to exit");
      printf("%s\n","You Won!!!" );
   }
}



if (draw_gob)
{
   goblet(-12,0,15,.5,.5,.5,90);
}


//positive x side
for (int i = 0; i < 12; ++i)
{
   fence(14,1,20.6-i*3.6,.75,.75,.75,.75);
}
//negitive x side
for (int i = 0; i < 14; ++i)
{
   fence(-14,1,20.25-i*3.6,.75,.75,.75,.75);
}
//postive z
for (int i = 0; i < 8; ++i)
{
   fence2(-12+i*3.6,1,21.75,.75,.75,.7,90);

}
//neg z
for (int i = 0; i < 6; ++i)
{
   fence2(-14.3+i*3.6,1,-26,.75,.75,.75,90);
}

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   for (int ii = -35; ii < 35; ++ii)
      for (int jj = -65; jj < 55; ++jj)
      {
       {
         glBegin(GL_QUADS);
         glNormal3f(0, +1, 0);
         glTexCoord2f(0,0); glVertex3f(-.2+ii*.4,0,-.2 +jj*.4);
         glTexCoord2f(1,0); glVertex3f(-.2+ii*.4,0, .2 +jj*.4);
         glTexCoord2f(1,1); glVertex3f(.2+ii *.4 ,0,.2 +jj*.4);
         glTexCoord2f(0,1); glVertex3f(.2+ii *.4 ,0,-.2+jj*.4);
         glEnd();
      }
      
   }

   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   for (int ii = -10; ii < 10; ++ii)
      for (int jj = -5; jj < 8; ++jj)
      {
       {
         glBegin(GL_QUADS);
         glNormal3f(0, +1, 0);
         glTexCoord2f(0,0); glVertex3f(10+-.2+ii*.4 ,.01,-.2 +-24+jj*.4);
         glTexCoord2f(1,0); glVertex3f(10+-.2+ii*.4 ,.01, .2 +-24+jj*.4);
         glTexCoord2f(1,1); glVertex3f(10+.2+ii *.4 ,.01,.2  +-24+jj*.4);
         glTexCoord2f(0,1); glVertex3f(10+.2+ii *.4 ,.01,-.2 +-24+jj*.4);
         glEnd();
      }
      
   }

   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,m?GL_REPLACE:GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[10]);
   for (int ii = -10; ii < 10; ++ii)
      for (int jj = -5; jj < 8; ++jj)
      {
       {
         glBegin(GL_QUADS);
         glNormal3f(0, +1, 0);
         glTexCoord2f(0,0); glVertex3f(10+-.2+ii*.4 ,3.2,-.2 +-24+jj*.4);
         glTexCoord2f(1,0); glVertex3f(10+-.2+ii*.4 ,3.2, .2 +-24+jj*.4);
         glTexCoord2f(1,1); glVertex3f(10+.2+ii *.4 ,3.2,.2  +-24+jj*.4);
         glTexCoord2f(0,1); glVertex3f(10+.2+ii *.4 ,3.2,-.2 +-24+jj*.4);
         glEnd();
      }
      
   }
  

      //glEnd();
   glDisable(GL_TEXTURE_2D);

   
   //front right(as entering)
   for (int i = 0; i < 18; ++i)
   {
      for (int j = 0; j < 16; ++j)
      {
         side_house(13.7-i*.2001,.1+j*.2001,-21.1,.1,.1,.1,1);
      }
   }
   
   //front left
   for (int i = 0; i < 18; ++i)
   {
      for (int j = 0; j < 16; ++j)
      {
         side_house(9.3-i*.2001,.1+j*.2001,-21.1,.1,.1,.1,1);
      }
   }
   
   //back
   for (float i = 0; i < 40; ++i)
   {
      for (float j = 0; j < 16; ++j)
      {
         side_house(13.7-i*.2001,.1+j*.2001,-26.3,.1,.1,.1,1);
      }
   }

//right side(when entering)
for (int i = 0; i < 16; ++i)
   {
      for (int j = 0; j < 26; ++j)
      {
         side_house2(13.9,.1+i*.2001,-26.1+j*.2001,-.1,.1,.1,1);
      }
   }
//left when entering
for (int i = 0; i < 16; ++i)
   {
      for (int j = 0; j < 26; ++j)
      {
         side_house2(5.9,.1+i*.2001,-26.1+j*.2001,-.1,.1,.1,1);
      }
   }

 

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


      ErrCheck("display");
      glFlush();
   //  Make the rendered scene visible
      glutSwapBuffers();

      init = 0;
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
if(mode)
{
   if(end)
   {
      switch (key)
             {
               case GLUT_KEY_DOWN : exit(0);
               case GLUT_KEY_LEFT : exit(0);
               case GLUT_KEY_RIGHT: exit(0);
             }
      glutPostRedisplay();
   }
   
     
   int i;

   player1.wall[0] = 0;
   player1.wall[1] = 0;
   player1.wall[2] = 0;
   player1.wall[3] = 0;

   if (player1.pos[0]>=12.5)
   {
      player1.wall[0] = 1;
   }
   if (player1.pos[0]<=-12.5)
   {
      player1.wall[1] = 1; 
   }
   if (player1.pos[1]<=-24.5)
   {
      player1.wall[2] = 1;
   }
   if (player1.pos[1]>=20)
   {
      player1.wall[3] = 1; 
   }


   for (i = 0; i < obj_index; ++i)
   {
      collision=test_player_col(&player1, &obj_list[i]);

      if(collision||player1.wall[0] || player1.wall[1]|| player1.wall[2]|| player1.wall[3] )
      {
         if (player1.pos[0] >= .5 && player1.pos[0] <= 2 )
         {
            if (player1.pos[1] >= 1.5 && player1.pos[1] <= 2 )
            {
               printf("%s\n","You Won!");
            }
         }
            //printf("%i\n",collision );
            //printf("%f\n",collision );
            while(GLUT_KEY_UP)
            {
               break;
            }
            if(GLUT_KEY_DOWN){
               CameraPos = -1.0f;break;
            }
            else{
               break;
            }

            
      }

      else
         {

            switch (key) 
            {
              case GLUT_KEY_UP : CameraPos = 1.0f; break;
              case GLUT_KEY_DOWN : CameraPos = -1.0f; break;
              case GLUT_KEY_LEFT :
              angle -= 0.001f;
              lx = sin(angle);
              lz = -cos(angle);
              break;
              case GLUT_KEY_RIGHT :
              angle += 0.001f;
              lx = sin(angle);
              lz = -cos(angle);
              break;
            }
         }
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

   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Tell GLUT it is necessary to redisplay the scene
   glMatrixMode(GL_PROJECTION);
   //  Wrap angles
   Th = Th%360;
   Ph = Ph%360;
   glLoadIdentity();

   if (mode){
      gluPerspective(fov,asp,dim/4,dim*20);
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
   glutInitWindowSize(1000,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Evan Brookens - Final Project - The Cabin");
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

  if (Mix_OpenAudio(44100,AUDIO_S16SYS,2,4096)) Fatal("Cannot initialize audio\n");
   Mix_Music* music;
   music = Mix_LoadMUS("music.ogg");
   if (!music) Fatal("Cannot load music.ogg\n");
   Mix_PlayMusic(music,-1);
   
   
   
   obj = LoadOBJ("DeadTree1.obj");
   obj1 = LoadOBJ("DeadTree21.obj");
   obj2 = LoadOBJ("3d-model.obj");
   obj3 = LoadOBJ("Rock_6.OBJ");
   obj4 = LoadOBJ("table.obj");
   obj5 = LoadOBJ("ghost.obj");
   obj6 = LoadOBJ("fence.obj");
   obj7 = LoadOBJ("goblet.obj");

   //  Load textures

   texture[0] = LoadTexBMP("crate.bmp");
   texture[1] = LoadTexBMP("spooder.bmp");
   texture[2] = LoadTexBMP("scary_sheets.bmp");
   texture[3] = LoadTexBMP("img5.bmp");
   texture[4] = LoadTexBMP("grass.bmp");
   texture[5] = LoadTexBMP("dead_tree_bark.bmp");
   texture[6] = LoadTexBMP("img6.bmp");
   texture[7] = LoadTexBMP("dead_tree3.bmp");
   texture[8] = LoadTexBMP("dead_tree2.bmp");
   texture[9] = LoadTexBMP("bad_wood.bmp");
   texture[10] = LoadTexBMP("darkwood.bmp");


   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   //  Shut down SDL
   Mix_CloseAudio();
   SDL_Quit();
   return 0;
}
