#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "categoria.h"
#include "produto.h"

/* Persistencia: le/grava as listas nos arquivos de texto.
   Se o arquivo nao existir, a funcao carregar devolve 0 (lista vazia). */

int carregarCategorias(Categoria lista[]);
void salvarCategorias(Categoria lista[], int total);

int carregarProdutos(Produto lista[]);
void salvarProdutos(Produto lista[], int total);

#endif
