#include <GL\glut.h>
#include <math.h>
#include <stdlib.h>

#define GLT_PI 3.14159265358979323846

GLint w, h; // size of window
GLfloat fExtent; // XZ size of scene
GLfloat R = 3.0f;
GLfloat Radius;

bool Fly = false; // if it's flying
bool Fog = false; //if we want to have fog
float angle_x, angle_y; // Camera angles
float x, y, z; // XYZ camera
float lx, ly, lz; //XYZ point view of camera

GLfloat light_position[] = { x, y, z, 0.0 };

//draw net like ground
void DrawGround(void)
{
	GLfloat fStep = 1.0f;
	GLfloat yy = -0.4f;
	GLint iLine;
	
	//set black color of lines
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	//draw scene
	for (iLine = -fExtent; iLine <= fExtent; iLine += fStep)
	{
		glVertex3f(iLine, yy, fExtent);
		glVertex3f(iLine, yy, -fExtent);
		glVertex3f(fExtent, yy, iLine);
		glVertex3f(-fExtent, yy, iLine);
	}
	glEnd();
}

//set lighting and material
void typeOfLight(GLfloat mat_specular[], GLfloat mat_shininess[], GLfloat light[], GLfloat light_position[]){
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void Picture(void)
{
	//set fog
	if (Fog)
	{
		
		GLfloat fogcolor[] = { 0.5, 0.5, 0.5, 0.7 };
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogfv(GL_FOG_COLOR, fogcolor);
		glFogf(GL_FOG_DENSITY, 0.1);
		glHint(GL_FOG_HINT, GL_NICEST);
		glFogf(GL_FOG_START, 15.0);
		glFogf(GL_FOG_END, 100.0);
	}
	else glDisable(GL_FOG);

	//initialize arrays
	GLfloat mat_specular1[] = {1.0, 1.0, 1.0, 0.15};
	GLfloat mat_shininess[] = { 75.0 };
	GLfloat white_light[] = {0.0, 1.0, 1.0, 0.15};
	GLfloat light_position[] = { x, y, z, 0.0 };
	typeOfLight(mat_specular1, mat_shininess, white_light, light_position);
	
	//Sphere
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3ub(0, 255, 255);
	glTranslatef(0.0f, R, 0.0f);
	glutSolidSphere(R, 26, 26);
	glPopMatrix();
	
	//initialize arrays
	GLfloat mat_specular2[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat yellow_light[] = { 0.5, 0.0, 0.5, 0.15 };
	typeOfLight(mat_specular2, mat_shininess, yellow_light, light_position);
	
	//Teapot
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
	glTranslatef(-10.f, 0.5*R+0.75, -10.0f);
	glColor3ub(125, 0, 125);		
	glutSolidTeapot(R);
	glPopMatrix();
	
	//initialize arrays
	GLfloat mat_specular3[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat green_light[] = { 0.5, 1.0, 0.0, 0.15 };
	typeOfLight(mat_specular3, mat_shininess, green_light, light_position);
	
	//Torus
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef(10.f, 1.65*R, -10.0f);
	glColor3ub(125, 255, 0);		
	glutSolidTorus(0.5*R, R+1, 30, 20);
	glPopMatrix();

	//initialize array
	GLfloat mat_specular4[] = { 0.0, 0.0, 0.0, 0.75 };
	GLfloat light[] = { 1.0, 0.0, 0.0, 1.0 };
	typeOfLight(mat_specular4, mat_shininess, light, light_position);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//sphere 
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef(10.f, R, 10.0f);
	glColor3ub(255, 0, 0);
	glutSolidSphere(R, 26, 26);
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	
}

//calculate XYZ of camera
void changeXYZ(){
	x = (-Radius)*cos(angle_x)*sin(angle_y);
	y = Radius*cos(angle_y);
	z = (-Radius)*sin(angle_x)*sin(angle_y);
}

void RenderSceneSphereWorld()
{
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_POLYGON_BIT);
	glColor3ub(0, 0, 0);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, -10.0, 0.0f); 
	gluLookAt(x, y, z,
		lx, ly, lz,
		0.0f, 1.0, 0.0f);
	DrawGround();
	Picture();
	glutSwapBuffers();
	glPopAttrib();
}

//change size of screen
void ChangeSizeSphereWorld(int width, int height)
{
	w = width, h = height;
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, aspectRatio, 1.0f, 6.0f*fExtent);
	gluLookAt(x, y, z,
		lx, ly, lz,
		0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//timer which control fly function
void TimerFunc(int value)
{
	float fraction = 0.1f;
	if (Fly){
		//Radius = 1 * angle_x;
		Radius = 20 * cos(3 * angle_x);
		angle_x += fraction / GLT_PI;
		changeXYZ();
		glutPostRedisplay();
		glutTimerFunc(100, TimerFunc, 1);
	}

}

//set action for button
void Keys(unsigned char Key, int, int)
{

	if (Key == 27) exit(0);
	if (Key == 32) 
	{
		if (Fly) Fly = false;
		else Fly = true;
		if (Fly) glutTimerFunc(100, TimerFunc, 1);
		else glutPostRedisplay();
	}
	if (Key == 'F' || Key == 'f'){
		if (Fog) { 
			Fog = false;
			glDisable(GL_FOG);
		}
		else{ 
			Fog = true;
			glEnable(GL_FOG);
		}
		glutPostRedisplay();
	}
}

//if button is pressed change x or y or z
void processSpecialKeys(int key, int xx, int yy) {
	float fraction = 0.1f;
	
	switch (key) {

	case GLUT_KEY_LEFT:
		angle_x += fraction / GLT_PI;
		break;
	case GLUT_KEY_RIGHT:
		angle_x -= fraction / GLT_PI;
		break;
	case GLUT_KEY_UP:
		angle_y += fraction / GLT_PI;
		break;
	case GLUT_KEY_DOWN:
		angle_y -= fraction / GLT_PI;
		break;
	case GLUT_KEY_PAGE_UP:
		Radius -= fraction*10;
		break;
	case GLUT_KEY_PAGE_DOWN:
		Radius += fraction*10;
		break;
	}
	changeXYZ();
	glutPostRedisplay();

}

void SetupRC(void)
{

	glClearColor(0.5F, 0.5F, 0.5F, 1.0F);
	glEnable(GL_DEPTH_TEST); 
	fExtent = 15.0f; // size of scene [-fExtent..fExtent]
	Radius = fExtent;

	angle_x = GLT_PI / 2;
	angle_y = -GLT_PI / 2;
	changeXYZ();

	lx = ly = lz = 0; // point view
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("I like to move it, move it...");
	glutDisplayFunc(RenderSceneSphereWorld);
	glutReshapeFunc(ChangeSizeSphereWorld);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(Keys);
	SetupRC();

	glutMainLoop();
}