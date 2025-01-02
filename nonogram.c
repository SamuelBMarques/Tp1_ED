#include <stdio.h>
#include <stdlib.h>
#include "nonogram.h"

struct nonogram {
    int dimension;
    int **board;
    int **hints_rows;
    int **hints_columns;
    int *num_hints_rows;
    int *num_hints_columns;
};

// Função para alocar o nonograma
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

// Função para liberar o nonograma
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

// Função para ler o nonograma
void NonogramRead(Nonogram *nonogram) {
    int n = nonogram->dimension;

    for (int i = 0; i < n; i++) {
        scanf("%d", &nonogram->num_hints_rows[i]);
        nonogram->hints_rows[i] = (int*) malloc(nonogram->num_hints_rows[i] * sizeof(int));
        for (int j = 0; j < nonogram->num_hints_rows[i]; j++) {
            scanf("%d", &nonogram->hints_rows[i][j]);
        }
    }

    for (int i = 0; i < n; i++) {
        scanf("%d", &nonogram->num_hints_columns[i]);
        nonogram->hints_columns[i] = (int*) malloc(nonogram->num_hints_columns[i] * sizeof(int));
        for (int j = 0; j < nonogram->num_hints_columns[i]; j++) {
            scanf("%d", &nonogram->hints_columns[i][j]);
        }
    }
}

// Função para imprimir o nonograma resolvido
void NonogramPrint(Nonogram *nonogram, int solution_count) {
    printf("SOLUTION %d:\n", solution_count);
    for (int i = 0; i < nonogram->dimension; i++) {
        for (int j = 0; j < nonogram->dimension; j++) {
            if (nonogram->board[j][i] == 1) {
                printf("x");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

// Função auxiliar para verificar uma linha ou coluna
int VerifyRC(int *vet, int *hints, int num_hints, int tamanho) {
    int conjunto = 0, i = 0, j = 0;

    while (i < tamanho) {
        if (vet[i] == -1) break;

        if (vet[i] == 1) {
            conjunto++;
        } else if (vet[i] == 0 && conjunto > 0) {
            if (j >= num_hints || conjunto != hints[j]) return 0;
            conjunto = 0;
            j++;
        }
        i++;
    }

    if (conjunto > 0) {
        if (j >= num_hints || conjunto != hints[j]) return 0;
        j++;
    }

    return j == num_hints;
}

// Função para resolver o nonograma
void NonogramPlay(Nonogram *nonogram, int linha, int coluna, int *solution_count) {
    int n = nonogram->dimension;

    if (linha == n) {
        (*solution_count)++;
        NonogramPrint(nonogram, *solution_count);
        return;
    }

    int proxLinha = (coluna == n - 1) ? linha + 1 : linha;
    int proxColuna = (coluna == n - 1) ? 0 : coluna + 1;

    for (int valor = 0; valor <= 1; valor++) {
        nonogram->board[linha][coluna] = valor;

        int valido = 1;
        if (coluna == n - 1) {
            valido = VerifyRC(nonogram->board[linha], nonogram->hints_rows[linha], nonogram->num_hints_rows[linha], n);
        }

        if (valido && linha == n - 1) {
            int colunaAtual[n];
            for (int i = 0; i < n; i++) {
                colunaAtual[i] = nonogram->board[i][coluna];
            }
            valido = VerifyRC(colunaAtual, nonogram->hints_columns[coluna], nonogram->num_hints_columns[coluna], n);
        }

        if (valido) {
            NonogramPlay(nonogram, proxLinha, proxColuna, solution_count);
        }
    }

    nonogram->board[linha][coluna] = -1; // Backtracking
}

