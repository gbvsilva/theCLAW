#include "obj.h"

Obj::Obj(){
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;
	clr[0] = 0;
	clr[1] = 0;
	clr[2] = 0;
	spd = 0;
	caught = false;
}

Cube::Cube(GLfloat s) : Obj::Obj(){
	type = 0;
	size = s;
}
