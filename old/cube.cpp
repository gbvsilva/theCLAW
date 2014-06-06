#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

float xRotation = 0;
float yRotation = 0;
float zRotation = 0;

// CAMERA CONTROLS
float eye[] = {0.0, 8.0, 8.0};
float center[] = {0.0, 0.0, 0.0};
float up[] = {0.0, 1.0, 0.0};

void drawCube(){
    glPushMatrix();

    glRotatef(xRotation, 1.0, 0.0, 0.0);
    glRotatef(yRotation, 0.0, 1.0, 0.0);
    glRotatef(zRotation, 0.0, 0.0, 1.0);
    
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(1, 1, 1);
    glVertex3f(1, -1, 1);
    glVertex3f(1, -1, -1);
    glVertex3f(1, 1, -1);
    glNormal3f(0, 1, 0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(1, 1, 1);
    glVertex3f(1, 1, -1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1, 1);
    glNormal3f(0,0,1);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1, 1, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, -1, 1);
    glVertex3f(1, -1, 1);
    glNormal3f(-1,0,0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, -1, 1);
    glNormal3f(0,-1,0);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-1, -1, -1);
    glVertex3f(1, -1, -1);
    glVertex3f(1, -1, 1);
    glVertex3f(-1, -1, 1);
    glNormal3f(0,0,-1);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-1, -1, -1);
    glVertex3f(1, -1, -1);
    glVertex3f(1, 1, -1);
    glVertex3f(-1, 1, -1);
    glEnd();

    glPopMatrix();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawCube();

    glutSwapBuffers();
}

void reshape(int w, int h){
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, (float)w/h, 0.1, 30);
}

void idle(){
    glutPostRedisplay();
}

void input(unsigned char key, int x, int y){
    switch(key){
	case 27:
	    exit(0);
	    break;
	case 'a':
	    yRotation += 2;
	    break;
	case 's':
	    yRotation -= 2;
	    break;
	case 'w':
	    eye[2] *= -1;
	    break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(180, 100);
    glutCreateWindow("Cube");

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutKeyboardFunc(input);

    glutMainLoop();

    return EXIT_SUCCESS;
}
