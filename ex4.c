#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char palavra[21];
    int peso;
} Palavra;

// comparação para max-heap
int maior(Palavra a, Palavra b) {
    if (a.peso > b.peso) return 1;
    if (a.peso < b.peso) return 0;

    // desempate: ordem alfabética inversa (Z → A)
    if (strcmp(a.palavra, b.palavra) > 0) return 1;

    return 0;
}

void swap(Palavra *a, Palavra *b) {
    Palavra temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(Palavra *v, int n, int i) {
    int maior_idx = i;
    int esq = 2*i + 1;
    int dir = 2*i + 2;

    if (esq < n && maior(v[esq], v[maior_idx]))
        maior_idx = esq;

    if (dir < n && maior(v[dir], v[maior_idx]))
        maior_idx = dir;

    if (maior_idx != i) {
        swap(&v[i], &v[maior_idx]);
        heapify(v, n, maior_idx);
    }
}

void buildHeap(Palavra *v, int n) {
    for (int i = n/2 - 1; i >= 0; i--) {
        heapify(v, n, i);
    }
}

// heapsort sem reconstruir heap
void heapSort(Palavra *v, int n) {
    for (int i = n-1; i > 0; i--) {
        swap(&v[0], &v[i]);
        heapify(v, i, 0);
    }
}

int calculaPeso(char *palavra, int freq[]) {
    int soma = 0;
    for (int i = 0; palavra[i] != '\0'; i++) {
        soma += freq[palavra[i] - 'a'];
    }
    return soma;
}

int valida(char *palavra, int freq[], int K) {
    for (int i = 0; palavra[i] != '\0'; i++) {
        if (freq[palavra[i] - 'a'] < K)
            return 0;
    }
    return 1;
}

int main() {
    int N, K;
    scanf("%d %d", &N, &K);

    char **entrada = malloc(N * sizeof(char*));
    for (int i = 0; i < N; i++) {
        entrada[i] = malloc(21 * sizeof(char));
        scanf("%s", entrada[i]);
    }

    // FASE 1: contagem global
    int freq[26] = {0};
    for (int i = 0; i < N; i++) {
        for (int j = 0; entrada[i][j] != '\0'; j++) {
            freq[entrada[i][j] - 'a']++;
        }
    }

    // FASE 2: filtragem
    Palavra *validas = malloc(N * sizeof(Palavra));
    int M = 0;

    for (int i = 0; i < N; i++) {
        if (!valida(entrada[i], freq, K)) {
            printf("A palavra %s eh invalida (K=%d)\n", entrada[i], K);
        } else {
            strcpy(validas[M].palavra, entrada[i]);
            validas[M].peso = calculaPeso(entrada[i], freq);
            M++;
        }
    }

    // liberar entrada
    for (int i = 0; i < N; i++) {
        free(entrada[i]);
    }
    free(entrada);

    if (M == 0) {
        printf("Erro: Sem palavras validas para ordenacao.\n");
        free(validas);
        return 0;
    }

    // FASE 3: construir heap
    buildHeap(validas, M);

    printf("build_heap:");
    for (int i = 0; i < M; i++) {
        printf(" %s", validas[i].palavra);
    }
    printf("\n");

    // ordenar
    heapSort(validas, M);

    printf("palavras:");
    for (int i = 0; i < M; i++) {
        printf(" %s", validas[i].palavra);
    }
    printf("\n");

    free(validas);
    return 0;
}