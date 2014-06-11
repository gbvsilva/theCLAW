#ifndef OBJ
#define OBJ
#include <GL/glut.h>

class Obj{
	public:
	GLfloat pos[3];
	GLfloat rot[3];
	GLfloat clr[3];
	GLfloat spd;
	bool caught;
	Obj();
};

class Cube : public Obj{
	public:
	GLfloat size;
	int type;
	Cube(GLfloat s);	
};

#endif
