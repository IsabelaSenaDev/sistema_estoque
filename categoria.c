#include <stdio.h>
#include "categoria.h"
#include "util.h"

int buscarCategoriaPorCodigo(Categoria lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

int cadastrarCategoria(Categoria lista[], int total) {
    Categoria c;

    if (total >= MAX_CATEGORIAS) {
        printf("Lista cheia! Limite de %d categorias atingido.\n\n", MAX_CATEGORIAS);
        return total;
    }

    /* Codigo: positivo e que ainda nao exista */
    do {
        c.codigo = lerInteiro("Codigo da categoria: ");
        if (c.codigo <= 0) {
            printf("Codigo invalido! Deve ser um numero positivo.\n");
        } else if (buscarCategoriaPorCodigo(lista, total, c.codigo) != -1) {
            printf("Codigo ja cadastrado! Digite outro codigo.\n");
            c.codigo = 0;
        }
    } while (c.codigo <= 0);

    /* Nome: nao pode ficar em branco */
    do {
        printf("Nome da categoria: ");
        lerLinha(c.nome, sizeof(c.nome));
        if (textoEmBranco(c.nome)) {
            printf("Nome invalido! O nome nao pode ficar em branco.\n");
        } else if (temPontoEVirgula(c.nome)) {
            printf("Nome invalido! Nao use o caractere ';'.\n");
            c.nome[0] = '\0';
        }
    } while (textoEmBranco(c.nome));

    lista[total] = c;
    printf("\nCategoria cadastrada com sucesso!\n\n");
    return total + 1;
}

void listarCategorias(Categoria lista[], int total) {
    int i;

    if (total == 0) {
        printf("Nenhuma categoria foi cadastrada.\n\n");
        return;
    }

    printf("\n===== Lista de Categorias (%d) =====\n", total);
    for (i = 0; i < total; i++) {
        printf("Codigo: %d | Nome: %s\n", lista[i].codigo, lista[i].nome);
    }
    printf("====================================\n\n");
}
