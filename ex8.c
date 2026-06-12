#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *dados;
    int tam;
    int cap;
} Bloco;

typedef struct {
    Bloco *blocos;
    int qtdBlocos;
    int capacidade;
    int blocoLeitura;
    int posLeituraBloco;
} Fita;

typedef struct {
    int valor;
    int id_fita;
} NoIntercalacao;

// FASE 1: Geração de Blocos iniciais via Seleção por Substituição
void heapify1(int heap[], int n, int i) {
    int menorIdx = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && heap[esq] < heap[menorIdx])
        menorIdx = esq;

    if (dir < n && heap[dir] < heap[menorIdx])
        menorIdx = dir;

    if (menorIdx != i) {
        int tmp = heap[i];
        heap[i] = heap[menorIdx];
        heap[menorIdx] = tmp;
        heapify1(heap, n, menorIdx);
    }
}

void buildHeap1(int heap[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify1(heap, n, i);
}

//FASE 2: Intercalação Balanceada K-Way
void heapify2(NoIntercalacao heap[], int n, int i) {
    int menorIdx = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    // Critério de desempate: estabilidade pela fita de origem
    if (esq < n) {
        if (heap[esq].valor < heap[menorIdx].valor ||
           (heap[esq].valor == heap[menorIdx].valor && heap[esq].id_fita < heap[menorIdx].id_fita))
            menorIdx = esq;
    }

    if (dir < n) {
        if (heap[dir].valor < heap[menorIdx].valor ||
           (heap[dir].valor == heap[menorIdx].valor && heap[dir].id_fita < heap[menorIdx].id_fita))
            menorIdx = dir;
    }

    if (menorIdx != i) {
        NoIntercalacao tmp = heap[i];
        heap[i] = heap[menorIdx];
        heap[menorIdx] = tmp;
        heapify2(heap, n, menorIdx);
    }
}

void buildHeap2(NoIntercalacao heap[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify2(heap, n, i);
}

// GERENCIAMENTO DE FITAS: funções para criar blocos, escrever neles, ler da fita, etc.
void initFita(Fita *f) {
    f->qtdBlocos = 0;
    f->capacidade = 10;
    f->blocos = malloc(f->capacidade * sizeof(Bloco));
    f->blocoLeitura = 0;
    f->posLeituraBloco = 0;
}

void novoBloco(Fita *f) {
    if (f->qtdBlocos == f->capacidade) {
        f->capacidade *= 2;
        f->blocos = realloc(f->blocos, f->capacidade * sizeof(Bloco));
    }
    Bloco *b = &f->blocos[f->qtdBlocos++];
    b->cap = 10;
    b->tam = 0;
    b->dados = malloc(b->cap * sizeof(int));
}

void escreveBloco(Fita *f, int valor) {
    Bloco *b = &f->blocos[f->qtdBlocos - 1];
    if (b->tam == b->cap) {
        b->cap *= 2;
        b->dados = realloc(b->dados, b->cap * sizeof(int));
    }
    b->dados[b->tam++] = valor;
}

int leFitaCorrente(Fita *f, int *valor) {
    if (f->blocoLeitura >= f->qtdBlocos) return 0;
    Bloco *b = &f->blocos[f->blocoLeitura];
    if (f->posLeituraBloco < b->tam) {
        *valor = b->dados[f->posLeituraBloco++];
        return 1;
    }
    return 0; // Bloco atual foi todo consumido
}

void imprimeFitasFase1(Fita fitas[], int K) {
    printf("[Fase 1 - Fitas Geradas]:\n");
    for (int i = 0; i < K; i++) {
        printf("Fita %d:", i);
        for (int j = 0; j < fitas[i].qtdBlocos; j++) {
            printf(" [");
            for (int k = 0; k < fitas[i].blocos[j].tam; k++) {
                if (k) printf(" ");
                printf("%d", fitas[i].blocos[j].dados[k]);
            }
            printf("]");
        }
        printf("\n");
    }
}

int main() {
    int N, M, K;
    if (scanf("%d %d %d", &N, &M, &K) != 3) return 0;

    // FASE 1: Geração de Blocos iniciais via Seleção por Substituição
    int *heap = malloc(M * sizeof(int));
    int lidos = 0;

    // Lê a carga inicial sem armazenar tudo num vetor
    for (int i = 0; i < M && lidos < N; i++) {
        scanf("%d", &heap[i]);
        lidos++;
    }

    int ativos = lidos;
    int inativos = 0;
    buildHeap1(heap, ativos);

    printf("[Fase 1 - Heap Inicial]:");
    for (int i = 0; i < ativos; i++) printf(" %d", heap[i]);
    printf("\n");

    Fita *fitas = malloc(2 * K * sizeof(Fita));
    for (int i = 0; i < 2 * K; i++) initFita(&fitas[i]);

    int fitaAtual = 0;
    novoBloco(&fitas[fitaAtual]);
    int mostrouHeapAlterado = 0;

    while (1) {
        if (ativos == 0) {
            if (inativos > 0) {
                if (!mostrouHeapAlterado) {
                    printf("[Fase 1 - Heap Alterado]:");
                    // Elementos inativos mapeados estritamente do fim para o começo do vetor
                    for (int i = 0; i < inativos; i++) printf(" %d", heap[M - inativos + i]);
                    printf("\n");
                    mostrouHeapAlterado = 1;
                }
                
                // Reposiciona os elementos inativos no início do Heap para o próximo bloco
                for (int i = 0; i < inativos; i++) {
                    heap[i] = heap[M - inativos + i];
                }
                
                ativos = inativos;
                inativos = 0;
                buildHeap1(heap, ativos);
                fitaAtual = (fitaAtual + 1) % K;
                novoBloco(&fitas[fitaAtual]);
            } else {
                break; // Acabaram todos os dados
            }
        }

        int min = heap[0];
        escreveBloco(&fitas[fitaAtual], min);

        if (lidos < N) {
            int novo;
            scanf("%d", &novo);
            lidos++;
            if (novo >= min) {
                heap[0] = novo;
                heapify1(heap, ativos, 0);
            } else {
                // Diminui o heap ativo e aloca o inativo rigidamente no final do vetor
                heap[0] = heap[ativos - 1];
                ativos--;
                inativos++;
                heap[M - inativos] = novo;
                heapify1(heap, ativos, 0);
            }
        } else {
            // EOF atingido, apenas reduz o heap ativo
            heap[0] = heap[ativos - 1];
            ativos--;
            heapify1(heap, ativos, 0);
        }
    }

    imprimeFitasFase1(fitas, K);

    // FASE 2: Intercalação Balanceada K-Way
    int in_offset = 0;
    int out_offset = K;
    int primeiraPassada = 1;

    while (1) {
        int totalBlocos = 0;
        for (int i = 0; i < K; i++) {
            totalBlocos += fitas[in_offset + i].qtdBlocos;
            fitas[in_offset + i].blocoLeitura = 0;
            fitas[in_offset + i].posLeituraBloco = 0;
        }

        if (totalBlocos <= 1 && !primeiraPassada) break; 
        if (totalBlocos == 0) break; // Array vazio

        // Limpa memórias das fitas de destino (evita leaks)
        for (int i = 0; i < K; i++) {
            int id = out_offset + i;
            for (int j = 0; j < fitas[id].qtdBlocos; j++) {
                free(fitas[id].blocos[j].dados);
            }
            fitas[id].qtdBlocos = 0;
            fitas[id].blocoLeitura = 0;
            fitas[id].posLeituraBloco = 0;
        }

        int out_tape = out_offset;
        int blocosProcessados = 0;

        while (blocosProcessados < totalBlocos) {
            novoBloco(&fitas[out_tape]);
            NoIntercalacao heap2[K];
            int tamHeap2 = 0;

            // Puxa o primeiro elemento de cada fita ativa
            for (int i = 0; i < K; i++) {
                int id = in_offset + i;
                if (fitas[id].blocoLeitura < fitas[id].qtdBlocos) {
                    int val;
                    if (leFitaCorrente(&fitas[id], &val)) {
                        heap2[tamHeap2].valor = val;
                        heap2[tamHeap2].id_fita = id;
                        tamHeap2++;
                        blocosProcessados++;
                    }
                }
            }
            buildHeap2(heap2, tamHeap2);

            // Imprime o heap de intercalação só uma vez
            if (primeiraPassada && out_tape == out_offset && tamHeap2 > 0) {
                printf("[Fase 2 - Heap Intercalacao]:");
                for (int i = 0; i < tamHeap2; i++) {
                    printf(" %d", heap2[i].valor);
                }
                printf("\n");
            }

            // Merge das fitas para o bloco atual
            while (tamHeap2 > 0) {
                NoIntercalacao min = heap2[0];
                escreveBloco(&fitas[out_tape], min.valor);

                int val;
                if (leFitaCorrente(&fitas[min.id_fita], &val)) {
                    heap2[0].valor = val;
                    heapify2(heap2, tamHeap2, 0);
                } else {
                    heap2[0] = heap2[tamHeap2 - 1];
                    tamHeap2--;
                    heapify2(heap2, tamHeap2, 0);
                }
            }

            // Avança o ponteiro de blocos nas fitas de leitura que participaram
            for (int i = 0; i < K; i++) {
                int id = in_offset + i;
                if (fitas[id].blocoLeitura < fitas[id].qtdBlocos) {
                    fitas[id].blocoLeitura++;
                    fitas[id].posLeituraBloco = 0;
                }
            }

            // Alterna o destino
            out_tape++;
            if (out_tape == out_offset + K) {
                out_tape = out_offset;
            }
        }

        if (primeiraPassada) {
            printf("[Fase 2 - Fitas apos 1a Passada]:\n");
            for (int i = 0; i < K; i++) {
                int id = out_offset + i;
                printf("Fita %d:", id);
                for (int j = 0; j < fitas[id].qtdBlocos; j++) {
                    printf(" [");
                    for (int k = 0; k < fitas[id].blocos[j].tam; k++) {
                        if (k) printf(" ");
                        printf("%d", fitas[id].blocos[j].dados[k]);
                    }
                    printf("]");
                }
                printf("\n");
            }
            primeiraPassada = 0;
        }

        // Troca os conjuntos de fitas leitura/escrita
        int temp = in_offset;
        in_offset = out_offset;
        out_offset = temp;
    }

    // Busca o resultado final (garantindo que lê do conjunto de fitas que finalizou ativo)
    printf("[Resultado Final]:");
    for (int i = 0; i < K; i++) {
        int id = in_offset + i;
        if (fitas[id].qtdBlocos == 1) {
            for (int k = 0; k < fitas[id].blocos[0].tam; k++) {
                printf(" %d", fitas[id].blocos[0].dados[k]);
            }
            break;
        }
    }
    printf("\n");

    free(heap);
    return 0;
}