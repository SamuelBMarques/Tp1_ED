#include <stdio.h>
#include "nonogram.h"

int main() {
    int n;

    //Lê a dimensao do tabuleiro.
    scanf("%d", &n);

    //Aloca o nonograma.
    Nonogram *nonogram = NonogramAllocate(n);

    //Lê as dicas do nonograma.
    NonogramRead(nonogram);

    //Resolve o nonograma.
    int solution_count = 0;
    NonogramPlay(nonogram, 0, 0, &solution_count);

    //Mensagem caso nenhuma solução seja encontrada ou indicando quantas soluções foram encontradas.
    if (solution_count > 0){
        printf("Total of solutions: %d\n",solution_count);
    }
    else if(solution_count == 0) {
        printf("No solution was found!\n");
    }

    // Libera a memória do nonograma.
    nonogram = NonogramFree(nonogram);

    return 0;
}
