#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "obj.h"

bool asd = true;
// ROBOT ARM CONTROLS
GLfloat BaseTransX=-0.5;
GLfloat BaseTransZ=0;
GLfloat BaseSpin=0;        
GLfloat ShoulderAng=-10;  
GLfloat ElbowAng=-120;   
GLfloat WristAng=90;
GLfloat WristRot=10;
GLfloat FingerAng1=45;
GLfloat FingerAng2=-45;

// ROBOT COLORS
GLubyte Arms[] = { 128,128,128 };
GLubyte Joints[] = { 0,68,119 };
GLubyte Fingers[] = { 150,0,24 };
GLubyte FingerJoints[] = { 128,128,128 };

GLubyte teapotColor[] = { 128,128,0 };

GLubyte Red[] = { 128,0,0 };
GLubyte Green[] = { 0,128,0 };
GLubyte Yellow[] = { 128,128,0 };
GLubyte Unk[] = { 0,128,128 };

// CAMERA CONTROLS
GLfloat eye[] = {0.0, 4.0, 4.0};
GLfloat center[] = {0.0, 1.0, 0.0};
GLfloat up[] = {0.0, 1.0, 0.0};

// PHYSICS
GLfloat* catchPtDir = (float*)malloc(sizeof(GLint)*16);
GLfloat* catchPtEsq = (float*)malloc(sizeof(GLint)*16);
GLfloat* catchPtCenter = (float*)malloc(sizeof(GLint)*16);


// LIGHT
bool light0 = false;
bool light1 = false;
bool light2 = false;
bool light3 = false;
GLfloat light0Pos[] = {0.0, 2.0, 2.0, 1.0};
GLfloat light1Pos[] = {0.0, 2.0, 0.0, 0.0};
GLfloat light2Pos[] = {0.0, 2.0, -2.0, 1.0};
GLfloat light3Pos[] = {0.0, 0.0, 0.0, 0.0};
GLfloat light0Intensity[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light1Intensity[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light2Intensity[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light3Intensity[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light0Dir[] = {0, -2, -2};

// FLOOR
float floorSize = 1.0;

// OBJECT LIST
Cube** cube;

// USER INTERFACE GLOBALS
int LeftButtonDown=0;    // MOUSE STUFF
int OldX,OldY,NewX,NewY;
int RobotControl=1;

void DrawUnitCylinder(int NumSegs)  // x,y,z in [0,1], Y-axis is up
{
    int i;
    float *Px = new float[NumSegs];
    float *Py = new float[NumSegs];
    float AngIncr = (2.0f*3.1415927f) / (float)NumSegs;
    float Ang = AngIncr;
    Px[0]=1;
    Py[0]=0;
    for (i=1; i<NumSegs; i++, Ang+=AngIncr)
    {
	Px[i]=(float)cos(Ang);
	Py[i]=(float)sin(Ang);
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(0.5f,0.5f,0.5f);
    glScalef(0.5f,0.5f,0.5f);

    // TOP
    glNormal3f(0,1,0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0,1,0);
    for (i=0; i<NumSegs; i++)
	glVertex3f(Px[i],1,-Py[i]);
    glVertex3f(Px[0],1,-Py[0]);
    glEnd();

    // BOTTOM
    glNormal3f(0,-1,0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0,-1,0);
    for (i=0; i<NumSegs; i++)
	glVertex3f(Px[i],-1,Py[i]);
    glVertex3f(Px[0],-1,Py[0]);
    glEnd();

    // SIDES
    glBegin(GL_QUAD_STRIP);
    for (i=0; i<NumSegs; i++)
    {
	glNormal3f(Px[i],0,-Py[i]);
	glVertex3f(Px[i],1,-Py[i]);
	glVertex3f(Px[i],-1,-Py[i]);
    }
    glNormal3f(Px[0],0,-Py[0]);
    glVertex3f(Px[0],1,-Py[0]);
    glVertex3f(Px[0],-1,-Py[0]);
    glEnd();

    glPopMatrix();

    delete[] Px;
    delete[] Py;
}

void DrawUnitSphere(int NumSegs)  // x,y,z in [0,1]
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(0.5f,0.5f,0.5f);
    glutSolidSphere(0.5f,NumSegs,NumSegs);
    glPopMatrix();
}

void DrawUnitCone(int NumSegs)  // x,y,z in [0,1], apex is in +Y direction
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glTranslatef(0.5f,0,0.5f);
		glRotatef(-90, 1,0,0);
		glutSolidCone(0.5f,1,NumSegs,NumSegs);
    glPopMatrix();
}

void DrawGroundPlane()
{
    glColor3f(0.7f,0.7f,0.7f);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    GLfloat gridSize = 0.1;
    for(GLfloat i = -floorSize; i < floorSize; i += gridSize){
    	for(GLfloat j = -floorSize; j < floorSize; j += gridSize){
			glVertex3f(i,0,j+gridSize);
			glVertex3f(i+gridSize,0,j+gridSize);
			glVertex3f(i+gridSize,0,j);
			glVertex3f(i,0,j);
		}
	}
    glEnd();
}

void DrawJoint(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glScalef(0.15f,0.15f,0.12f);
		glRotatef(90,1,0,0);
		glTranslatef(-0.5f,-0.5f,-0.5f);
		glColor3ubv(Joints);
		DrawUnitCylinder(NumSegs);
    glPopMatrix();
}

void MyDrawJoint(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glScalef(0.15f,0.15f,0.12f);
		glRotatef(90,1,0,0);
		glTranslatef(-0.5f,-0.5f,-0.5f);
		glColor3ubv(Unk);
		DrawUnitCylinder(NumSegs);
    glPopMatrix();
}

void DrawBase(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glScalef(0.2f,0.025f,0.2f);
		glTranslatef(-0.5f,0,-0.5f);
		glColor3ubv(Green);
		DrawUnitCylinder(NumSegs);
    glPopMatrix();
    glPushMatrix();
		glTranslatef(-0.05f,0,-0.05f);
		glScalef(0.1f,0.4f,0.1f);
		glColor3ubv(Yellow);
		DrawUnitCylinder(NumSegs);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0,0.4f,0);
		glColor3ubv(Unk);
		MyDrawJoint(NumSegs);
    glPopMatrix();
}

void DrawArmSegment(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glTranslatef(-0.05f,0,-0.05f);
		glScalef(0.1f,0.5f,0.1f);
		glColor3ubv(Arms);
		DrawUnitCylinder(NumSegs);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,0.5f,0);
		DrawJoint(NumSegs);
    glPopMatrix();
}

void DrawWrist(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glTranslatef(-0.04f,0,-0.04f);
		glScalef(0.08f,0.2f,0.08f);
		glColor3ubv(Fingers);
		DrawUnitCylinder(NumSegs);
    glPopMatrix();
    glPushMatrix();
		glTranslatef(0,0.2f,0);
		glScalef(0.12f,0.12f,0.12f);
		glTranslatef(-0.5f,-0.5f,-0.5f);
		glColor3ubv(FingerJoints);
		DrawUnitSphere(NumSegs);
    glPopMatrix();
}

void DrawFingerBase(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glTranslatef(-0.025f,0,-0.025f);
		glScalef(0.05f,0.3f,0.05f);
		glColor3ubv(Fingers);
		DrawUnitCylinder(NumSegs);
    glPopMatrix();
    glPushMatrix();
		glTranslatef(0,0.3f,0);
		glScalef(0.08f,0.08f,0.08f);
		glTranslatef(-0.5f,-0.5f,-0.5f);
		glColor3ubv(FingerJoints);
		DrawUnitSphere(NumSegs);
    glPopMatrix();
}

void DrawFingerTip(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glScalef(0.05f,0.25f,0.05f);
		glTranslatef(-0.5f,0,-0.5f);
		glColor3ubv(Fingers);
		DrawUnitCone(NumSegs);
    glPopMatrix();
}

void DrawRobotArm(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		/** Base of Arm **/
		glTranslatef(0,0,0);
		glRotatef(BaseSpin,0.0,1.0,0.0);
		DrawBase(NumSegs);

		/** Arm Segment #1 **/
		glTranslatef(0,0.4f,0);
		glRotatef(ShoulderAng,0.0,0.0,1.0);
		DrawArmSegment(NumSegs);

		/** Arm Segment #2 **/
		glTranslatef(0,0.5f,0);
		glRotatef(ElbowAng,0.0,0.0,1.0);
		DrawArmSegment(NumSegs);

		/** Wrist **/
		glTranslatef(0,0.5f,0);
		glRotatef(WristAng,0.0,0.0,1.0);
		glRotatef(WristRot,0.0,1.0,0.0);
		DrawWrist(NumSegs);
		
		glTranslatef(0,0.2f,0);    
		glRotatef(FingerAng1,0.0,0.0,1.0);
		DrawFingerBase(NumSegs);
		  
		glTranslatef(0, 0.3f, 0);
		glRotatef(-90, 0, 0, 1);
		DrawFingerTip(NumSegs);
		 
		glRotatef(90, 0, 0, 1);
		glTranslatef(0, -0.3f, 0);
		
		glRotatef(FingerAng2*2,0.0,0.0,1.0);
		DrawFingerBase(NumSegs);
		glTranslatef(0, 0.3f, 0);
		glRotatef(90, 0, 0, 1);
		DrawFingerTip(NumSegs);
    glPopMatrix();				
}

void drawCube(Cube* cube){
	if(cube->pos[1] > cube->size/2) cube->pos[1] -= 0.01;
	
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    	if(	catchPtCenter[12] > cube->pos[0]-cube->size &&
    		catchPtCenter[12] < cube->pos[0]+cube->size &&
    		catchPtCenter[13] > cube->pos[1]-cube->size &&
    		catchPtCenter[13] < cube->pos[1]+cube->size &&
    		catchPtCenter[14] > cube->pos[2]-cube->size &&
    		catchPtCenter[14] < cube->pos[2]+cube->size){
    		cube->clr[0] = 0.0;		
    	}
    	else{
    		cube->clr[0] = 1.0;
    	}
		glTranslatef(cube->pos[0], cube->pos[1], cube->pos[2]);
		glRotatef(cube->rot[0], 1.0, 0.0, 0.0);
		glRotatef(cube->rot[1], 0.0, 1.0, 0.0);
		glRotatef(cube->rot[2], 0.0, 0.0, 1.0);
		glColor3f(cube->clr[0], cube->clr[1], cube->clr[2]);
		glutSolidCube(cube->size);
    glPopMatrix();
}

void drawCubeArray(){
	for(int i = 0; i < 10; i++) drawCube(cube[i]);
}

void drawCatchSphere(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(catchPtDir[12], catchPtDir[13], catchPtDir[14]);
		glutSolidSphere(0.03, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(catchPtEsq[12], catchPtEsq[13], catchPtEsq[14]);
		glutSolidSphere(0.03, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(catchPtCenter[12], catchPtCenter[13], catchPtCenter[14]);
		glutSolidSphere(0.03, 10, 10);
	glPopMatrix();
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    	glLoadIdentity();
		glTranslatef(0,0,0);
		glRotatef(BaseSpin,0.0,1.0,0.0);
		glTranslatef(0,0.4f,0);
		glRotatef(ShoulderAng,0.0,0.0,1.0);
		glTranslatef(0,0.5f,0);
		glRotatef(ElbowAng,0.0,0.0,1.0);
		glTranslatef(0,0.5f,0);
		glRotatef(WristAng,0.0,0.0,1.0);
		glRotatef(WristRot,0.0,1.0,0.0);		
		glTranslatef(0,0.2f,0);
		glPushMatrix();
			glRotatef(FingerAng1,0.0,0.0,1.0);
			glTranslatef(0, 0.3f, 0);
			glRotatef(-90, 0, 0, 1);
			glTranslatef(0, 0.2f, 0);
			glGetFloatv(GL_MODELVIEW_MATRIX, catchPtDir);
		glPopMatrix();
		glPushMatrix();
			glRotatef(FingerAng2,0.0,0.0,1.0);
			glTranslatef(0, 0.3f, 0);
			glRotatef(90, 0, 0, 1);
			glTranslatef(0, 0.2f, 0);
			glGetFloatv(GL_MODELVIEW_MATRIX, catchPtEsq);
		glPopMatrix();
		for(int i = 0; i < 16; i++) catchPtCenter[i] = (catchPtDir[i]+catchPtEsq[i])/2.0;
    glPopMatrix();
    glPushMatrix();
		glLoadIdentity();
		gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
		DrawGroundPlane();
		DrawRobotArm(16);
		drawCubeArray();
		drawCatchSphere();
    glPopMatrix();    
    
    glutSwapBuffers();
}

void myReshape(int w, int h)
{
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30,(float)w/h,0.1,10);
}

void setCubeArray(){
	
}

void myIdle(){
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Intensity);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 100);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0Dir);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1Intensity);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2Intensity);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);    
    
    glutPostRedisplay();
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    switch(key){
	// GET OUT
	case 27:
	    exit(0);
	    break;
    // BASE
	case 'a':
	    BaseSpin += 4;
	    break;
	case 's':
	    BaseSpin -= 4;
	    break;
	// SHOULDER
	case 'r':
	    ShoulderAng += 4;
	    break;
	case 'f':
	    ShoulderAng -= 4;
	    break;
	// ELBOW
	case 't':
	    ElbowAng += 4;
	    break;
	case 'g':
	    ElbowAng -= 4;
	    break;
	// WRIST
	case 'y':
	    WristAng += 4;
	    break;
	case 'h':
	    WristAng -= 4;
	    break;
	case 'u':
	    WristRot += 4;
	    break;
	case 'j':
	    WristRot -= 4;
	    break;
	// FINGERS
	case 'q':
	    FingerAng1 += 4;
	    FingerAng2 -= 4;
	    break;
	case 'w':
	    FingerAng1 -= 4;
	    FingerAng2 += 4;
	    break;
	// CAMERA CONTROLS
	case 'z':
	    worldRot += 1;
	    break;
	case 'x':
	    worldRot -= 1;
	    break;
	//LIGHTING CONTROLS
	case 'e':
	    light0Dir[1] += 0.01;
	    break;
	case 'd':
	    light0Dir[1] -= 0.01;
	    break;
	// LIGHTING SWITCHS
	case '1':
	    if(light0 == true){
	    	glDisable(GL_LIGHT0);
	    	light0 = false;
	    }
	    else{
	    	glEnable(GL_LIGHT0);
	    	light0 = true;
	    }
	    break;
	case '2':
	    if(light1 == true){
	    	glDisable(GL_LIGHT1);
	    	light1 = false;
	    }
	    else{
	    	glEnable(GL_LIGHT1);
	    	light1 = true;
	    }
	    break;
	case '3':
	    if(light2 == true){
	    	glDisable(GL_LIGHT2);
	    	light2 = false;
	    }
	    else{
	    	glEnable(GL_LIGHT2);
	    	light2 = true;
	    }
	    break;
    }
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	
	cube = new Cube*[10];
    for(int i = 0; i < 10; i++){
    	cube[i] = new Cube(0.2);
    	cube[i]->pos[0] = (GLfloat)(rand()%18)/10.0 - 0.8;
    	cube[i]->pos[1] = 0.3 + (GLfloat)(rand()%10)/5;
    	cube[i]->pos[2] = (GLfloat)(rand()%18)/10.0 - 0.8;
    }
	
	for (int i = 0; i < 16; i++){
    	catchPtDir[i] = 0;
    	catchPtEsq[i] = 0;
    	catchPtCenter[i] = 0;
    }
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(512,512);
    glutInitWindowPosition(180,100);
    glutCreateWindow("The Robot Arm");

    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LINE_SMOOTH);
    	
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutIdleFunc(myIdle);

    glutKeyboardFunc(KeyboardFunc);
       
    glutMainLoop();

    return EXIT_SUCCESS;
}
