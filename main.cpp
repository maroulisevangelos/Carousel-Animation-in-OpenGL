#include <stdio.h>           // Standard C/C++ Input-Output
#include <math.h>            // Math Functions
#include <windows.h>         // Standard Header For MSWindows Applications
#include <gl/glut.h>            // The GL Utility Toolkit (GLUT) Header
#include <time.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct vector3d
{
	float X, Y, Z;

	inline vector3d(void) {}
	inline vector3d(const float x, const float y, const float z)
	{
		X = x; Y = y; Z = z;
	}

	inline vector3d operator + (const vector3d& A) const
	{
		return vector3d(X + A.X, Y + A.Y, Z + A.Z);
	}

	inline vector3d operator + (const float A) const
	{
		return vector3d(X + A, Y + A, Z + A);
	}

	inline float Dot(const vector3d& A) const
	{
		return A.X * X + A.Y * Y + A.Z * Z;
	}
};


// Uncomment the following if you want to use the glut library from the current directory
#pragma comment(lib, "lib/glut32.lib")

// Global Variables
bool g_gamemode;				// GLUT GameMode ON/OFF
bool g_fullscreen;				// Fullscreen Mode ON/OFF (When g_gamemode Is OFF)
bool b_culling = false;			// Culling Enabled is Mandatory for this assignment do not change
float aspect = 1;

const float PI = 3.1415926535897932384626433832795028;
const float epsilon = 0.001;
const int n = 30; // Number of sides of the cylinder

//Function Prototypes
void render(void);
void initLights(void);
bool init(void);
void reshape(int w,int h);
void keyboard(unsigned char key, int x, int y);
void special_keys(int a_keys, int x, int y);



float x = 0.0f;     // Current x position for inside teapots
float x1 = 0.0f;	// Current x position for ouside teapots
float x2 = 0.0f;	// Current x position for inside horses
float x3 = 0.0f;	// Current x position for ouside horses
float angle_sun = 0.0f;	//angle for sun's rotation
float ad_sun = 0.1;	//control the rotation of sun
float angle_cam = 0.0f;	//angle for camera's rotation
float ad_cam = 0.01;	//control the rotation of camera
float camZ = 30.0f;		//using it to rotate the camera
float pivotZ = 0.0f;	//using it to rotate the camera
float angle_car = 0;	//angle for carousel's rotation
float ad_car = 0.2;	//control the rotation of carousel
float inc_speed = 1; //used to increase the speed of teapot's and horse's movement
float temp = 0;
float temp2 = 0;
int temp3 = 0;
int car_stop = 1; //used to stop or unstop the movement of teapots and horses
int side = 1; //used to change the side of the rotation
bool movingLightFlag = false;

void initLights(void) {

	glEnable(GL_LIGHT0);	
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);							   // Enable Lighting

}


// Our GL Specific Initializations. Returns true On Success, false On Fail.
bool init(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);             // Pixel Storage Mode To Byte Alignment
	glEnable(GL_TEXTURE_2D);                           // Enable Texture Mapping 
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);			   // Gray Background (CHANGED)
    glClearDepth(1.0f);								   // Depth Buffer Setup
    glDepthFunc(GL_LEQUAL);							   // The Type Of Depth Testing To Do
    glEnable(GL_DEPTH_TEST);						   // Enables Depth Testing
    glShadeModel(GL_SMOOTH);						   // Enable Smooth Shading
	initLights();
	glEnable(GL_COLOR_MATERIAL);					   // Enable Material Coloring
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Hint for nice perspective interpolation
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);	// Set the color tracking for both faces for both the ambient and diffuse components
	
	//// Set Specular
	GLfloat matSpec[] = { 0.1, 0.1,0.1,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	GLfloat shininess[] = { 64 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CCW);                               //Counter Clock Wise definition of the front and back side of faces
	glCullFace(GL_BACK);                               //Hide the back side
	
	//Change the light because of the sun
	/*GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	light_position[0] = 5.0f * cos(angle_sun);
	light_position[1] = 5.0f * sin(angle_sun);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);*/

	return true;
}

void positionCamera(){

	glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix

	glLoadIdentity();                // Reset The Projection Matrix
	gluPerspective(45.0f, aspect, 0.1, 100.0);
	
	camZ = pivotZ + 30.0 * cos(angle_cam); //make the camera rotate around the center (0,0,0)
	gluLookAt(0.0f, 5.5f, camZ, 0.0f, 0.0f, pivotZ, 0.0f, 1.0f, 0.0f); // position of the camera

	//camera transformations go here
	glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix

}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	// Calculate The Aspect Ratio And Set The Clipping Volume
	if (h == 0) h = 1;
	aspect = (float)w/(float)h;
	positionCamera();
	glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
	initLights();
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'c':
			if (ad_car == 0)
			{
				ad_car = 0.2;
				car_stop = 1;
				inc_speed = 1;
			}
			else {
				printf("The rotation is not stoped!\n");
			}
			break;
		case 's':
			if (ad_car != 0)
			{
				ad_car = 0;
				car_stop = 0;
			}
			else {
				printf("The rotation is already stoped!\n");
			}
			break;
		case 27:        // When Escape Is Pressed...
			exit(0);    // Exit The Program
			break;      // Ready For Next Case
		default:
		break;
	}
	glutPostRedisplay();
}

// Our Keyboard Handler For Special Keys (Like Arrow Keys And Function Keys)
void special_keys(int a_keys, int x, int y)
{

	switch (a_keys) {
		case GLUT_KEY_UP:
			if (ad_car < 1.5)
			{
				ad_car += 0.1;
				inc_speed += 0.5;
			}
			else {
				printf("This is the maximum speed!\n");
			}
			break;
		case GLUT_KEY_DOWN:
			if (ad_car > 0.2)
			{
				ad_car -= 0.1;
				inc_speed -= 0.5;
			}
			else {
				printf("This is the minimum speed!\n");
			}
			break;
		case GLUT_KEY_LEFT:
			if (side != -1)
			{
				side = -1;
			}
			else {
				printf("Now there is already left-rotation!\n");
			}
			break;
		case GLUT_KEY_RIGHT:
			if (side != 1)
			{
				side = 1;
			}
			else {
				printf("Now there is already right-rotation!\n");
			}
			break;
		default:
			;
	}

	glutPostRedisplay();
}

// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);                           // GLUT Initializtion
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); // (CHANGED)|
	if (g_gamemode) {
		glutGameModeString("1024x768:32");            // Select 1024x768 In 32bpp Mode
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
			glutEnterGameMode();                     // Enter Full Screen
		else
			g_gamemode = false;                     // Cannot Enter Game Mode, Switch To Windowed
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (!g_gamemode) {
		int screenSize[] = { glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT) };
		int windowsSize[] = { 1000,1000 };
		glutInitWindowPosition((screenSize[0]/2) - (windowsSize[0]/2), (screenSize[1] / 2) - (windowsSize[1] / 2));	// Window Position
		glutInitWindowSize(windowsSize[0], windowsSize[1]);	// Window Size
		glutCreateWindow("EPL426 - 1st Assignment");	// Window Title
	}
	if (!init()) {                                   // Our Initialization
		fprintf(stderr,"Initialization failed.");
		return -1;
	}
	glutDisplayFunc(render);                     // Register The Display Function
	glutReshapeFunc(reshape);                    // Register The Reshape Handler
	glutKeyboardFunc(keyboard);                  // Register The Keyboard Handler
	glutSpecialFunc(special_keys);               // Register Special Keys Handler
	glutSetCursor(GLUT_CURSOR_NONE);			 // Do not show the cursor
	glutIdleFunc(NULL);                        	 // We Do Rendering In Idle Time
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();                              // Go To GLUT Main Loop
	return 0;
}

void RenderSquare(vector3d center, float size)
{
	float halfSize = size/2;

	glBegin(GL_TRIANGLES);
		//triangle 1
		glColor3f(0.7f, 0.0f, 0.0f);
		glVertex3f(center.X + halfSize, center.Y + halfSize, center.Z);
		glVertex3f(center.X + halfSize, center.Y - halfSize, center.Z);
		glVertex3f(center.X - halfSize, center.Y + halfSize, center.Z);

		//triangle 2
		glColor3f(0.7f, 0.0f, 0.0f);
		glVertex3f(center.X - halfSize, center.Y + halfSize, center.Z);
		glVertex3f(center.X - halfSize, center.Y - halfSize, center.Z);
		glVertex3f(center.X + halfSize, center.Y - halfSize, center.Z);
	glEnd();
}

void RenderSquare2(vector3d point1, vector3d point2, vector3d point3, vector3d point4, vector3d color)
{
	glColor3f(color.X, color.Y, color.Z);
	glBegin(GL_TRIANGLES);
		//triangle 1
		glVertex3f(point1.X, point1.Y, point1.Z);
		glVertex3f(point2.X, point2.Y, point2.Z);
		glVertex3f(point3.X, point3.Y, point3.Z);

		//triangle 2
		glVertex3f(point2.X, point2.Y, point2.Z);
		glVertex3f(point3.X, point3.Y, point3.Z);
		glVertex3f(point4.X, point4.Y, point4.Z);
	glEnd();
}

void RenderCube(vector3d center, float size)
{
	float halfSize = size / 2;

	// Front
	vector3d frontLeftUp = center + vector3d(-halfSize, halfSize, halfSize);
	vector3d frontRightUp = center + vector3d(halfSize, halfSize, halfSize);
	vector3d frontLeftDown = center + vector3d(-halfSize, -halfSize, halfSize);
	vector3d frontRightDown = center + vector3d(halfSize, -halfSize, halfSize);

	// Back
	vector3d backLeftUp = center + vector3d(-halfSize, halfSize, -halfSize);
	vector3d backRightUp = center + vector3d(halfSize, halfSize, -halfSize);
	vector3d backLeftDown = center + vector3d(-halfSize, -halfSize, -halfSize);
	vector3d backRightDown = center + vector3d(halfSize, -halfSize, -halfSize);

	// Front
	RenderSquare2(frontLeftUp, frontRightUp, frontLeftDown, frontRightDown, vector3d(1,0,0));
	// Right
	RenderSquare2(frontRightUp, frontRightDown, backRightUp, backRightDown, vector3d(1, 1, 0));
	// Back
	RenderSquare2(backLeftUp, backRightUp, backLeftDown, backRightDown, vector3d(0, 1, 0));
	// Left
	RenderSquare2(frontLeftUp, frontLeftDown, backLeftUp, backLeftDown, vector3d(0, 1, 1));
	// Up
	RenderSquare2(frontLeftUp, frontRightUp, backLeftUp, backRightUp, vector3d(0, 0, 1));
	// Down 
	RenderSquare2(frontLeftDown, frontRightDown, backLeftDown, backRightDown, vector3d(1, 1, 1));
}

void drawCylinder(float radius, float height) {
	float angle = 2.0f * 3.1416 / n;

	// Draw the sides of the cylinder
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= n; i++) {
		float x = radius * cos(i * angle);
		float y = height / 2.0f;
		float z = radius * sin(i * angle);
		glNormal3f(x, 0, z);
		glVertex3f(x, y, z);
		glVertex3f(x, -y, z);
	}
	glEnd();

	// Draw the top and bottom faces of the cylinder
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	for (int i = 0; i < n; i++) {
		float x = radius * cos(i * angle);
		float z = radius * sin(i * angle);
		glVertex3f(x, height / 2.0f, z);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	for (int i = 0; i < n; i++) {
		float x = radius * cos(i * angle);
		float z = radius * sin(i * angle);
		glVertex3f(x, -height / 2.0f, z);
	}
	glEnd();
}

void Sun_Moon() {

	glEnable(GL_LIGHT1);
	// Set Specular
	GLfloat matSpec[] = { 0.1, 0.1,0.1,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	GLfloat shininess[] = { 64 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	GLfloat lightOn[4] = { 1,1,1,1 };
	GLfloat lightAmbientOn[4] = { 0.1,0.1,0.1,1 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbientOn);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOn);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightOn);

	//---Sun---

	//Movement of sun and moon
	glPushMatrix();
	glTranslatef(-0.5, -0.5, 0.0); // Move the object to the origin
	glRotatef(angle_sun, 0.0, 0.0, -1.0); // Rotate the object
	glTranslatef(0.5, 0.5, 0.0); // Move the object back to its original position

	//First Sphere
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 0.0f,0.8f); // yellow
	glTranslatef(0,24, 0);	//oposite position from moon
	glutSolidSphere(4, 150, 4);
	glPopMatrix();

	//Second Sphere
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 0.0f, 0.6f); // yellow
	glTranslatef(0, 24, 0);	//oposite position from moon
	glutSolidSphere(3, 150, 4);
	glPopMatrix();

	//Third Sphere
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 0.0f, 0.4f); // yellow
	glTranslatef(0, 24, 0);	//oposite position from moon
	glutSolidSphere(2, 150, 4);
	glPopMatrix();

	//Fourth Sphere
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 0.2f); // yellow
	glTranslatef(0, 24, 0);	//oposite position from moon
	glutSolidSphere(1, 150, 4);
	glPopMatrix();

	//---Moon----
	glPushMatrix();
	glColor3f(176,176,176); // grey
	glTranslatef(0, -24, 0);	//oposite position from sun
	glutSolidSphere(3, 150, 10);
	glPopMatrix();

	glPopMatrix();
}

void Flag() {
	//Flag
	glPushMatrix();
	glColor3f(0.7, 0, 0);	//red
	glTranslatef(1, 0, 0);	
	RenderSquare(vector3d(0, 15, 0), 2);
	glPopMatrix();
}

void Horse() {
	
	// Horse's body
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f);
	glRotatef(90, 0, 1, 0);
	glScalef(1.5f, 1.0f, 0.8f);
	glutSolidSphere(0.8, 50, 10);
	glPopMatrix();

	// Horse's tail
	glPushMatrix();
	glTranslatef(0, 0.5, -1.3f);
	glRotatef(-45, -1, 0, 0);
	glScalef(0.2f, 2.0f, 0.3f);
	glutSolidCube(0.6f);
	glPopMatrix();

	// Horse's neck
	glPushMatrix();
	glTranslatef(0, 1.2f, 1.0f);
	glRotatef(90, 0, -0.6,1);
	glScalef(1.5f, 0.6f, 0.4f);
	glutSolidSphere(0.7, 50, 10);
	glPopMatrix();

	// Horse's head
	glPushMatrix();
	glTranslatef(0.0f, 1.5f, 1.6);
	glRotatef(90, 0, 0.3, 1);
	glScalef(1.5f, 0.6f, 0.4f);
	glutSolidSphere(0.5, 50, 10);
	glPopMatrix();

	// Horse's Ears

	//Left Ear
	glPushMatrix();
	glTranslatef(-0.1f, 2.1f, 1.5);
	glRotatef(-15, 1, 0, 0);
	glScalef(0.2f, 2.0f, 0.3f);
	glutSolidCube(0.4f);
	glPopMatrix();

	//Right Ear
	glPushMatrix();
	glTranslatef(0.1f, 2.1f, 1.5);
	glRotatef(-15, 1, 0, 0);
	glScalef(0.2f, 2.0f, 0.3f);
	glutSolidCube(0.4f);
	glPopMatrix();


	// Horse's legs

	//Front left lrg
	glPushMatrix();
	glTranslatef(-0.4f, -0.5f, 0.4f);
	glScalef(0.2f, 2.0f, 0.3f);
	glutSolidCube(1.0f);
	glPopMatrix();

	//Back left leg
	glPushMatrix();
	glTranslatef(-0.4f, -0.5f, -0.4f);
	glScalef(0.2f, 2.0f, 0.3f);
	glutSolidCube(1.0f);
	glPopMatrix();

	//Front Right leg
	glPushMatrix();
	glTranslatef(0.4f, -0.5f, 0.4f);
	glScalef(0.2f, 2.0f, 0.3f);
	glutSolidCube(1.0f);
	glPopMatrix();

	//Back Right leg
	glPushMatrix();
	glTranslatef(0.4f, -0.5f, -0.4f);
	glScalef(0.2f, 2.0f, 0.3f);
	glutSolidCube(1.0f);
	glPopMatrix();

};

void Carousel() {

	glPushMatrix();

	//Movement of Carousel
	glTranslatef(0,0, 0); // move back to focus of gluLookAt
	glRotatef(angle_car ,0,side * 1,0); //  rotate around center
	glTranslatef(0,0,0); //move object to center
	
	//Central Cylinder
	glPushMatrix();
	glColor3f(0.1 , 0.5 , 1);	//light blue
	glTranslatef(0, 0, 0);
	drawCylinder(3.5, 14);
	glPopMatrix();

	//Base-Inside
	glPushMatrix();
	glColor3f(0, 1, 1);	//Turquoise
	glTranslatef(0, -(7), 0);
	drawCylinder(9, 1.5);
	glPopMatrix();

	//Base-Outside
	glPushMatrix();
	glColor3f(0.1, 0.5, 1);	//light blue
	glTranslatef(0, -(7+1.5), 0);
	drawCylinder(13, 1.5);
	glPopMatrix();

	//Top
	glPushMatrix();
	glColor3f(0.1, 0.5, 1);	//light blue
	glTranslatef(0, 7, 0);
	glRotatef(-90, 90, 0, 0);
	glutSolidCone(15, 6, 100, 6);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 1, 1);	//Turquoise
	glTranslatef(0, 6.5, 0);
	drawCylinder(15, 1);
	glPopMatrix();

	//Pole of Flag
	glPushMatrix();
	glColor3f(0, 1, 1);	//Turquoise
	glTranslatef(0, 14.5, 0);
	drawCylinder(0.3, 3);
	glPopMatrix();

	//Pole of Horse-Teapot
	
	//Inside Poles
	glPushMatrix();
	glColor3f(0.2, 0, 1);	//blue
	glTranslatef(0, 0, 7.5);
	drawCylinder(0.5, 13);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0, 1);	//blue
	glTranslatef(0, 0, -7.5);
	drawCylinder(0.5, 13);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1, 1, 0.6);	//light blue - turquoise
	glTranslatef(7.5, 0, 0);
	drawCylinder(0.5, 13);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1, 1, 0.6);	//light blue - turquoise
	glTranslatef(-7.5, 0, 0);
	drawCylinder(0.5, 13);
	glPopMatrix();

	//Outside Poles

	glPushMatrix();
	glColor3f(0.1, 1, 0.6);	//light blue - turquoise
	glTranslatef(0, 0, 11.5);
	drawCylinder(0.5, 15);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.1, 1, 0.6);	//light blue - turquoise
	glTranslatef(0, 0, -11.5);
	drawCylinder(0.5, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0, 1);	//blue
	glTranslatef(11.5, 0, 0);
	drawCylinder(0.5, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0, 1);	//blue
	glTranslatef(-11.5, 0, 0);
	drawCylinder(0.5, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0.2, 0.6);	//darker blue
	glTranslatef(8.75, 0, 6.75);
	drawCylinder(0.5, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0.2, 0.6);	//darker blue
	glTranslatef(-8.75, 0, -6.75);
	drawCylinder(0.5, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0.2, 0.6);	//darker blue
	glTranslatef(-8.75, 0, 6.75);
	drawCylinder(0.5, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0.2, 0.6);	//darker blue
	glTranslatef(8.75, 0, -6.75);
	drawCylinder(0.5, 15);
	glPopMatrix();

	//Teapots and Horses
	float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f * inc_speed; // Get elapsed time in seconds
	
	//Teapots

	//Inside Teapots

	//Sin-Movement for inside teapots
	x = car_stop*sin(t) * 3.0f; // Use sine function to calculate x position
	
	glPushMatrix();
	glColor3f(1, 1, 1);	//white
	glTranslatef(0, x, 7.5);
	glutSolidTeapot(2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.7, 0.3, 0.6);	//light purple
	glTranslatef(0, x, -7.5);
	glRotatef(180, 0, 1, 0);
	glutSolidTeapot(2);
	glPopMatrix();

	//Outside Teapots

	//Sin-Movement for outside teapots
	x1 = car_stop * sin(t) * 4.0f; // Use sine function to calculate x position
	
	glPushMatrix();
	glColor3f(0.6, 1, 0);	//light green
	glTranslatef(11.5, x1-1, 0);
	glRotatef(90, 0, 1, 0);
	glutSolidTeapot(2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.7, 0, 0);	//red
	glTranslatef(-11.5, x1-1, 0);
	glRotatef(-90, 0, 1, 0);
	glutSolidTeapot(2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9, 0, 0.6);	//pink
	glTranslatef(8.75, x1-1, 6.75);
	glRotatef( 45, 0, 1, 0);
	glutSolidTeapot(2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.6, 0.5);	//saumon
	glTranslatef(-8.75, x1-1, -6.75);
	glRotatef(-135, 0, 1, 0);
	glutSolidTeapot(2);
	glPopMatrix();

	//Horses

	//Inside Horses

	//Sin-Movement for inside horses
	x2 = -sin(t) * car_stop * 3.0f; // Use sine function to calculate x position

	glPushMatrix();
	glColor3f(0.6, 0.3, 0);	//brown
	glTranslatef(7.5, x2, 0);
	glRotatef(180, 0, 1, 0);
	glScalef(1.5, 1.5, 1.5);
	Horse();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0, 0.8);	//purple
	glTranslatef(-7.5, x2, 0);
	glScalef(1.5, 1.5, 1.5);
	Horse();
	glPopMatrix();

	//Outside Horses

	//Sin-Movement for outside horses
	x3 = -sin(t) * car_stop * 3.0f; // Use sine function to calculate x position

	glPushMatrix();
	glColor3f(0.5, 0.4, 0);	//olive colour
	glTranslatef(0, x3-1, 11.5);
	glRotatef(90, 0, 1, 0);
	glScalef(1.5, 1.5, 1.5);
	Horse();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.4, 0.2);	//darker green
	glTranslatef(0, x3-1, -11.5);
	glRotatef(-90, 0, 1, 0);
	glScalef(1.5, 1.5, 1.5);
	Horse();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0, 0.2);	//darker purple
	glTranslatef(8.75, x3-1, -6.75);
	glRotatef(-135, 0, 1, 0);
	glScalef(1.5, 1.5, 1.5);
	Horse();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.6, 0.4);	//Beige
	glTranslatef(-8.75, x3-1, 6.75);
	glRotatef(45, 0, 1, 0);
	glScalef(1.5, 1.5, 1.5);
	Horse();
	glPopMatrix();

	glPopMatrix();
}

// Our Rendering Is Done Here
void render(void)   
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	// Do we have culling enabled?
	if (b_culling == true)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	glLoadIdentity();

	//Movement of the sun
	

	angle_sun += ad_sun;	// Increment the angle of rotation for the sun
	angle_car += ad_car; // Increment the angle of rotation for the carousel
	angle_cam += ad_cam; // Increment the angle of rotation for the camera
	camZ = pivotZ + 30.0 * cos(angle_cam);
	glutPostRedisplay(); // Mark the window as needing to be redrawn

	glLoadIdentity();

#pragma region Carousel
	glScalef(0.5f, 0.5f, 0.5f);	//make everything look smaller
	Flag();	//create the Flag
	Carousel();	//create the whole carousel
	Sun_Moon();	//create sun and moon
#pragma endregion

    // Swap The Buffers To Make Our Rendering Visible
    glutSwapBuffers();
	glutPostRedisplay(); //animation
}