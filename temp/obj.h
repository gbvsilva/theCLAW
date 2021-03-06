#ifndef OBJ
#define OBJ
#include <GL/glut.h>

class Obj{
	public:
	GLfloat pos[3];
	GLfloat rot[3];
	GLfloat clr[3];	
	Obj();
};

class Cube : public Obj{
	public:
	GLfloat size;
	Cube(GLfloat s);	
};

Obj::Obj(){
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;
	clr[0] = 1;
	clr[1] = 1;
	clr[2] = 1;
}

Cube::Cube(GLfloat s) : Obj::Obj(){
	size = s;
}

#endif
