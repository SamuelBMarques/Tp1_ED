#ifndef NONOGRAM_H
#define NONOGRAM_H

typedef struct nonogram Nonogram;

Nonogram* NonogramAllocate(int n);
Nonogram* NonogramFree(Nonogram *nonogram);
void NonogramRead(Nonogram *nonogram);
void NonogramPrint(Nonogram *nonogram, int solution_count); 
void NonogramPlay(Nonogram *nonogram, int linha, int coluna, int *solution_count);

#endif // NONOGRAM_H
