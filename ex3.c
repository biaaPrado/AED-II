#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int gravidade;
    int id;
} Paciente;

long long swaps = 0;

// Função de prioridade
int prioridade(Paciente a, Paciente b) {
    if (a.id > b.id) return 1;
    if (a.id < b.id) return 0;
    return a.gravidade < b.gravidade;
}

// Swap com controle
void trocar(Paciente A[], int i, int j) {
    if (i == j) return;

    if (A[i].gravidade == A[j].gravidade && A[i].id == A[j].id)
        return;

    Paciente temp = A[i];
    A[i] = A[j];
    A[j] = temp;

    swaps++;
}

// Mediana de 3
void medianaDeTres(Paciente A[], int p, int r) {
    int mid = p + (r - p) / 2;

    if (prioridade(A[mid], A[p]))
        trocar(A, mid, p);

    if (prioridade(A[r], A[p]))
        trocar(A, r, p);

    if (prioridade(A[r], A[mid]))
        trocar(A, r, mid);

    // coloca pivô no final
    trocar(A, mid, r);
}

// Partição
int partition(Paciente A[], int p, int r) {
    medianaDeTres(A, p, r);

    Paciente pivo = A[r];
    int i = p - 1;

    for (int j = p; j < r; j++) {
        if (prioridade(A[j], pivo)) {
            i++;
            trocar(A, i, j);
        }
    }

    trocar(A, i + 1, r);
    return i + 1;
}

// QuickSort
void quickSort(Paciente A[], int p, int r) {
    if (p < r) {
        int q = partition(A, p, r);
        quickSort(A, p, q - 1);
        quickSort(A, q + 1, r);
    }
}

int main() {
    int N;
    scanf("%d", &N);

    Paciente *A = (Paciente*) malloc(N * sizeof(Paciente));

    for (int i = 0; i < N; i++) {
        scanf("%d %d", &A[i].gravidade, &A[i].id);
    }

    quickSort(A, 0, N - 1);

    for (int i = 0; i < N; i++) {
        printf("%d\n", A[i].gravidade);
    }
    printf("Swaps: %lld\n", swaps);

    free(A);
    return 0;
}