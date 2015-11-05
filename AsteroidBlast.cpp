 
#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

#define GROUPS 10 //Number of groups of asteroids
#define ROWS 8  // Number of rows of asteroids.
#define COLUMNS 10 // Number of columns of asteroids.
#define FILL_PROBABILITY 20 // Percentage probability that a particular row-column slot will be 
                             // filled with an asteroid. It should be an integer between 0 and 100.

using namespace std;
							 
// Globals.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 0; // Co-ordinates of the spacecraft.
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static unsigned int spacecraft, space; // Display lists base index.
GLUquadricObj *myQuadric;  //the quadric

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
} 

// Asteroid class.
class Asteroid
{
public:
   Asteroid();
   Asteroid(float x, float y, float z, float r, unsigned char colorR, 
		   unsigned char colorG, unsigned char colorB);
   float getCenterX() { return centerX; }
   float getCenterY() { return centerY; }
   float getCenterZ() { return centerZ; }
   float getRadius()  { return radius; }
   void draw();

private:
   float centerX, centerY, centerZ, radius;
   unsigned char color[3];
};

// Asteroid default constructor.
Asteroid::Asteroid()
{
   centerX = 0.0;
   centerY = 0.0;
   centerZ = 0.0; 
   radius = 0.0; // Indicates no asteroid exists in the position.
   color[0] = 0;
   color[1] = 0;
   color[2] = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(float x, float y, float z, float r, unsigned char colorR, 
				  unsigned char colorG, unsigned char colorB)
{
   centerX = x;
   centerY = y;
   centerZ = z; 
   radius = r;
   color[0] = colorR;
   color[1] = colorG;
   color[2] = colorB;
}
	
// Function to draw asteroid.
void Asteroid::draw()
{
	myQuadric=gluNewQuadric();
	gluQuadricDrawStyle(myQuadric,GLU_FILL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   if (radius > 0.0) // If asteroid exists.
   {
      glPushMatrix();
      glTranslatef(centerX, centerY, centerZ);
      glColor3ubv(color);
	 gluSphere(myQuadric, radius, (int)radius*6, (int)radius*6);
      glPopMatrix();
   }
}

Asteroid arrayAsteroids[GROUPS][ROWS][COLUMNS];//Global Asteroids

// Initialization routine.
void setup(void) 
{
   int i, j;
   myQuadric=gluNewQuadric();
	gluQuadricDrawStyle(myQuadric,GLU_FILL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   spacecraft = glGenLists(1);
   glNewList(spacecraft, GL_COMPILE);
	 glPushMatrix();
	 glRotatef(180.0, xVal, zVal, 0.0); // To make the spacecraft point down the $z$-axis initially.
	 glColor3f (1.0, 1.0, 1.0);
      gluCylinder(myQuadric,3.0,0.0,9.0,10,10);
	 glPopMatrix();
   glEndList();

   space = glGenLists(1); 
   glNewList(space, GL_COMPILE);
	 glPushMatrix();
	 glRotatef(180.0, 0.0, 0.0, 0.0); // To make the space point down the $z$-axis initially.
	 glColor3f (1.0, 1.0, 1.0);
      gluCylinder(myQuadric,50.0,0.0,150.0,20,20);
	 glPopMatrix();
   glEndList();

   
      
   // Initialize global arrayAsteroids.
   constructAsteroids(0, 15.0, 30.0, 0.0, -40.0, 30.0, 3.0);
   constructAsteroids(1, 15.0, 20.0, -15.0, -40.0, 30.0, 3.0);
   constructAsteroids(2, 15.0, 20.0, 15.0, -40.0, 30.0, 3.0);
   constructAsteroids(3, 15.0, 30.0, 0.0, -40.0, 30.0, 3.0);
   constructAsteroids(4, 15.0, 20.0, -45.0, -40.0, 20.0, 3.0);
   constructAsteroids(5, 15.0, 30.0, 8.0, -40.0, 30.0, 3.0);
   constructAsteroids(6, 15.0, 20.0, -30.0, -40.0, 30.0, 3.0);
   constructAsteroids(7, 15.0, 20.0, 30.0, -40.0, 30.0, 3.0);
   constructAsteroids(8, 15.0, 20.0, 20.0, -40.0, 20.0, 3.0);
   constructAsteroids(9, 15.0, 20.0, -40.0, -40.0, 30.0, 3.0);

   glEnable(GL_DEPTH_TEST);
   glClearColor (0.0, 0.0, 0.0, 0.0);
}
void constructAsteroids(int Group, float Xnum1, float Xnum2, float Ynum, float Znum1, float Znum2, float Rnum){
	int i, j;
	// Initialize global arrayAsteroids.
	for (j = 0; j < COLUMNS; j++)
		for (i = 0; i < ROWS; i++)
			if (rand() % 100 < FILL_PROBABILITY)
				// If rand()%100 >= FILL_PROBABILITY the default constructor asteroid remains in the slot 
				// which indicates that there is no asteroid there because the default's radius is 0.
			{
		// Position the asteroids depending on if there is an even or odd number of columns
		// so that the spacecraft faces the middle of the asteroid field.
		if (COLUMNS % 2) // Odd number of columns.
			arrayAsteroids[Group][i][j] = Asteroid(Xnum2*(-COLUMNS / 2 + j), Ynum, Znum1 - Znum2*i, Rnum,
			rand() % 256, rand() % 256, rand() % 256);
		else // Even number of columns.
			arrayAsteroids[Group][i][j] = Asteroid(Xnum1 + Xnum2*(-COLUMNS / 2 + j), Ynum, Znum1 - Znum2*i, Rnum,
			rand() % 256, rand() % 256, rand() % 256);
			}
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1, 
						    float x2, float y2, float z2, float r2)
{
   return ( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) <= (r1+r2)*(r1+r2) );
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a)
{
   int g,i,j;

   // Check for collision with each asteroid.
   for (g = 0; g < GROUPS; g++)
	  for (j=0; j<COLUMNS; j++)
		 for (i=0; i<ROWS; i++)
			if (arrayAsteroids[g][i][j].getRadius() > 0 ) // If asteroid exists.
				if ( checkSpheresIntersection( x - 5 * sin( (PI/180.0) * a), 0.0, 
					z - 5 * cos( (PI/180.0) * a), 7.072, 
					arrayAsteroids[g][i][j].getCenterX(), arrayAsteroids[g][i][j].getCenterY(), 
					arrayAsteroids[g][i][j].getCenterZ(), arrayAsteroids[g][i][j].getRadius() ) )
				  return 1;
   
   return 0;
}

// Drawing routine.
void drawScene(void)
{  
   int i, j;
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // Begin viewport.
   glViewport(0, 0, width, height);
   glLoadIdentity();

 

    // Draw spacecraft.
   glPushMatrix();
   glTranslatef(xVal,zVal , 0.0);
   glRotatef(angle, xVal, zVal, 40.0);
   glCallList(spacecraft);
   glPopMatrix();
   
   //Draw space 
    glPushMatrix();
    glTranslatef(0.0,0.0, 0.0);
	glRotatef(180.0, 0.0, 0.0, 40.0); 
	glCallList(space);
	glPopMatrix();

   // Write text in isolated (i.e., before gluLookAt) translate block.
   glPushMatrix();
   glColor3f(1.0, 0.0, 0.0);
   glRasterPos3f(-28.0, 25.0, -30.0);
   if (isCollision) writeBitmapString((void*)font, "Cannot - will crash!");
   glPopMatrix();

   // Draw a vertical line on the left of the viewport to separate the two viewports
   glColor3f(1.0, 1.0, 1.0);
   glLineWidth(2.0);
   glBegin(GL_LINES);
      glVertex3f(-5.0, -5.0, -5.0);
      glVertex3f(-5.0, 5.0, -5.0);
   glEnd();
   glLineWidth(1.0);

   // Locate the camera at the tip of the cone and pointing in the direction of the cone.
   gluLookAt(xVal, 
	        zVal, 
			-10, 
	        xVal,
			zVal,
             -11, 
             0.0, 
			1.0, 
			0.0);

   // Draw all the asteroids in arrayAsteroids.
   drawAsteroids();
   // End right viewport.

   glutSwapBuffers();
}

void drawAsteroids(){
	int g, i, j;
	// Draw all the asteroids in arrayAsteroids.
	for (g = 0; g < GROUPS; g++)
		for (j = 0; j<COLUMNS; j++)
			for (i = 0; i<ROWS; i++)
				arrayAsteroids[g][i][j].draw();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
   glMatrixMode(GL_MODELVIEW);

   // Pass the size of the OpenGL window.
   width = w;
   height = h;
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

   // Compute next position.
   if (key == GLUT_KEY_LEFT) tempxVal -= 1.0; //tempAngle = angle + 5.0;
   if (key == GLUT_KEY_RIGHT) tempxVal += 1.0; //tempAngle = angle - 5.0;
   if( key == GLUT_KEY_UP) tempzVal += 1.0;
   if( key == GLUT_KEY_DOWN)tempzVal -= 1.0;

   // Move spacecraft to next position only if there will not be collision with an asteroid.
   if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle) )
   {
      isCollision = 0;
      xVal = tempxVal;
	 zVal = tempzVal;
	 angle = tempAngle;
   }
   else isCollision = 1;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left/right arrow keys to turn the craft." << endl
        << "Press the up/down arrow keys to move the craft." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize(400, 400);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("AsteroidBlast.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop(); 

   return 0;  
}

