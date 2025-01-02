#include <stdio.h>
#include <stdlib.h>
#include "nonogram.h"
/*
Nome: nonogram
Descricao: Struct que contem os dados do nonograma, a dimensao do tabuleiro, o proprio tabuleiro, o numero das dicas em cada coluna e linha
e tambem quais linhas sao essas.
 */

struct nonogram {
    int dimension;
    int **board;
    int **hints_rows;
    int **hints_columns;
    int *num_hints_rows;
    int *num_hints_columns;
};

/*
Nome: NonogramAlocate
Descricao: Funcao para alocar a memoria referente ao tabuleiro do nonograma (matriz), dicas (matriz) e numero de dicas (vetor).
Entrada: Tamanho do nonograma.
Saida: Endereco da struct nonogram, com a memoria alocada.
*/

Nonogram* NonogramAllocate(int n) {
    Nonogram *nonogram = (Nonogram*) malloc(sizeof(Nonogram));

    nonogram->dimension = n;

    // Aloca tabuleiro
    nonogram->board = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        nonogram->board[i] = (int*) malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            nonogram->board[i][j] = -1; // Inicializa com -1
        }
    }

    // Aloca dicas para linhas e colunas
    nonogram->hints_rows = (int**) malloc(n * sizeof(int*));
    nonogram->hints_columns = (int**) malloc(n * sizeof(int*));

    // Aloca os vetores de quantidades de dicas
    nonogram->num_hints_rows = (int*) malloc(n * sizeof(int));
    nonogram->num_hints_columns = (int*) malloc(n * sizeof(int));

    return nonogram;
}

/*
Nome: NonogramFree
Descricao: Funcao para liberar a memoria referente ao tabuleiro do nonograma (matriz), dicas (matriz) e numero de dicas (vetor).
Entrada: Struct do nonograma.
Saida: Endereco nulo.
*/

Nonogram* NonogramFree(Nonogram *nonogram) {
    if (nonogram == NULL) return NULL;

    for (int i = 0; i < nonogram->dimension; i++) {
        free(nonogram->board[i]);
        free(nonogram->hints_rows[i]);
        free(nonogram->hints_columns[i]);
    }

    free(nonogram->board);
    free(nonogram->hints_rows);
    free(nonogram->hints_columns);
    free(nonogram->num_hints_rows);
    free(nonogram->num_hints_columns);

    free(nonogram);     
    return NULL;
}

/*
Nome: NonogramRead
Descricao: Função para ler os dados de entrada referentes ao tamanho e às dicas do nonograma.
Entrada: Struct do nonograma.
Saida: --
*/

void NonogramRead(Nonogram *nonogram) {
    int n = nonogram->dimension;

    for (int i = 0; i < n; i++) {
        scanf("%d", &nonogram->num_hints_columns[i]);                                               //Escaneia o numero de dicas
        nonogram->hints_columns[i] = (int*) malloc(nonogram->num_hints_columns[i] * sizeof(int));   //Aloca o vetor para as dicas
        for (int j = 0; j < nonogram->num_hints_columns[i]; j++) {
            scanf("%d", &nonogram->hints_columns[i][j]);                                            //Preenche o vetor
        }
    }

    for (int i = 0; i < n; i++) {
        scanf("%d", &nonogram->num_hints_rows[i]);
        nonogram->hints_rows[i] = (int*) malloc(nonogram->num_hints_rows[i] * sizeof(int));
        for (int j = 0; j < nonogram->num_hints_rows[i]; j++) {
            scanf("%d", &nonogram->hints_rows[i][j]);
        }
    }
}

/*
Nome: NonogramPrint
Descricao: Função para imprimir o nonograma resolvido, substituindo a matriz de 0s e 1s por "." e "x".
Entrada: Struct do nonograma e a contagem de soluções.
Saida: --
*/

void NonogramPrint(Nonogram *nonogram, int solution_count) {
    printf("SOLUTION %d:\n", solution_count);
    for (int i = 0; i < nonogram->dimension; i++) {
        for (int j = 0; j < nonogram->dimension; j++) {
            if (nonogram->board[i][j] == 1) {
                printf("x");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

/*
Nome: VerifyRC
Descricao: Função para verificar se uma linha ou coluna está correta de acordo com as dicas.
Entrada: Vetor a se verificar (linha ou coluna), vetor de dicas da linha ou coluna correspondente, o número de dicas, o tamanho
do nonograma e se a linha esta parcialmente completa ou não.
Saida: 1 ou 0 , correspondendo a se a linha esta correta ou não.
*/

int VerifyRC(int *vet, int *hints, int num_hints, int tamanho, int partial) {
    int conjunto = 0, i = 0, j = 0;

    while (i < tamanho) {
        if (vet[i] == 1) {
            conjunto++;
        } else if (vet[i] == 0 || (vet[i] == -1 && partial)) {                  //Se tiver encontrado um 0 ou tiver encontrado um -1 e o vetor estiver incompleto
            if (conjunto > 0) {
                if (j >= num_hints || conjunto > hints[j]) return 0;            //Se tiver mais conjuntos contados do que dicas ou mais 1s no conjunto do que na dica
                if (vet[i] == 0 && conjunto != hints[j]) return 0;              //Se tiver encontrado um zero e o conjunto de 1s é maior que o da dica
                conjunto = 0;                                                   
                if (vet[i] == 0) j++;                                           //Pula para o proximo conjunto
            }
        } else if (vet[i] == -1) {
            break;                                                              //Para a validação parcial
        }
        i++;
    }

    if (conjunto > 0) {                                                         //Validação para o caso em que a linha/coluna termina em 1 e o não for encontrado um 0 para fazer a validação
        if (j >= num_hints || conjunto > hints[j]) return 0;                    //Se tiver mais conjuntos contados do que dicas ou mais 1s no conjunto do que na dica                
        if (!partial && conjunto != hints[j]) return 0;                         //Se o número de 1s no conjunto for diferente que o da dica e o vetor já está completo
        j++;
    }

    return partial || j == num_hints;                                           //Retorna 1 para validação concluída
}

/*
Nome: NonogramPlay
Descricao: Função resolver o tabuleiro de maneira recursiva, chamando a função de imprimir sempre que um resultado correto for encontrado.
Entrada: Struct do nonograma, a linha e a coluna que estão sendo alteradas e o endereco da variável de contagem de solucoes.
Saida: --
*/

void NonogramPlay(Nonogram *nonogram, int linha, int coluna, int *solution_count) {
    int n = nonogram->dimension;

    if (linha == n) {                                               //Se chegou na última linha
        (*solution_count)++;                                        //Aumenta o número de soluções encontradas
        NonogramPrint(nonogram, *solution_count);                   //Imprime o nonograma
        return;
    }

    int proxLinha = (coluna == n - 1) ? linha + 1 : linha;          //Se estiver na última coluna, pula para a prúxima linha e mantém na mesma caso contrário
    int proxColuna = (coluna == n - 1) ? 0 : coluna + 1;            //Se estiver na última coluna, volta para a primeira e pula para a próxima caso contrário

    for (int valor = 0; valor <= 1; valor++) {                      //De 0 a 1
        nonogram->board[linha][coluna] = valor;                     //Preenche o nonograma com o valor

        int valido = 1;
        int colunaAtual[n];                                         //Cria um vetor para armazenar a coluna que esta sendo alterada

        for (int i = 0; i < n; i++) {
            colunaAtual[i] = nonogram->board[i][coluna];            //Preenche o vetor da coluna
        }

        valido &= VerifyRC(nonogram->board[linha], nonogram->hints_rows[linha], nonogram->num_hints_rows[linha], n, 1);         //Verifica se a linha está correta de acordo com as dicas
        valido &= VerifyRC(colunaAtual, nonogram->hints_columns[coluna], nonogram->num_hints_columns[coluna], n, 1);            //Verifica se a coluna está correta de acordo com as dicas

        if (coluna == n - 1) {                                      //Se estiver na última coluna
            valido &= VerifyRC(nonogram->board[linha], nonogram->hints_rows[linha], nonogram->num_hints_rows[linha], n, 0);
        }
        if (linha == n - 1) {                                       //Se estiver na última linha
            valido &= VerifyRC(colunaAtual, nonogram->hints_columns[coluna], nonogram->num_hints_columns[coluna], n, 0);
        }
        if (valido) {                                               //Se todos os casos foram validados
            NonogramPlay(nonogram, proxLinha, proxColuna, solution_count);      //Chama a função recursivamente
        }
    }
    
    nonogram->board[linha][coluna] = -1; 
}

