#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    int peso;
    struct No *esq, *dir;
} No;

int peso(No *n) {
    return (n == NULL) ? 0 : n->peso;
}

void atualizaPeso(No *n) {
    if (n != NULL)
        n->peso = 1 + peso(n->esq) + peso(n->dir);
}

No* rotacaoDireita(No *y) {
    No *x = y->esq;
    No *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    atualizaPeso(y);
    atualizaPeso(x);

    return x;
}

No* rotacaoEsquerda(No *x) {
    No *y = x->dir;
    No *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    atualizaPeso(x);
    atualizaPeso(y);

    return y;
}

No* balancear(No *n) {
    if (n == NULL) return n;

    int wesq = peso(n->esq);
    int wdir = peso(n->dir);

    // esquerda muito pesada
    if ((wesq + 1) > 2 * (wdir + 1)) {
        No *B = n->esq;

        if (peso(B->esq) >= peso(B->dir)) {
            // LL
            return rotacaoDireita(n);
        } else {
            // LR
            n->esq = rotacaoEsquerda(n->esq);
            return rotacaoDireita(n);
        }
    }

    // direita muito pesada
    if ((wdir + 1) > 2 * (wesq + 1)) {
        No *B = n->dir;

        if (peso(B->dir) >= peso(B->esq)) {
            // RR
            return rotacaoEsquerda(n);
        } else {
            // RL
            n->dir = rotacaoDireita(n->dir);
            return rotacaoEsquerda(n);
        }
    }

    return n;
}

No* novoNo(int valor) {
    No *n = (No*) malloc(sizeof(No));
    n->valor = valor;
    n->peso = 1;
    n->esq = n->dir = NULL;
    return n;
}

No* inserir(No *raiz, int valor) {
    if (raiz == NULL)
        return novoNo(valor);

    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = inserir(raiz->dir, valor);
    else
        return raiz; // ignora duplicado

    atualizaPeso(raiz);
    return balancear(raiz);
}

No* maiorEsquerda(No *n) {
    while (n->dir != NULL)
        n = n->dir;
    return n;
}

No* remover(No *raiz, int valor) {
    if (raiz == NULL) return NULL;

    if (valor < raiz->valor)
        raiz->esq = remover(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = remover(raiz->dir, valor);
    else {
        // encontrou
        if (raiz->esq == NULL) {
            No *temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            No *temp = raiz->esq;
            free(raiz);
            return temp;
        }

        // dois filhos → usa predecessor
        No *pred = maiorEsquerda(raiz->esq);
        raiz->valor = pred->valor;
        raiz->esq = remover(raiz->esq, pred->valor);
    }

    atualizaPeso(raiz);
    return balancear(raiz);
}

void buscaFaixa(No *raiz, int X, int Y, int *primeiro) {
    if (raiz == NULL) return;

    if (raiz->valor > X)
        buscaFaixa(raiz->esq, X, Y, primeiro);

    if (raiz->valor >= X && raiz->valor <= Y) {
        if (!(*primeiro)) printf(", ");
        printf("%d", raiz->valor);
        *primeiro = 0;
    }

    if (raiz->valor < Y)
        buscaFaixa(raiz->dir, X, Y, primeiro);
}

int main() {
    No *raiz = NULL;

    int x;

    // inserção
    while (scanf("%d", &x) && x != -1)
        raiz = inserir(raiz, x);

    if (raiz)
        printf("%d,%d,%d\n", raiz->peso, peso(raiz->esq), peso(raiz->dir));
    else
        printf("ARVORE VAZIA\n");

    // remoção
    while (scanf("%d", &x) && x != -1)
        raiz = remover(raiz, x);

    if (raiz)
        printf("%d,%d,%d\n", raiz->peso, peso(raiz->esq), peso(raiz->dir));
    else {
        printf("ARVORE VAZIA\n");
    }

    int X, Y;
    scanf("%d %d", &X, &Y);

    if (raiz == NULL) {
        printf("NADA A EXIBIR\n");
        return 0;
    }

    int primeiro = 1;
    buscaFaixa(raiz, X, Y, &primeiro);

    if (primeiro)
        printf("NADA A EXIBIR\n");

    return 0;
}