#ifndef PRODUTO_H
#define PRODUTO_H

#include "categoria.h"

/* DOMINIO: produto. Regras especificas de produto ficam aqui. */

#define MAX_PRODUTOS 50

typedef struct Produto {
    int codigo;
    char nome[50];
    char prateleira;
    float precoUnitario;
    int quantidadeEstoque;
    int disponivelParaVenda; /* sempre calculado: 1 se quantidade > 0, senao 0 */
    int codigoCategoria;     /* so o codigo; o nome fica na lista de categorias */
    int ativo;               /* 1 = normal, 0 = removido (exclusao logica) */
} Produto;

void exibirProduto(Produto p, Categoria categorias[], int totalCategorias);

/* Acha o produto pelo codigo, ATIVO OU NAO (usado p/ garantir codigo unico).
   Devolve a posicao ou -1. */
int buscarPorCodigo(Produto lista[], int total, int codigo);

/* Buscas que ignoram produtos removidos. Devolvem a posicao ou -1. */
int buscarAtivoPorCodigo(Produto lista[], int total, int codigo);
int buscarAtivoPorNome(Produto lista[], int total, const char *nome);

int cadastrarProduto(Produto lista[], int total,
                     Categoria categorias[], int totalCategorias);
void listarProdutos(Produto lista[], int total,
                    Categoria categorias[], int totalCategorias);
void pesquisarProduto(Produto lista[], int total,
                      Categoria categorias[], int totalCategorias);
void alterarEstoque(Produto lista[], int total);
void removerProduto(Produto lista[], int total);

#endif
