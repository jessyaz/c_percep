perceptron: ./src/main.c
	gcc ./src/*.c -o ./bin/run.out $(sdl2-config --cflags --libs) -I./src/lib/sdl/ -lSDL2 -lm

	./bin/run.out
	make clean
clean:
	rm ./bin/run.out
	rm ./bin/temp.csv 
