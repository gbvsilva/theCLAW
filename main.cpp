#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "obj.h"

// ROBOT ARM CONTROLS
GLfloat baseSpin = 0.0;        
GLfloat shoulderAng = -22.5;  
GLfloat elbowAng = -22.5;   
GLfloat wristAng = -22.5;
GLfloat wristRot = 0.0;
GLfloat fingerAng = 45.0;
GLfloat clawScale = 0.8;

// ROBOT COLORS
GLfloat red[] = {1.0, 0, 0};
GLfloat green[] = {0, 1.0, 0};
GLfloat weakGreen[] = {0, 0.2, 0};
GLfloat blue[] = {0, 0, 1.0};
GLfloat yellow[] = {1.0, 1.0, 0};
GLfloat darkYellow[] = {0.25, 0.25, 0};
GLfloat purple[] = {1.0, 0, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0};
GLfloat grey[] = {0.5, 0.5, 0.5};
GLfloat lightGrey[] = {0.75, 0.75, 0.75};
GLfloat darkGrey[] = {0.25, 0.25, 0.25};
GLfloat black[] = {0.0, 0.0, 0.0};

GLfloat blueLightGrey[] = {0.7, 0.75, 0.85};
GLfloat blueDarkGrey[] = {0.2, 0.25, 0.35};

// CAMERA CONTROLS
GLfloat eye[] = {0.0, 6.0, 6.0};
GLfloat center[] = {0.0, 1.0, 0.0};
GLfloat up[] = {0.0, 1.0, 0.0};
GLfloat camRot[] = {0.0, 0.0};
GLfloat shake[] = {0.0, 0.0, 0.0};
bool autocam = false;
bool autorotation = false;
bool lighttest = false;

// PHYSICS
GLfloat* catchPtDir = (GLfloat*)malloc(sizeof(GLint)*16);
GLfloat* catchPtEsq = (GLfloat*)malloc(sizeof(GLint)*16);
GLfloat* catchPtCenter = (GLfloat*)malloc(sizeof(GLint)*16);
GLfloat* catchPtWrist = (GLfloat*)malloc(sizeof(GLint)*16);
bool clawSwitch = false;
bool caught = false;
bool collision = false;
bool textureSwitch = false;

// CLAW AREA
GLfloat floorSize = 2.5;
GLfloat gridSize = 0.5;
GLfloat wallSize = 1;
GLfloat collectArea[] = {0.4, 1.0, 0.4, 1.0};

bool asd = true;
// LIGHT
bool light0 = false;
bool light1 = false;
bool light2 = false;
bool light3 = false;
// Luz 0: Difusa, acima do campo
GLfloat light0Pos[] = {0.0, 2.0, 0.0, 1.0};
GLfloat light0Intensity[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light0Specular[] = {0.0, 0.0, 0.0, 1.0};
// Luz 1: Ambiente
GLfloat light1Pos[] = {0.0, 0.0, 0.0, 0.0};
GLfloat light1Intensity[] = {0.4, 0.4, 0.4, 1.0};
// Luz 2: Especular, câmera
GLfloat light2Pos[] = {0, floorSize, floorSize, 1.0};
GLfloat light2Intensity[] = {1.0, 1.0, 1.0, 1.0};
// Luz 3: Spot difusa, acoplada à garra. Parte difusa não funcionando
GLfloat light3Pos[] = {0.0, 0.0, 0.0, 1.0};
GLfloat light3Intensity[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light3Dir[] = {0.0, 1.0, 0.0};

// TEXTURE
GLuint wallTexture; // Identificador da textura atualmente em uso
unsigned int width, height; // Altura x Largura da imagem de textura
unsigned char * data; // Dados da imagem de textura

// OBJECT LIST
Cube** cubeArray;
int totalCube = 1;
int cubeLeft = 1;
int maxCube = 3;
time_t start;
time_t end;

// PROTÓTIPOS
void lightConfig();
GLuint loadBMP(const char*);
GLfloat ambLight(GLfloat);
void setMaterial(GLfloat[3], GLfloat);
void drawUnitCylinder(int);
void drawUnitCone(int);
void drawUnitCube();
void drawUnitSphere(int);
void drawFloor();
void drawWalls();
void drawJoint(int);
void drawJoint2(int);
void drawBase(int);
void drawArmSegment(int);
void drawWrist(int);
void drawFingerBase(int);
void drawFingerTip(int);
void drawRobotArm(int);
void drawCube(Cube*);
void drawCubeArray();
void drawCatchSphere();
void display();
void reshape();
void idle();
void keyInput();
void lightConfig();
void createCatchPt();
void createCubeArray();
int main(int, char**);

GLfloat* ambLight(GLfloat* color){
	if(light1) return color;
	else return black;
}

void drawUnitCube(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glutSolidCube(1);
	glPopMatrix();
}

void drawUnitCylinder(int NumSegs)  // x,y,z in [0,1], Y-axis is up
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

void drawUnitSphere(int NumSegs)  // x,y,z in [0,1]
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glutSolidSphere(0.5f,NumSegs,NumSegs);
    glPopMatrix();
}

void drawFloor(){
    setMaterial(white, 128.0);
	
	// Carregando a textura 'wallTexture'
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, wallTexture);

    glBegin(GL_QUADS);
	    glNormal3f(0, 1, 0); // Vetor normal a superficie
	    /* Definido as coordenadas da textura e do piso (modelo 3D) fazendo o mapeamento correspondente. */
	    // Coordenada (0,0) da imagem eh mapeada para a coordenada (-floorSize, -floorSize) do piso 3D.
	    glTexCoord2f(0.0,0.0); 
	    glVertex3f(-floorSize,0,-floorSize);
	    glTexCoord2f(0.0,1.0);
	    glVertex3f(-floorSize,0,floorSize);
	    glTexCoord2f(1.0,1.0);
	    glVertex3f(floorSize,0,floorSize);
	    glTexCoord2f(1.0,0.0);
	    glVertex3f(floorSize,0,-floorSize);
    glEnd();
}

/* Desenhando as paredes */
void drawWalls(){
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		setMaterial(white, 128.0);

		// Carregando a textura 'wallTexture'
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		
		glBegin(GL_QUADS);
			// Parede do meio visivel
			glNormal3f(0, 0, -1);
			glTexCoord2f(0.0,1.0);
			glVertex3f(-floorSize, wallSize*2, floorSize);
			glTexCoord2f(1.0,1.0);
			glVertex3f(floorSize, wallSize*2, floorSize);
			glTexCoord2f(1.0,0.0);
			glVertex3f(floorSize, 0, floorSize);
			glTexCoord2f(0.0,0.0);
			glVertex3f(-floorSize, 0, floorSize);

			// Parede mais a direita
			glNormal3f(-1, 0, 0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(floorSize, wallSize*2, floorSize);
			glTexCoord2f(1.0,1.0);
			glVertex3f(floorSize, wallSize*2, -floorSize);
			glTexCoord2f(1.0,0.0);
			glVertex3f(floorSize, 0, -floorSize);
			glTexCoord2f(0.0,0.0);
			glVertex3f(floorSize, 0, floorSize);

			// Parede do meio (inicialmente, mais perto da camera)
			glNormal3f(0, 0, 1);
			glTexCoord2f(0.0,1.0);
			glVertex3f(floorSize, wallSize*2, -floorSize);
			glTexCoord2f(1.0,1.0);
			glVertex3f(-floorSize, wallSize*2, -floorSize);
			glTexCoord2f(1.0,0.0);
			glVertex3f(-floorSize, 0, -floorSize);
			glTexCoord2f(0.0,0.0);
			glVertex3f(floorSize, 0, -floorSize);

			// Parede mais a esquerda
			glNormal3f(1, 0, 0);
			glTexCoord2f(0.0,1.0);
			glVertex3f(-floorSize, wallSize*2, -floorSize);
			glTexCoord2f(1.0,1.0);
			glVertex3f(-floorSize, wallSize*2, floorSize);
			glTexCoord2f(1.0,0.0);
			glVertex3f(-floorSize, 0, floorSize);
			glTexCoord2f(0.0,0.0);
			glVertex3f(-floorSize, 0, -floorSize);
		glEnd();	
    glPopMatrix();
}

void armDetails(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		setMaterial(blueDarkGrey, 96.0);
		glTranslatef(0.0, 0.0, 0.0);
		glPushMatrix();
			glTranslatef(0.0, -0.25, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.15, 0.1);
			drawUnitCylinder(32);
		glPopMatrix();	
		
		glPushMatrix();			
			glScalef(0.1, 0.5, 0.15);
			drawUnitCube();
		glPopMatrix();	
		
		glPushMatrix();
			glTranslatef(0.0, 0.25, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.15, 0.1);
			drawUnitCylinder(32);
		glPopMatrix();
	glPopMatrix();
}

void drawArmAxle(){
	glMatrixMode(GL_MODELVIEW);
	setMaterial(darkGrey, 128.0);
	glPushMatrix();
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(0.1, 0.3, 0.1);
		drawUnitCylinder(6);
	glPopMatrix();
}

void drawArm(GLfloat rotation){
	glMatrixMode(GL_MODELVIEW);
	setMaterial(blueLightGrey, 96.0);
	glPushMatrix();
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(0.2, 0.1, 0.2);
		drawUnitCylinder(32);
	glPopMatrix();	
	glTranslatef(0.0, 0.4, 0.0);
	glPushMatrix();
		glScalef(0.2, 0.8, 0.1);
		drawUnitCube();
	glPopMatrix();
	
	// Detalhes do braço
	armDetails();
	
	setMaterial(blueLightGrey, 96.0);
	
	glTranslatef(0.0, 0.4, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glPushMatrix();
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(0.2, 0.1, 0.2);
		drawUnitCylinder(32);
	glPopMatrix();
}

void drawDAAAAACLAAAAAAAAAAW(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		// Base
		setMaterial(darkGrey, 128.0);
		
		glScalef(clawScale, clawScale, clawScale);
		glTranslatef(0.0, 0.05, 0.0);
		glPushMatrix();
			glRotatef(-120, 0.0, 1.0, 0.0);
			glTranslatef(0.0, -0.02, -0.4);
			glRotatef(-20, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.09, 0.2);
			drawUnitCube();
		glPopMatrix();
		glPushMatrix();
			glRotatef(120, 0.0, 1.0, 0.0);
			glTranslatef(0.0, -0.02, -0.4);
			glRotatef(-20, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.09, 0.2);
			drawUnitCube();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -0.02, -0.4);
			glRotatef(-20, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.09, 0.2);
			drawUnitCube();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -0.01, 0.0);
			glScalef(0.7, 0.09, 0.7);
			drawUnitCylinder(32);
		glPopMatrix();
		glRotatef(baseSpin, 0.0, 1.0, 0.0);
		
		setMaterial(white, 96.0);
		
		glPushMatrix();			
			glTranslatef(0.0, 0.05, -0.15);
			glRotatef(-20, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.1, 0.25);
			drawUnitCube();
		glPopMatrix();
		glPushMatrix();			
			glTranslatef(0.0, 0.05, 0.15);
			glRotatef(20, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.1, 0.25);
			drawUnitCube();
		glPopMatrix();
		glPushMatrix();			
			glScalef(0.5, 0.1, 0.5);
			drawUnitCylinder(32);
		glPopMatrix();		
		
		setMaterial(blueLightGrey, 96.0);
		
		glTranslatef(0.0, 0.4, -0.05);
		glPushMatrix();
			glScalef(0.2, 0.8, 0.1);
			drawUnitCube();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -0.05, 0.1);
			glScalef(0.2, 0.6, 0.1);
			drawUnitCube();
		glPopMatrix();
		glTranslatef(0.0, 0.4, 0.0);
		glRotatef(shoulderAng, 0.0, 0.0, 1.0);
		glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			glScalef(0.2, 0.1, 0.2);
			drawUnitCylinder(32);
		glPopMatrix();
		
		// Porca que segura os braços	
		glTranslatef(0.0, 0.0, 0.075);
		drawArmAxle();
		
		// Primeiro braço	
		glTranslatef(0.0, 0.0, 0.075);
		drawArm(elbowAng);
		
		// Porca que segura os braços
		glTranslatef(0.0, 0.0, -0.075);
		drawArmAxle();	
		
		// Segundo braço		
		glTranslatef(0.0, 0.0, -0.075);
		drawArm(wristAng);
				
		setMaterial(darkGrey, 128.0);
					
		// Garra
		glTranslatef(0.0, 0.0, 0.075);
		glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			glScalef(0.15, 0.3, 0.15);
			drawUnitCylinder(32);
		glPopMatrix();
		
		setMaterial(blueLightGrey, 96.0);
		
		glTranslatef(0.0, 0.25, 0.075);		
		glRotatef(wristRot, 0.0, 1.0, 0.0);
		glPushMatrix();
			glScalef(0.15, 0.5, 0.15);
			drawUnitCylinder(32);
		glPopMatrix();
		glTranslatef(0.0, 0.1, 0.0);
		
		setMaterial(darkGrey, 128.0);
		
		// WHY
		glPushMatrix();
			glRotatef(-wristRot*2+45, 0.0, 1.0, 0.0);
			glPushMatrix();			
				glTranslatef(0.0, -0.05, 0.0);
				glRotatef(90, 1.0, 0.0, 0.0);
				glScalef(0.05, 0.2, 0.05);
				drawUnitCube();
			glPopMatrix();
			glPushMatrix();			
				glTranslatef(0.0, -0.05, 0.0);
				glRotatef(90, 0.0, 0.0, 1.0);
				glScalef(0.05, 0.2, 0.05);
				drawUnitCube();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0, -0.15, 0.0);
				glRotatef(90, 1.0, 0.0, 0.0);
				glScalef(0.05, 0.2, 0.05);
				drawUnitCube();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0, -0.15, 0.0);
				glRotatef(90, 0.0, 0.0, 1.0);
				glScalef(0.05, 0.2, 0.05);
				drawUnitCube();
			glPopMatrix();
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(0.0, -0.1, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			glScalef(0.05, 0.2, 0.05);
			drawUnitCube();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -0.1, 0.0);
			glRotatef(90, 0.0, 0.0, 1.0);
			glScalef(0.05, 0.2, 0.05);
			drawUnitCube();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -0.2, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			glScalef(0.05, 0.2, 0.05);
			drawUnitCube();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, -0.2, 0.0);
			glRotatef(90, 0.0, 0.0, 1.0);
			glScalef(0.05, 0.2, 0.05);
			drawUnitCube();
		glPopMatrix();
		
		// Dedo 1
		glPushMatrix();
			glTranslatef(0.0, 0.0, -0.05);
			glRotatef(-fingerAng, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.15, 0.0);
			glPushMatrix();
				glScalef(0.05, 0.3, 0.05);
				drawUnitCube();
			glPopMatrix();	
			
			setMaterial(blueLightGrey, 96.0);
			
			glTranslatef(0.0, 0.15, 0.0);				
			glPushMatrix();
				glRotatef(90, 0.0, 0.0, 1.0);
				glScalef(0.1, 0.1, 0.1);
				drawUnitCylinder(32);
			glPopMatrix();
			
			setMaterial(darkGrey, 128.0);
		
			glRotatef(70, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.15, 0.0);	
			glPushMatrix();
				glScalef(0.05, 0.3, 0.05);
				drawUnitCube();
			glPopMatrix();			
		glPopMatrix();
				
		// Dedo 2
		setMaterial(darkGrey, 128.0);
		
		glPushMatrix();
			glTranslatef(-0.05, 0.0, 0.0);
			glRotatef(fingerAng, 0.0, 0.0, 1.0);
			glTranslatef(0.0, 0.15, 0.0);
			glPushMatrix();
				glScalef(0.05, 0.3, 0.05);
				drawUnitCube();
			glPopMatrix();	
			
			setMaterial(blueLightGrey, 96.0);
		
			glTranslatef(0.0, 0.15, 0.0);				
			glPushMatrix();
				glRotatef(90, 1.0, 0.0, 0.0);
				glScalef(0.1, 0.1, 0.1);
				drawUnitCylinder(32);
			glPopMatrix();
			
			setMaterial(darkGrey, 128.0);
		
			glRotatef(-70, 0.0, 0.0, 1.0);
			glTranslatef(0.0, 0.15, 0.0);	
			glPushMatrix();
				glScalef(0.05, 0.3, 0.05);
				drawUnitCube();
			glPopMatrix();
		glPopMatrix();
		
		// Dedo 3
		setMaterial(darkGrey, 128.0);
		
		glPushMatrix();
			glTranslatef(0.0, 0.0, 0.05);
			glRotatef(fingerAng, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.15, 0.0);
			glPushMatrix();
				glScalef(0.05, 0.3, 0.05);
				drawUnitCube();
			glPopMatrix();	
			
			setMaterial(blueLightGrey, 96.0);
		
			glTranslatef(0.0, 0.15, 0.0);				
			glPushMatrix();
				glRotatef(90, 0.0, 0.0, 1.0);
				glScalef(0.1, 0.1, 0.1);
				drawUnitCylinder(32);
			glPopMatrix();
			
			setMaterial(darkGrey, 128.0);
		
			glRotatef(-70, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.15, 0.0);	
			glPushMatrix();
				glScalef(0.05, 0.3, 0.05);
				drawUnitCube();
			glPopMatrix();
		glPopMatrix();
		
		// Dedo 4
		setMaterial(darkGrey, 128.0);
		
		glPushMatrix();
			glTranslatef(0.05, 0.0, 0.0);
			glRotatef(-fingerAng, 0.0, 0.0, 1.0);
			glTranslatef(0.0, 0.15, 0.0);
			glPushMatrix();
				glScalef(0.05, 0.3, 0.05);
				drawUnitCube();
			glPopMatrix();	
			
			setMaterial(blueLightGrey, 96.0);
					
			glTranslatef(0.0, 0.15, 0.0);				
			glPushMatrix();
				glRotatef(90, 1.0, 0.0, 0.0);
				glScalef(0.1, 0.1, 0.1);
				drawUnitCylinder(32);
			glPopMatrix();
			
			setMaterial(darkGrey, 128.0);	
				
			glRotatef(70, 0.0, 0.0, 1.0);
			glTranslatef(0.0, 0.15, 0.0);	
			glPushMatrix();
				glScalef(0.05, 0.3, 0.05);
				drawUnitCube();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void drawCube(Cube* cube){
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();    	
    	if(!clawSwitch){
    		caught = false;
    		cube->caught = false;
    	}    	
    	//Captura
    	else if(
    	catchPtCenter[12] > cube->pos[0]-cube->size &&
    	catchPtCenter[12] < cube->pos[0]+cube->size &&
    	catchPtCenter[13] > cube->pos[1]-cube->size &&
    	catchPtCenter[13] < cube->pos[1]+cube->size &&
    	catchPtCenter[14] > cube->pos[2]-cube->size &&
    	catchPtCenter[14] < cube->pos[2]+cube->size){    		
			if(caught && cube->caught){
				if(fingerAng > 45.0){
					clawSwitch = false;
					cube->caught = false;
				}
				else if(fingerAng < 45.0) fingerAng = 45.0;
				else{			
					cube->pos[0] = catchPtCenter[12];
			  		cube->pos[1] = catchPtCenter[13];
			  		cube->pos[2] = catchPtCenter[14];
			  	}
			}
			else if(!caught){
				cube->caught = true;
				caught = true;
				fingerAng = 45.0;
			}
    	}	
      	if(!cube->caught){
      		collision = false;
      		for(int i = 0; i < totalCube; i++){
      			if(
      			cubeArray[i] != cube &&
      			cube->pos[0] > cubeArray[i]->pos[0] - cubeArray[i]->size/2.0 - cube->size/2.0 + 0.01 &&
  				cube->pos[0] < cubeArray[i]->pos[0] + cubeArray[i]->size/2.0 + cube->size/2.0 - 0.01  &&
  				cube->pos[2] > cubeArray[i]->pos[2] - cubeArray[i]->size/2.0 - cube->size/2.0 + 0.01  &&
  				cube->pos[2] < cubeArray[i]->pos[2] + cubeArray[i]->size/2.0 + cube->size/2.0 - 0.01  &&
  				cube->pos[1] <= cubeArray[i]->pos[1] + cubeArray[i]->size/2.0 + cube->size/2.0 + 0.01  &&
  				cube->pos[1] > cubeArray[i]->pos[1]){
  					collision = true;
     			}
      		}
      		if(!collision){
      			if(cube->pos[1] > cube->size/2.0) cube->pos[1] -= 0.1;
      			else if(cube->pos[1] < cube->size/2.0){
      				cube->pos[1] = 0.1;
      				if(
      				cube->pos[0] > collectArea[0] &&
      				cube->pos[0] < collectArea[1] &&
      				cube->pos[2] > collectArea[2] &&
      				cube->pos[2] < collectArea[3]){
 						cube->type = 0;
 						cube->pos[1] = -10.0;
 						cubeLeft -= 1;
 					}
 				}      			
      		}
      	}
		glTranslatef(cube->pos[0], cube->pos[1], cube->pos[2]);
		glRotatef(cube->rot[0], 1.0, 0.0, 0.0);
		glRotatef(cube->rot[1], 0.0, 1.0, 0.0);
		glRotatef(cube->rot[2], 0.0, 0.0, 1.0);
		if(cube->caught) setMaterial(white, 96.0);
		else setMaterial(cube->clr, 128.0);
		glutSolidCube(cube->size);
    glPopMatrix();
}

void drawCubeArray(){
	for(int i = 0; i < totalCube; i++){
		if(cubeArray[i]->type != 0) drawCube(cubeArray[i]);
	}
}

void drawCatchSphere(){
	glMatrixMode(GL_MODELVIEW);
	setMaterial(red, 96.0);
	glPushMatrix();
		glTranslatef(catchPtCenter[12], catchPtCenter[13], catchPtCenter[14]);
		glutSolidSphere(0.02*clawScale, 16, 16);
	glPopMatrix();
}

void setMaterial(GLfloat color[3], GLfloat shininess){
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(color));
	glMaterialfv(GL_FRONT, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void drawPost(GLfloat height){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();	
		setMaterial(yellow, 32.0);
		glTranslatef(0.0, height/4, 0.0);
		glScalef(0.3, 0.3, 0.3);
		drawUnitSphere(32);
		
		setMaterial(darkGrey, 96.0);
		glTranslatef(0.0, -height/2, 0.0);
		glScalef(0.3, height, 0.3);
		drawUnitCylinder(32);
	glPopMatrix();
}

void drawPostGroup(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(2.0, 0.0, 2.0);
		for(int i = 0; i < 9; i++){
			glPushMatrix();
				glTranslatef(0.0, 0.0, -i*0.5);
				drawPost(2.0+i*0.5);
			glPopMatrix();		
		}
	glPopMatrix();
}

void drawCollectBox(){
	glMatrixMode(GL_MODELVIEW);
	setMaterial(yellow, 96.0);
	glPushMatrix();
		glTranslatef((collectArea[0]+collectArea[1])/2, 0.02, (collectArea[0]+collectArea[1])/2);
		glPushMatrix();
			glTranslatef(0.0, -0.01, 0.0);
			glScalef(6.2, 0.2, 6.2);
			glutSolidCube(0.1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 0.2, 0.3);
			glScalef(6.0, 4.0, 0.2);
			glutSolidCube(0.1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 0.2, -0.3);
			glScalef(6.0, 4.0, 0.2);
			glutSolidCube(0.1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.3, 0.2, 0.0);
			glScalef(0.2, 4.0, 6.0);
			glutSolidCube(0.1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.3, 0.2, 0.0);
			glScalef(0.2, 4.0, 6.0);
			glutSolidCube(0.1);
		glPopMatrix();
	glPopMatrix();	
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    	glLoadIdentity();
    	glScalef(clawScale, clawScale, clawScale);
		glTranslatef(0.0, 0.05, 0.0);		
		glRotatef(baseSpin, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.8, -0.05);	
		glRotatef(shoulderAng, 0.0, 0.0, 1.0);
		glTranslatef(0.0, 0.8, 0.15);
		glRotatef(elbowAng, 0.0, 0.0, 1.0);
		glTranslatef(0.0, 0.8, -0.15);
		glRotatef(wristAng, 0.0, 0.0, 1.0);
					
		// Garra
		glTranslatef(0.0, 0.2, 0.125);	
		glRotatef(wristRot, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.1, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, catchPtWrist);
		glLightfv(GL_LIGHT3, GL_POSITION, light3Pos);
		//glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3Dir);

		// Dedo 1
		glPushMatrix();
			glTranslatef(0.0, 0.0, -0.05);
			glRotatef(-fingerAng, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.3, 0.0);
			glRotatef(70, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.3, 0.0);	
			glGetFloatv(GL_MODELVIEW_MATRIX, catchPtDir);		
		glPopMatrix();
		
		// Dedo 2
		glPushMatrix();
			glTranslatef(0.0, 0.0, 0.05);
			glRotatef(fingerAng, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.3, 0.0);
			glRotatef(-70, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.3, 0.0);	
			glGetFloatv(GL_MODELVIEW_MATRIX, catchPtEsq);
		glPopMatrix();
		for(int i = 0; i < 16; i++) catchPtCenter[i] = (catchPtDir[i]+catchPtEsq[i])/2.0;
    glPopMatrix();
    
    glPushMatrix();
		glLoadIdentity();
		if(autocam) gluLookAt(
			catchPtWrist[12], 5.0, catchPtWrist[14],
			catchPtCenter[12], catchPtCenter[13], catchPtCenter[14],
			up[0], up[1], up[2]);
		else gluLookAt(
			eye[0]+shake[0], eye[1]+shake[1],
			eye[2]+shake[2], center[0], center[1],
			center[2], up[0], up[1], up[2]);		
		glRotatef(camRot[1], 1, 0, 0);
		glRotatef(camRot[0], 0, 1, 0);
		drawFloor();
		drawWalls();
		drawDAAAAACLAAAAAAAAAAW();
		drawCubeArray();
		drawCollectBox();
		drawCatchSphere();
		if(lighttest) drawPostGroup();
    glPopMatrix();	
    
    glutSwapBuffers();
}

void reshape(int w, int h){
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (float)w/h, 0.01, 50.0);
}

void idle(){
	shake[0] = 0;
	shake[1] = 0;
	shake[2] = 0;
	lightConfig();
	if(autorotation){
		camRot[0] += 0.5;
		if(camRot[0] > 360.0) camRot[0] -= 360.0;
	}
	if(cubeLeft == 0){
		totalCube += 2;
		if(totalCube > maxCube){
			end = time(NULL);
			printf("Tempo total:\n%.f segundos\n", difftime(end, start));
			exit(0);
		}
		cubeLeft = totalCube;
		createCubeArray();
	}
    glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y){
    switch(key){
	// GET OUT
	case 27:
	    exit(0);
	    break;
	// GET SOMETHING
	case 32:
		if(clawSwitch == false) clawSwitch = true;
		else clawSwitch = false;
		break;
    // BASE
	case 'a':
	    baseSpin += 4;
	    if(baseSpin > 360.0) baseSpin -= 360.0;
	    break;
	case 's':
	    baseSpin -= 4;
	    if(baseSpin < -360.0) baseSpin += 360.0;
	    break;
	// SHOULDER
	case 'r':
	    if(shoulderAng < 160.0) shoulderAng += 4;
	    break;
	case 'f':
	    if(shoulderAng > -160.0) shoulderAng -= 4;
	    break;
	// ELBOW
	case 't':
	    if(elbowAng < 160.0) elbowAng += 4;
	    break;
	case 'g':
	    if(elbowAng > -160.0) elbowAng -= 4;
	    break;
	// WRIST
	case 'y':
	    if(wristAng < 160.0) wristAng += 4;
	    break;
	case 'h':
	    if(wristAng > -160.0) wristAng -= 4;
	    break;
	case 'u':
	    wristRot += 4;
	    if(wristRot > 360.0) wristRot -= 360.0;
	    break;
	case 'j':
	    wristRot -= 4;
	    if(wristRot < -360.0) wristRot += 360.0;
	    break;
	case 'i':
	    wristRot += 66.6;
	    shake[0] = (float)(rand()%3)/7.5 - 0.2;
	    shake[1] = (float)(rand()%3)/7.5 - 0.2;
	    shake[2] = (float)(rand()%3)/7.5 - 0.2;
	    if(wristRot > 360.0) wristRot -= 360.0;
	    break;
	case 'k':
	    wristRot -= 66.6;
	    shake[0] = (float)(rand()%3)/7.5 - 0.2;
	    shake[1] = (float)(rand()%3)/7.5 - 0.2;
	    shake[2] = (float)(rand()%3)/7.5 - 0.2;
	    if(wristRot < -360.0) wristRot += 360.0;
	    break;
	// FINGERS
	case 'q':
	    if(fingerAng < 100.0){
			fingerAng += 5;
		}
	    break;
	case 'w':
		if(fingerAng > 35.0){
			fingerAng -= 4;
		}
	    break;
	// CAMERA CONTROLS
	case 'z':
	    if(!autocam) camRot[0] += 5;
		if(camRot[0] > 360.0) camRot[0] -= 360.0;
	    break;
	case 'x':
	    if(!autocam) camRot[0] -= 5;
		if(camRot[0] < -360.0) camRot[0] += 360.0;
	    break;
	case 'c':
	    if(!autocam && camRot[1] < 90.0) camRot[1] += 5;
	    break;
	case 'v':
	    if(!autocam && camRot[1] > -90.0) camRot[1] -= 5;
	    break;
	case 'e':
	    if(!autocam){
	    	eye[1] += 0.1;
	    	eye[2] += 0.1;
	    }
	    break;
	case 'd':
	    if(!autocam){
	    	eye[1] -= 0.1;
	    	eye[2] -= 0.1;
	    }
	    break;
	case 'm':
	    if(autocam) autocam = false;
	    else{
	    	autocam = true;
	    	camRot[0] = 0;
	    	camRot[1] = 0;
	    }	    	
	    break;
	case 'n':
	    if(autorotation){
	    	autorotation = false;
	    	light0Intensity[0] = 1.0;	
	    	light0Intensity[1] = 1.0;	
	    	light0Intensity[2] = 1.0;	
	    }
	    else{
	    	autorotation = true;	    	
	    	light0Intensity[0] = 0.2;	
	    	light0Intensity[1] = 0.2;	
	    	light0Intensity[2] = 0.2;
	    }	    	
	    break;
	// LIGHTING CONTROLS
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
	case '4':
	    if(light3 == true){
	    	glDisable(GL_LIGHT3);
	    	light3 = false;
	    }
	    else{
	    	glEnable(GL_LIGHT3);
	    	light3 = true;
	    }
	    break;
	case '8':
	    if(lighttest) lighttest = false;
	    else lighttest = true;
	    break;	    
	case '9':
    	if(light0Intensity[0] < 1.0){
    		light0Intensity[0] += 0.05;
    		light0Intensity[1] += 0.05;
    		light0Intensity[2] += 0.05;
    	}
    	break;
	case '0':
    	if(light0Intensity[0] > 0.0){
    		light0Intensity[0] -= 0.05;
    		light0Intensity[1] -= 0.05;
    		light0Intensity[2] -= 0.05;
    	}
    	break;
    }    
}

void lightConfig(){
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);	
	glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);	
	//glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 200.0);
	//glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30.0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Intensity);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1Intensity);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2Intensity);    
	//glLightfv(GL_LIGHT3, GL_DIFFUSE, light3Intensity);	
}

void createCubeArray(){
	if(cubeArray != NULL) delete[] cubeArray;
	cubeArray = new Cube*[totalCube];
	int type;
    for(int i = 0; i < totalCube; i++){
    	cubeArray[i] = new Cube(0.2);
    	cubeArray[i]->pos[0] = (GLfloat)(rand()%(int)(18))/10.0 - (0.9);
    	cubeArray[i]->pos[1] = 5.0 + (GLfloat)(rand()%6);
    	cubeArray[i]->pos[2] = (GLfloat)(rand()%(int)(18))/10.0 - (0.9);
    	cubeArray[i]->clr[0] = (GLfloat)(rand()%6)/10.0;
    	cubeArray[i]->clr[1] = (GLfloat)(rand()%6)/10.0;
    	cubeArray[i]->clr[2] = (GLfloat)(rand()%6)/10.0;
    	type = rand()%3+1;
    	cubeArray[i]->type = type;
    	if(type == 1) cubeArray[i]->clr[0] = 1.0;
    	else if(type == 2) cubeArray[i]->clr[1] = 1.0;
    	else cubeArray[i]->clr[2] = 1.0;
    }
}

void createCatchPt(){
	for (int i = 0; i < 16; i++){
    	catchPtDir[i] = 0;
    	catchPtEsq[i] = 0;
    	catchPtCenter[i] = 0;
    	catchPtWrist[i] = 0;
    }
}

/* Ler imagem BMP para aplicar textura. */
GLuint loadBMP(const char * path) 
{
	unsigned char header[54]; // Cada arquivo BMP comeca com um cabecalho desse tamanho.
	unsigned char dataPos; // Posicao no arquivo que esta sendo lida em determinado momento.
	
	unsigned int imageSize; // width*height*3;
	// RGB Data
	

	// Abrindo o arquivo da imagem
	FILE *file = fopen(path, "rb");
	if(!file) {
		printf("A imagem nao pode ser aberta!!\n");
		return 0;
	}

	// Lendo cabecalho para comprovar que se trata de um arquivo BMP
	if( fread(header, 1, 54, file) != 54 ) { // Cabecalho deve conter 54 bytes
		printf("Nao eh uma imagem BMP!!\n");
		return 0;
	}

	// Todo arquivo BMP inicia com a string 'BM'
	if( header[0]!='B' || header [1]!= 'M' ) {
		printf("Nao eh uma imagem BMP!!\n");
		return 0;
	}

	// Lendo o tamanho da imagem, largura, altura e localizacao dos dados no arquivo
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Caso o cabecalho nao contem imageSize e dataPos...
	if(imageSize == 0) imageSize=width*height*3;
	if(dataPos == 0) dataPos = header[54];

	// Alocando memoria para os dados RGB
	data = new unsigned char [imageSize];

	// Lendo dados do arquivo para o buffer acima
	fread(data,1,imageSize,file);

	// Fechando arquivo
	fclose(file);

	// Criando um identificador que ira referenciar a textura OpenGL
	GLuint id;
	glGenTextures(1, &id);
	
	// Passando o controle da textura para o controle do OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// Fazendo com que todas as funcoes de textura que serao aplicadas modificam a textura carregada
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return id; // Retornando o id para poder posteriormente usar a textura na hora de desenhar
}

int init(int argc, char** argv){
	if(argc > 1){
		autorotation = true;
		baseSpin = 45.0;
		glEnable(GL_LIGHT2);
		light2 = true;
    	if(atoi(argv[1]) == 1){			
			glEnable(GL_LIGHT0);
			light0 = true;
			light0Intensity[0] = 0.2;	
			light0Intensity[1] = 0.2;	
			light0Intensity[2] = 0.2;
		}
	}
	else{
		glEnable(GL_LIGHT0);
		light0 = true;
	}
}

int main(int argc, char** argv){
	srand(time(NULL));
	createCubeArray();
	createCatchPt();

	start = time(NULL);	
			
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(512,512);
    glutInitWindowPosition(180,100);
    glutCreateWindow("DAAAAAAA CLAAAAAAAAW");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);	
    glEnable(GL_TEXTURE_2D);
    wallTexture = loadBMP("text_wall.bmp");	
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);  
    
    init(argc, argv);  
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyInput);
       
    glutMainLoop();

    return EXIT_SUCCESS;
}
