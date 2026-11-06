#include <stdio.h>
#include "arquivo.h"

#define ARQ_CATEGORIAS "categorias.txt"
#define ARQ_PRODUTOS "produtos.txt"

/* categorias.txt: uma por linha -> codigo;nome */
int carregarCategorias(Categoria lista[]) {
    FILE *arq = fopen(ARQ_CATEGORIAS, "r");
    char linha[100];
    int total = 0;

    if (arq == NULL) {
        return 0;
    }

    while (total < MAX_CATEGORIAS && fgets(linha, sizeof(linha), arq) != NULL) {
        Categoria c;
        if (sscanf(linha, "%d;%29[^\n]", &c.codigo, c.nome) == 2) {
            lista[total] = c;
            total++;
        }
    }

    fclose(arq);
    return total;
}

void salvarCategorias(Categoria lista[], int total) {
    FILE *arq = fopen(ARQ_CATEGORIAS, "w");
    int i;

    if (arq == NULL) {
        printf("Erro ao salvar o arquivo de categorias!\n");
        return;
    }

    for (i = 0; i < total; i++) {
        fprintf(arq, "%d;%s\n", lista[i].codigo, lista[i].nome);
    }

    fclose(arq);
}

/* produtos.txt: codigo;nome;prateleira;preco;quantidade;codigoCategoria;ativo */
int carregarProdutos(Produto lista[]) {
    FILE *arq = fopen(ARQ_PRODUTOS, "r");
    char linha[200];
    int total = 0;

    if (arq == NULL) {
        return 0;
    }

    while (total < MAX_PRODUTOS && fgets(linha, sizeof(linha), arq) != NULL) {
        Produto p;
        int lidos;

        p.ativo = 1; /* arquivos antigos (Tarefa 4) nao tem o campo ativo */
        lidos = sscanf(linha, "%d;%49[^;];%c;%f;%d;%d;%d",
                       &p.codigo, p.nome, &p.prateleira, &p.precoUnitario,
                       &p.quantidadeEstoque, &p.codigoCategoria, &p.ativo);

        if (lidos >= 6) {
            p.disponivelParaVenda = (p.quantidadeEstoque > 0) ? 1 : 0;
            lista[total] = p;
            total++;
        }
    }

    fclose(arq);
    return total;
}

void salvarProdutos(Produto lista[], int total) {
    FILE *arq = fopen(ARQ_PRODUTOS, "w");
    int i;

    if (arq == NULL) {
        printf("Erro ao salvar o arquivo de produtos!\n");
        return;
    }

    for (i = 0; i < total; i++) {
        fprintf(arq, "%d;%s;%c;%.2f;%d;%d;%d\n",
                lista[i].codigo, lista[i].nome, lista[i].prateleira,
                lista[i].precoUnitario, lista[i].quantidadeEstoque,
                lista[i].codigoCategoria, lista[i].ativo);
    }

    fclose(arq);
}
