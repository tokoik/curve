CPPFLAGS	= -I/usr/X11R6/include -DX11 -Wall
LDLIBS	= -L/usr/X11R6/lib -lglut -lGLU -lGL -lm
OBJECTS	= $(subst .cpp,.o,$(wildcard *.cpp))
TARGET	= curve

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	-rm -f $(TARGET) *.o *~

depend:
	makedepend -- $(CPPFLAGS) -- $(wildcard *.cpp)

# DO NOT DELETE

catmull_rom.o: catmull_rom.h
curve.o: curve.h
main.o: /usr/X11R6/include/GL/glut.h /usr/X11R6/include/GL/gl.h
main.o: /usr/X11R6/include/GL/glext.h /usr/include/stddef.h
main.o: /usr/include/_types.h /usr/include/sys/_types.h
main.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
main.o: /usr/include/sys/_posix_availability.h /usr/include/machine/_types.h
main.o: /usr/include/i386/_types.h /usr/include/inttypes.h
main.o: /usr/include/stdint.h /usr/X11R6/include/GL/glu.h curve.h
