#include <stdio.h>
#include <stdlib.h>

typedef struct No{
    int valor;
    struct No *ant;
    struct No *prox;
}No;

typedef struct{
    No *head;
    No *tail;
}Lista;

int hash(int k, int m){
    return k % m;
}

No * novoNo(int valor){
    No *n = (No*)malloc(sizeof(No));
    n->valor = valor;
    n->ant = NULL;
    n->prox = NULL;
    return n;
}

void insereOrdenado(Lista *l, int valor){
    No *novo = novoNo(valor);
    if(l->head == NULL){
        l->head = l->tail = novo;
        return;
    }

    No *atual = l->head;
    while(atual != NULL && atual->valor < valor){
        atual = atual->prox;
    }
    if(atual == NULL){
        novo->ant = l->tail;
        l->tail->prox = novo;
        l->tail = novo;
        return;

    } else if(atual == l->head){
        novo->prox = l->head;
        l->head->ant = novo;
        l->head = novo;
        return;

    }
    novo->prox = atual;
    novo->ant = atual->ant;

    atual->ant->prox = novo;
    atual->ant = novo;
}

int buscaRemove(Lista *l, int chave, char sentido[], int *achou){
    int NEP = 0;
    *achou = 0;

    if(l->head == NULL) return 0; // lista vazia

    int deltaHead = abs(chave - l->head->valor);
    int deltaTail = abs(l->tail->valor - chave);

    if (deltaHead <= deltaTail){ //busca pelo inicio
        sprintf(sentido, "Inicio");
        No *atual = l->head;
        while(atual != NULL){
            NEP++;
            if(atual->valor > chave)
                break;

            No *prox = atual->prox;
            if(atual->valor == chave){
                *achou = 1;

                if(atual == l->head){ //remove
                    l->head = atual->prox;
                }
                if(atual == l->tail){
                    l->tail = atual->ant;
                }
                if(atual->ant)
                    atual->ant->prox = atual->prox;
                if(atual->prox)                   
                    atual->prox->ant = atual->ant;
                free(atual);
            }
            atual = prox;
        }
    }
    else{
        sprintf(sentido, "Fim");
        No *atual = l->tail;
        while(atual != NULL){
            NEP++;
            if(atual->valor < chave)
                break;

            No *ant = atual->ant;
            if(atual->valor == chave){

                if(atual == l->head){ //remove
                    l->head = atual->prox;
                }
                if(atual == l->tail){
                    l->tail = atual->ant;
                }
                if(atual->ant)
                    atual->ant->prox = atual->prox;
                if(atual->prox)                   
                    atual->prox->ant = atual->ant;
                free(atual);
            }
            atual = ant;
        }
    }
    return NEP;
}

void imprimeLista(Lista *l, int indice){
    printf("%d :: ", indice);

    if(l->head == NULL){
        printf("NULL\n");
        return;
    }

    No *atual = l->head;
    while(atual != NULL){
        printf("%d -> ", atual->valor);
        atual = atual->prox;
    }
    printf("NULL :: ");

    atual = l->tail;
    while(atual != NULL){
        printf("%d -> ", atual->valor);
        atual = atual->ant;
    }
    printf("NULL\n");
}

int main(){
    int m, i, x;
    scanf("%d", &m);

    Lista *tabela = (Lista*)malloc(m * sizeof(Lista));

    for(i = 0; i < m; i++){
        tabela[i].head = NULL;
        tabela[i].tail = NULL;
    }
    while(scanf("%d", &x) && x != -1){
        int indice = hash(x, m);
        insereOrdenado(&tabela[indice], x);
    }

    int chave;
    scanf("%d", &chave);

    int pos = hash(chave, m);
    char sentido[20];
    int achou = 0;
    int NEP = buscaRemove(&tabela[pos], chave, sentido, &achou);

    if(tabela[pos].head == NULL && NEP == 0){
        printf("NEP: 0 (Lista Vazia)\n");
    }
    else{
        printf("NEP: %d (Sentido: %s)\n", NEP, sentido);
    }
    if(!achou){
        printf("Valor nao encontrado\n", chave);
    }
    for (i = 0; i < m; i++){
        imprimeLista(&tabela[i], i);
    }
    return 0;
}