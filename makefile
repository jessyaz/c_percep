perceptron: ./src/main.c
	gcc ./src/*.c -o ./bin/run.out $(sdl2-config --cflags --libs) -I/usr/local/opt/sdl2/include/SDL2/ -lSDL2 -lm
	#changer le chemin '/usr/local/opt/sdl2/include/SDL2/' en fonction de l'installation de la SDL

	./bin/run.out
	make clean
clean:
	rm ./bin/run.out
	rm ./bin/temp.csv 
