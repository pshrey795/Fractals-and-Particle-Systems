CC=g++
CFLAGS = -lglfw -ldl -lassimp
WIDTH = 800
HEIGHT = 600
MODE = 0

compile: main.cpp shader.hpp
	$(CC) -o draw main.cpp glad/glad.o $(CFLAGS) 

run: draw
	./draw $(WIDTH) $(HEIGHT) $(MODE)

clean:
	rm -f draw