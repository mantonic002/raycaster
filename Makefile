build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lSDL2_ttf -lm -o game

run:
	./game

clean:
	rm game