#include <stdio.h>
#include "nonogram.h"

int main() {
    int n;

    //printf("Enter the dimension of the nonogram: ");
    scanf("%d", &n);

    // Alocar o nonograma
    Nonogram *nonogram = NonogramAllocate(n);

    // Ler as dicas do nonograma
    //printf("Enter the hints for rows and columns:\n");
    NonogramRead(nonogram);

    // Resolver o nonograma
    int solution_count = 0;
    NonogramPlay(nonogram, 0, 0, &solution_count);

    // Mensagem caso nenhuma solução seja encontrada
    if (solution_count > 0){
        printf("Total of solutions: %d\n",solution_count);
    }
    else if(solution_count == 0) {
        printf("No solution was found!\n");
    }

    // Liberar a memória do nonograma
    nonogram = NonogramFree(nonogram);

    return 0;
}
