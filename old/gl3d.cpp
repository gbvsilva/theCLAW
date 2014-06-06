#include <GL/glut.h>

GLfloat rotation = 0.0f;

void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0, 0, -8, 0, 0, 0, 0, 1, 0);
    glRotatef(rotation, 1.0f, 0, 1.0f);

    glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f( 2, -0.5, 0);
	glVertex3f( 2, 0.5, 0);
	glVertex3f( -2, 0.5, 0);
	glVertex3f( -2, -0.5, 0);
    glEnd();

    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height){
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

void idle(){
    glutPostRedisplay();
}

void keyboardInput(unsigned char key, int x, int y){
    if (key == 'a') rotation -= 0.5f;
    else if(key == 'd') rotation += 0.5f;
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("The Claw");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutTimerFunc(0, timer, 0);
    
    glutKeyboardFunc(keyboardInput);
    
    glutMainLoop();
    
    return 0;
}
