build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lSDL2_ttf -lm -o game

run:
	./game

clean:
	rm game

build-wa:
	emcc ./src/*.c -s WASM=1 -s USE_SDL=2 -o game.js

run-wa:
	emrun game.html