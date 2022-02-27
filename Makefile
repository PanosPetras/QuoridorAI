default:
	gcc -c AI.c -O3
	gcc -c Board.c -Wno-alloc-size-larger-than -O3
	gcc -c main.c -O3
	gcc -c List.c -O3
	gcc -c IOHandler.c -O3
	gcc -o Tiger_B AI.o Board.o IOHandler.o main.o List.o -O3
	rm AI.o Board.o main.o List.o IOHandler.o