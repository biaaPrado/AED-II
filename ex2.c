#include <stdio.h>
#include <stdlib.h>

long long merge(long long vetor[], int esq, int meio, int dir) {
    int j = meio + 1; // Ponteiro para a segunda metade
    long long cont = 0;

    // Conta quedas drásticas
    for (int i = esq; i <= meio; i++) {
        while (j <= dir && vetor[i] > 2 * vetor[j]) { //para cada elemento da primeira metade, conta quantos elementos da segunda metade são menores que a metade do elemento atual
            j++;
        }
        cont += (j - (meio + 1));
    }

    // Merge normal
    int n = dir - esq + 1;
    long long aux[n];

    int i = esq;
    j = meio + 1; //comeca a segunda metade do vetor
    int k = 0;

    while (i <= meio && j <= dir) { 
        if (vetor[i] <= vetor[j]) { //primeiro compara os elementos da primeira metade com os da segunda metade 
            aux[k++] = vetor[i++]; //coloca o menor no vetor auxiliar
        } else {
            aux[k++] = vetor[j++];
        }
    }

    while (i <= meio) aux[k++] = vetor[i++]; //copia o resto da primeira metade para o vetor auxiliar
    while (j <= dir) aux[k++] = vetor[j++];  //copia o resto da segunda metade para o vetor auxiliar

    // copia o vetor auxiliar de volta para o vetor original
    for (int i = 0; i < n; i++) {
        vetor[esq + i] = aux[i];
    }

    return cont;
}

long long mergeSort(long long vetor[], int esq, int dir) {
    long long cont = 0;

    if (esq < dir) { //se tiver mais de um elemento
        int meio = (esq + dir) / 2; //divide o vetor em duas partes

        cont += mergeSort(vetor, esq, meio); //ordena a primeira parte esq
        cont += mergeSort(vetor, meio + 1, dir); //ordena a segunda parte dir

        cont += merge(vetor, esq, meio, dir); //faz o merge das duas partes e conta as quedas drásticas
    }

    return cont;
}

int main() {
    int N, i;
    scanf("%d", &N);

    long long vetor[N];

    for (i = 0; i < N; i++) {
        scanf("%lld", &vetor[i]);
    }

    long long resultado = mergeSort(vetor, 0, N - 1); //ordena o vetor

    
    printf("%lld\n", resultado);

    for (i = 0; i < N; i++) { //imprime o vetor ordenado
        printf("%lld", vetor[i]);
        if (i < N - 1) printf(" ");
    }
    printf("\n");

    return 0;
}