# Linux (default)
LIBS = -lSDL -lGL -lGLU -lglut


INCPATH = -I /usr/X11R6/include/ -I /home/graco/gitHub/theCLAW/firstTests.
LIBPATH = -L/usr/lib -L/usr/X11R6/lib/
CXXFLAGS = -g -Wall

%:%.c
	gcc $(INCPATH) $(LIBPATH) $^ $(LIBS) $(CFLAGS) -o $@

%:%.cpp
	g++ $(INCPATH) $(LIBPATH) $^ $(LIBS) $(CXXFLAGS) -o $@

