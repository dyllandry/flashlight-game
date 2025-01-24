build-and-run:
	make build
	make run

build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o flashlight-game

run:
	./flashlight-game

clean:
	rm flashlight-game

