all: obj.o claw.o
	g++ main.cpp obj.o claw.o -lglut -lGL -lGLU
obj.o:
	g++ -c obj.cpp -lglut -lGL -lGLU
claw.o:
	g++ -c claw.cpp -lglut -lGL -lGLU
