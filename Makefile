CPPFLAGS	= -I/usr/X11R6/include -DX11 -Wall
LDLIBS	= -L/usr/X11R6/lib -lglut -lGLU -lGL -lm
OBJECTS	= $(subst .cpp,.o,$(wildcard *.cpp))
TARGET	= curve

$(TARGET): $(OBJECTS)

clean:
	-rm -f $(TARGET) *.o *~
