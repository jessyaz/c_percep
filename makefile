perceptron: ./src/main.c
	gcc ./src/*.c -o ./bin/run.out $(sdl2-config --cflags --libs) -I /usr/local/opt/sdl2/include/SDL2/ -L/usr/local/lib -lSDL2 -ltensorflow -lm

	./bin/run.out
	make clean
clean:
	rm ./bin/run.out
	rm ./bin/temp.csv 
