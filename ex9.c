#include <stdio.h>
#include <stdlib.h>

#define INF 999999

typedef struct Lista {
    int dest;
    int peso;
    struct Lista* prox;
} Lista;

//inicia e insere ordenado 
Lista* insert_Lista(Lista* head, int dest, int peso) {
    Lista* nova_Lista = (Lista*)malloc(sizeof(Lista));
    nova_Lista->dest = dest;
    nova_Lista->peso = peso;
    nova_Lista->prox = NULL;
    // Inserção no começo se a lista estiver vazia ou se o destino for menor que o head atual
    if (head == NULL || head->dest > dest) {
        nova_Lista->prox = head;
        return nova_Lista;
    }
    // Busca linear para encontrar a posição correta mantendo a ordenação
    Lista* current = head;
    while (current->prox != NULL && current->prox->dest < dest) {
        current = current->prox;
    }
    nova_Lista->prox = current->prox;
    current->prox = nova_Lista;
    
    return head;
}

// Algoritmo de Bellman-Ford
void bellman_ford(Lista** graph, int N, int start, int R, int dest_node) {
    int dist[105];
    int pred[105];
    
    for (int i = 0; i < N; i++) {
        dist[i] = INF;
        pred[i] = -1;
    }
    dist[start] = 0;

    int upd = 0;
    for (int step = 0; step < N - 1; step++) {
        for (int u = 0; u < N; u++) {
            if (dist[u] == INF) continue; // Evita propagar do "infinito"
            
            for (Lista* e = graph[u]; e != NULL; e = e->prox) {
                int v = e->dest;
                int p = e->peso;
                
                // Verifica a condição de restrição para bloquear a aresta
                if (p > R && dist[u] <= R / 2.0) {
                    continue; 
                }
                
                // Relaxamento
                if (dist[u] + p < dist[v]) {
                    dist[v] = dist[u] + p;
                    pred[v] = u;
                    upd++;
                }
            }
        }
    }

    // Linha 1: Custos Bellman-Ford
    for (int i = 0; i < N; i++) {
        if (dist[i] == INF) printf("INF ");
        else printf("%d ", dist[i]);
    }
    printf("| Upd: %d\n", upd);

    // Linha 2: Rota Bellman-Ford
    if (dist[dest_node] == INF) {
        printf("Destino nao alcancado\n");
    } else {
        int path[105];
        int count = 0;
        int curr = dest_node;
        
        while (curr != -1) {
            path[count++] = curr;
            curr = pred[curr];
        }
        
        for (int i = count - 1; i >= 0; i--) {
            printf("%d%s", path[i], (i == 0) ? "" : " ");
        }
        printf("\n");
    }
}

// Algoritmo de Dijkstra modificado (Gula com busca linear)
void dijkstra(Lista** graph, int N, int start, int R, int dest_node) {
    int dist[105];
    int pred[105];
    int visited[105];
    int seq[105];
    int seq_count = 0;

    for (int i = 0; i < N; i++) {
        dist[i] = INF;
        pred[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int step = 0; step < N; step++) {
        int min_dist = INF;
        int u = -1;
        
        // Busca linear pelo menor custo; critério de desempate por `<=` para simular a fila exigida
        for (int i = 0; i < N; i++) {
            if (!visited[i] && dist[i] != INF && dist[i] <= min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }

        // Todos os restantes estão inalcançáveis
        if (u == -1) break;

        visited[u] = 1;
        seq[seq_count++] = u;

        for (Lista* e = graph[u]; e != NULL; e = e->prox) {
            int v = e->dest;
            int p = e->peso;
            
            // Verifica a restrição antes de tentar transitar na aresta
            if (p > R && dist[u] <= R / 2.0) {
                continue;
            }
            
            // Como é guloso, não reavalia nós já visitados e fechados
            if (!visited[v] && dist[u] + p < dist[v]) {
                dist[v] = dist[u] + p;
                pred[v] = u;
            }
        }
    }

    // Linha 4: Custos Dijkstra
    for (int i = 0; i < N; i++) {
        if (dist[i] == INF) printf("INF ");
        else printf("%d ", dist[i]);
    }
    
    // Imprime sequência de extração
    printf("| Seq:");
    for (int i = 0; i < seq_count; i++) {
        printf(" %d", seq[i]);
    }
    printf("\n");

    // Linha 5: Rota Dijkstra
    if (dist[dest_node] == INF) {
        printf("Destino nao alcancado\n");
    } else {
        int path[105];
        int count = 0;
        int curr = dest_node;
        
        while (curr != -1) {
            path[count++] = curr;
            curr = pred[curr];
        }
        
        for (int i = count - 1; i >= 0; i--) {
            printf("%d%s", path[i], (i == 0) ? "" : " ");
        }
        printf("\n");
    }
}

int main() {
    int V, R, N, M;
    
    // Processa múltiplos casos de teste, se houver
    while (scanf("%d", &V) != EOF) {
        scanf("%d", &R);
        scanf("%d %d", &N, &M);

        Lista* graph[105] = {NULL};
        int has_negative = 0;

        for (int i = 0; i < M; i++) {
            int u, v, p;
            scanf("%d %d %d", &u, &v, &p);
            
            // Marca a existência de aresta negativa para acionar o alerta
            if (p < 0) {
                has_negative = 1;
            }
            graph[u] = insert_Lista(graph[u], v, p);
        }

        int dest_node;
        scanf("%d", &dest_node);

        bellman_ford(graph, N, V, R, dest_node);

        // Linha 3: Aviso de Segurança
        if (has_negative) {
            printf("SOLUCAO NAO CONFIAVEL PARA DIJKSTRA\n");
        }

        dijkstra(graph, N, V, R, dest_node);

        // Limpeza de memória
        for (int i = 0; i < N; i++) {
            Lista* current = graph[i];
            while (current != NULL) {
                Lista* temp = current;
                current = current->prox;
                free(temp);
            }
        }
    }
    
    return 0;
}