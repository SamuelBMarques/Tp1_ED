all: main.o nonogram.o
	@gcc main.o nonogram.o -o exe -lm
	@rm -r main.o nonogram.o
main.o: main.c
	@gcc main.c -c -Wall
nonogram.o: nonogram.c
	@gcc nonogram.c -c -Wall
run:
	@./exe
val:
	@gcc -g -o exeval nonogram.c main.c -Wall
valrun: val
	@valgrind --leak-check=full --track-origins=yes -s ./exeval

