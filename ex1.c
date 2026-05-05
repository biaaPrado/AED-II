#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct No {
    int id;
    struct No* ant;
    struct No* prox;
} No;

typedef struct {
    No* cabeca;
    No* fim;
} Lista; 

Lista* criaLista(){
    Lista* lista = (Lista*)malloc(sizeof(Lista));
    lista->cabeca = NULL;
    lista->fim = NULL;
    return lista;
}

No* criaNo(int id){
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->id = id;
    novoNo->ant = NULL;
    novoNo->prox = NULL;
    return novoNo;
}

int insereOrdenado(Lista* lista, int id) {
    No* novoNo = criaNo(id);
    int NEP = 0; // Número de Elementos Percorridos
    
    //quando a lista estiver vazia 
    if (lista->cabeca == NULL) {
        lista->cabeca = lista->fim = novoNo;
        return NEP;
    } 
    // quando for inserir no início da lista
    else if (id <= lista->cabeca->id) { 
        novoNo->prox = lista->cabeca;
        lista->cabeca->ant = novoNo;
        lista->cabeca = novoNo;
        return NEP;
    } 
    // quando for inserir no fim
    else if(id >= lista->fim->id) {
        novoNo->ant = lista->fim;
        lista->fim->prox = novoNo;
        lista->fim = novoNo;
        return NEP;
    }

    //quando for inserir no meio da lista
   int D_cabeca = abs(id - lista->cabeca->id);
   int D_fim = abs(id - lista->fim->id);

   if(D_cabeca <= D_fim) { //busca pela cabeça
        No* atual = lista->cabeca;
        while (atual != NULL && atual->id < id) {
            atual = atual->prox;
            NEP++;
        }
        // insere antes do atual
        if (atual != NULL) {
            novoNo->prox = atual;
            novoNo->ant = atual->ant;
            atual->ant->prox = novoNo;
            atual->ant = novoNo;
        }
    } 
    else { //busca pelo fim
        No* atual = lista->fim; 
        while (atual != NULL && atual->id > id) {
            atual = atual->ant;
            NEP++;
        }
        // insere depois do atual
        if (atual != NULL) {
            novoNo->ant = atual;
            novoNo->prox = atual->prox;
            atual->prox->ant = novoNo;
            atual->prox = novoNo;
        }
    }
    return NEP;
}

void imprimeLista(Lista* lista){
    No* atual = lista->cabeca;
    while (atual != NULL) {
        printf("%d", atual->id);
        if (atual->prox != NULL) printf(" ");
        atual = atual->prox;
    }
    printf("\n");
}

int main(){
    int n, i;
    scanf("%d", &n);

    Lista* lista = criaLista();
    int NEP_total = 0;

    for (i=0; i<n; i++) {
        int id;
        scanf("%d", &id);
        
        NEP_total += insereOrdenado(lista, id);
    }
    imprimeLista(lista);
    printf("%d\n", NEP_total);

    return 0;    
}