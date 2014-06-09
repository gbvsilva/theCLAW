#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "obj.h"

// ROBOT ARM CONTROLS
GLfloat baseTransX = -0.5;
GLfloat baseTransZ = 0.0;
GLfloat baseSpin = 0.0;        
GLfloat shoulderAng = -10.0;  
GLfloat elbowAng = -120.0;   
GLfloat wristAng = 90.0;
GLfloat wristRot = 10.0;
GLfloat fingerAng1 = 45.0;
GLfloat fingerAng2 = -45.0;

// ROBOT COLORSF
GLfloat red[] = {1.0, 0, 0};
GLfloat green[] = {0, 1.0, 0};
GLfloat weakGreen[] = {0, 0.2, 0};
GLfloat blue[] = {0, 0, 1.0};
GLfloat yellow[] = {1.0, 1.0, 0};
GLfloat weakYellow[] = {0.5, 0.5, 0};
GLfloat purple[] = {1.0, 0, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0};
GLfloat grey[] = {0.5, 0.5, 0.5};
GLfloat black[] = {0.0, 0.0, 0.0};

// CAMERA CONTROLS
GLfloat eye[] = {0.0, 4.0, 4.0};
GLfloat center[] = {0.0, 1.0, 0.0};
GLfloat up[] = {0.0, 1.0, 0.0};
GLfloat camRot[] = {0.0, 0.0};
bool autocam = false;

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

// LIGHT
bool light0 = false;
bool light1 = false;
bool light2 = false;
bool light3 = false;
// Luz 0: Difusa, acima do campo
GLfloat light0Pos[] = {0.0, 2.0, 0.0, 1.0};
GLfloat light0Intensity[] = {0.7, 0.7, 0.7, 1.0};
// Luz 1: Ambiente
GLfloat light1Pos[] = {0.0, 0.0, 0.0, 0.0};
GLfloat light1Intensity[] = {0.4, 0.4, 0.4, 1.0};
// Luz 2: Especular, câmera
GLfloat light2Pos[] = {0, floorSize, floorSize, 1.0};
GLfloat light2Intensity[] = {1.0, 1.0, 1.0, 1.0};
// Luz 3: Spot difusa, acoplada à garra
GLfloat light3Pos[] = {0.0, 0.0, 0.0, 1.0};
GLfloat light3Intensity[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light3Dir[] = {0.0, -1.0, 0.0};


// TEXTURE
GLuint wallTexture; // Identificador da textura atualmente em uso
unsigned int width, height; // Altura x Largura da imagem de textura
unsigned char * data; // Dados da imagem de textura


// OBJECT LIST
Cube** cubeArray;
int totalCube = 1;
int cubeLeft = 1;
int maxCube = 1;
clock_t start;
clock_t end;
time_t start2;
time_t end2;

// PROTÓTIPOS
void lightConfig();
GLuint loadBMP(const char*);
GLfloat ambLight(GLfloat);
void drawUnitCylinder(int);
void drawUnitCone(int);
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

GLfloat* ambLight(GLfloat* clr){
	if(light1) return clr;
	else return black;
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

void drawUnitSphere(int NumSegs)  // x,y,z in [0,1]
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(0.5f,0.5f,0.5f);
    glutSolidSphere(0.5f,NumSegs,NumSegs);
    glPopMatrix();
}

void drawUnitCone(int NumSegs)  // x,y,z in [0,1], apex is in +Y direction
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glTranslatef(0.5f,0,0.5f);
		glRotatef(-90, 1,0,0);
		glutSolidCone(0.5f,1,NumSegs,NumSegs);
    glPopMatrix();
}

void drawFloor(){
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(white));
	glMaterialfv(GL_FRONT, GL_SPECULAR, white); 
	glMaterialfv(GL_FRONT, GL_EMISSION, black);	  
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
	
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
		glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(grey));
		glMaterialfv(GL_FRONT, GL_SPECULAR, grey); 
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0);

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

void drawJoint(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glScalef(0.15f,0.15f,0.12f);
		glRotatef(90,1,0,0);
		glTranslatef(-0.5f,-0.5f,-0.5f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(black));
		glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
		drawUnitCylinder(NumSegs);
    glPopMatrix();
}

void drawJoint2(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glScalef(0.15f,0.15f,0.12f);
		glRotatef(90,1,0,0);
		glTranslatef(-0.5f,-0.5f,-0.5f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(black));
		glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
		drawUnitCylinder(NumSegs);
    glPopMatrix();
}

void drawBase(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glScalef(0.2f,0.025f,0.2f);
		glTranslatef(-0.5f,0,-0.5f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(black));
		glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
		drawUnitCylinder(NumSegs);
    glPopMatrix();
    glPushMatrix();
		glTranslatef(-0.05f,0,-0.05f);
		glScalef(0.1f,0.4f,0.1f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(white));
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 32.0);
		drawUnitCylinder(NumSegs);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0,0.4f,0);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(black));
		glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
		drawJoint2(NumSegs);
    glPopMatrix();
}

void drawArmSegment(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glTranslatef(-0.05f,0,-0.05f);
		glScalef(0.1f,0.5f,0.1f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(white));
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 32.0);
		drawUnitCylinder(NumSegs);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,0.5f,0);
		drawJoint(NumSegs);
    glPopMatrix();
}

void drawWrist(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glTranslatef(-0.04f,0,-0.04f);
		glScalef(0.08f,0.2f,0.08f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(red));
		glMaterialfv(GL_FRONT, GL_SPECULAR, red);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 32.0);
		drawUnitCylinder(NumSegs);
    glPopMatrix();
    glPushMatrix();
		glTranslatef(0,0.2f,0);
		glScalef(0.12f,0.12f,0.12f);
		glTranslatef(-0.5f,-0.5f,-0.5f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(black));
		glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
		drawUnitSphere(NumSegs);
    glPopMatrix();
}

void drawFingerBase(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glTranslatef(-0.025f,0,-0.025f);
		glScalef(0.05f,0.3f,0.05f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(red));
		glMaterialfv(GL_FRONT, GL_SPECULAR, red);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 32.0);
		drawUnitCylinder(NumSegs);
    glPopMatrix();
    glPushMatrix();
		glTranslatef(0,0.3f,0);
		glScalef(0.08f,0.08f,0.08f);
		glTranslatef(-0.5f,-0.5f,-0.5f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(black));
		glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 128.0);		
		drawUnitSphere(NumSegs);
    glPopMatrix();
}

void drawFingerTip(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		glScalef(0.05f,0.25f,0.05f);
		glTranslatef(-0.5f,0,-0.5f);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(red));
		glMaterialfv(GL_FRONT, GL_SPECULAR, red);
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
		glMaterialf(GL_FRONT, GL_SHININESS, 32.0);		
		drawUnitCone(NumSegs);
    glPopMatrix();
}

void drawRobotArm(int NumSegs)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
		/** Base of Arm **/
		glTranslatef(0,0,0);
		glRotatef(baseSpin,0.0,1.0,0.0);
		drawBase(NumSegs);

		/** Arm Segment #1 **/
		glTranslatef(0,0.4f,0);
		glRotatef(shoulderAng,0.0,0.0,1.0);
		drawArmSegment(NumSegs);

		/** Arm Segment #2 **/
		glTranslatef(0,0.5f,0);
		glRotatef(elbowAng,0.0,0.0,1.0);
		drawArmSegment(NumSegs);

		/** Wrist **/
		glTranslatef(0,0.5f,0);
		glRotatef(wristAng,0.0,0.0,1.0);
		glRotatef(wristRot,0.0,1.0,0.0);
		drawWrist(NumSegs);	
			
		/** Fingers **/
		glTranslatef(0,0.2f,0);    
		glRotatef(fingerAng1,0.0,0.0,1.0);
		drawFingerBase(NumSegs);
		  
		glTranslatef(0, 0.3f, 0);
		glRotatef(-90, 0, 0, 1);
		drawFingerTip(NumSegs);
		 
		glRotatef(90, 0, 0, 1);
		glTranslatef(0, -0.3f, 0);
		
		glRotatef(fingerAng2*2,0.0,0.0,1.0);
		drawFingerBase(NumSegs);
		glTranslatef(0, 0.3f, 0);
		glRotatef(90, 0, 0, 1);
		drawFingerTip(NumSegs);
    glPopMatrix();				
}

void drawCube(Cube* cube){
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    	/*
    	if(!clawSwitch){
    		caught = false;
    		cube->caught = false;
    	}    	
    	//Captura
    	if(
    	catchPtCenter[12] > cube->pos[0]-cube->size &&
    	catchPtCenter[12] < cube->pos[0]+cube->size &&
    	catchPtCenter[13] > cube->pos[1]-cube->size &&
    	catchPtCenter[13] < cube->pos[1]+cube->size &&
    	catchPtCenter[14] > cube->pos[2]-cube->size &&
    	catchPtCenter[14] < cube->pos[2]+cube->size){
    		if(clawSwitch){
    			if(caught && cube->caught){
					if(fingerAng1 > 45.0){
						clawSwitch = false;
						cube->caught = false;
					}
					else if(fingerAng1 < 45.0){
						fingerAng1 = 45.0;
						fingerAng2 = -45.0;
					}
					else{			
						cube->pos[0] = catchPtCenter[12];
				  		cube->pos[1] = catchPtCenter[13];
				  		cube->pos[2] = catchPtCenter[14];
				  	}
				}
				else if(!caught){
					cube->caught = true;
					caught = true;
					fingerAng1 = 45.0;
					fingerAng2 = -45.0;
				}
			}
    	}*/
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
				if(fingerAng1 > 45.0){
					clawSwitch = false;
					cube->caught = false;
				}
				else if(fingerAng1 < 45.0){
					fingerAng1 = 45.0;
					fingerAng2 = -45.0;
				}
				else{			
					cube->pos[0] = catchPtCenter[12];
			  		cube->pos[1] = catchPtCenter[13];
			  		cube->pos[2] = catchPtCenter[14];
			  	}
			}
			else if(!caught){
				cube->caught = true;
				caught = true;
				fingerAng1 = 45.0;
				fingerAng2 = -45.0;
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
      			if(cube->pos[1] > cube->size/2.0) cube->pos[1] -= 0.01;
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
		if(cube->caught){
			glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(green));
			glMaterialfv(GL_FRONT, GL_SPECULAR, green);
			glMaterialfv(GL_FRONT, GL_EMISSION, weakGreen);
			glMaterialf(GL_FRONT, GL_SHININESS, 32.0);
		}
		else{
			glMaterialfv(GL_FRONT, GL_DIFFUSE, cube->clr);
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(cube->clr));
			glMaterialfv(GL_FRONT, GL_SPECULAR, cube->clr);
			glMaterialfv(GL_FRONT, GL_EMISSION, black);
			glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
		}
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
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(white));
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glMaterialf(GL_FRONT, GL_SHININESS, 64.0);
	glPushMatrix();
		glTranslatef(catchPtDir[12], catchPtDir[13], catchPtDir[14]);
		glutSolidSphere(0.03, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(catchPtEsq[12], catchPtEsq[13], catchPtEsq[14]);
		glutSolidSphere(0.03, 10, 10);
	glPopMatrix();
	/*
	glPushMatrix();
		glTranslatef(catchPtCenter[12], catchPtCenter[13], catchPtCenter[14]);
		glutSolidSphere(0.03, 10, 10);
	glPopMatrix();
	*/
}

void drawCollectBox(){
	glMatrixMode(GL_MODELVIEW);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambLight(yellow));
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellow);
	glMaterialfv(GL_FRONT, GL_EMISSION, weakYellow);
	glMaterialf(GL_FRONT, GL_SHININESS, 64.0);
	glPushMatrix();
		glTranslatef((collectArea[0]+collectArea[1])/2, 0.02, (collectArea[0]+collectArea[1])/2);
		glPushMatrix();
			glTranslatef(0.0, -0.01, 0.0);
			glScalef(6.04, 0.2, 6.04);
			glutSolidCube(0.1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 0.0, 0.3);
			glScalef(6.0, 4.0, 0.2);
			glutSolidCube(0.1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 0.0, -0.3);
			glScalef(6.0, 4.0, 0.2);
			glutSolidCube(0.1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.3, 0.0, 0.0);
			glScalef(0.2, 4.0, 6.0);
			glutSolidCube(0.1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.3, 0.0, 0.0);
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
		glTranslatef(0,0,0);
		glRotatef(baseSpin,0.0,1.0,0.0);
		glTranslatef(0,0.4f,0);
		glRotatef(shoulderAng,0.0,0.0,1.0);
		glTranslatef(0,0.5f,0);
		glRotatef(elbowAng,0.0,0.0,1.0);
		glTranslatef(0,0.5f,0);
		glRotatef(wristAng,0.0,0.0,1.0);
		glRotatef(wristRot,0.0,1.0,0.0);		
		glTranslatef(0,0.2f,0);
		glPushMatrix();
			glTranslatef(0.0, 0.0, 0.2);
			glGetFloatv(GL_MODELVIEW_MATRIX, catchPtWrist);
		glPopMatrix();
		glLightfv(GL_LIGHT3, GL_POSITION, light3Pos);		
		glPushMatrix();
			glRotatef(fingerAng1,0.0,0.0,1.0);
			glTranslatef(0, 0.3f, 0);
			glRotatef(-90, 0, 0, 1);
			glTranslatef(0, 0.25f, 0);
			glGetFloatv(GL_MODELVIEW_MATRIX, catchPtDir);
		glPopMatrix();
		glPushMatrix();
			glRotatef(fingerAng2,0.0,0.0,1.0);
			glTranslatef(0, 0.3f, 0);
			glRotatef(90, 0, 0, 1);
			glTranslatef(0, 0.25f, 0);
			glGetFloatv(GL_MODELVIEW_MATRIX, catchPtEsq);
		glPopMatrix();
		for(int i = 0; i < 16; i++) catchPtCenter[i] = (catchPtDir[i]+catchPtEsq[i])/2.0;
    glPopMatrix();
    glPushMatrix();
		glLoadIdentity();
		if(autocam) gluLookAt(catchPtWrist[12], 5.0, catchPtWrist[14], catchPtCenter[12], catchPtCenter[13], catchPtCenter[14], up[0], up[1], up[2]);
		else gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);		
		glRotatef(camRot[1], 1, 0, 0);
		glRotatef(camRot[0], 0, 1, 0);
		drawFloor();
		drawWalls();
		drawRobotArm(16);
		drawCubeArray();
		drawCollectBox();
		drawCatchSphere();
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
	lightConfig();
	if(cubeLeft == 0){
		totalCube += 2;
		if(totalCube > maxCube){
			end = clock();
			end2 = time(NULL);
			printf("Tempo total:\n%d clocks\n%.f segundos\n", (int)(end-start), difftime(end2, start2));
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
	// FINGERS
	case 'q':
	    if(fingerAng1 < 100.0){
			fingerAng1 += 5;
			fingerAng2 -= 5;
		}
	    break;
	case 'w':
		if(fingerAng1 > 35.0){
			fingerAng1 -= 4;
			fingerAng2 += 4;
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
	    if(!autocam) camRot[1] += 5;
		if(camRot[1] > 360.0) camRot[0] -= 360.0;
	    break;
	case 'v':
	    if(!autocam) camRot[1] -= 5;
	    if(camRot[1] < -360.0) camRot[0] += 360.0;
	    break;
	case 'e':
	    if(!autocam) eye[2] += 0.1;
	    break;
	case 'd':
	    if(!autocam) eye[2] -= 0.1;
	    break;
	case 'm':
	    if(autocam) autocam = false;
	    else{
	    	autocam = true;
	    	camRot[0] = 0;
	    	camRot[1] = 0;
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
    }
}

void lightConfig(){
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);	
	glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);	
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 100.0);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 10.0);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3Dir);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Intensity);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1Intensity);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2Intensity);    
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3Intensity);	
}

void createCubeArray(){
	if(cubeArray != NULL) delete[] cubeArray;
	cubeArray = new Cube*[totalCube];
	int type;
    for(int i = 0; i < totalCube; i++){
    	cubeArray[i] = new Cube(0.2);
    	cubeArray[i]->pos[0] = (GLfloat)(rand()%(int)(18))/10.0 - (0.9);
    	cubeArray[i]->pos[1] = 0.3 + (GLfloat)(rand()%10)/5;
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

int main(int argc, char** argv){
	srand(time(NULL));
	createCubeArray();
	createCatchPt();

	start = clock();
	start2 = time(NULL);
	    
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
    	
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyInput);
       
    glutMainLoop();

    return EXIT_SUCCESS;
}
