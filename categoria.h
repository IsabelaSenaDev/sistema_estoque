#ifndef CATEGORIA_H
#define CATEGORIA_H

/* DOMINIO: categoria. Regras especificas de categoria ficam aqui. */

#define MAX_CATEGORIAS 20

typedef struct Categoria {
    int codigo;
    char nome[30];
} Categoria;

/* Devolve a posicao da categoria na lista, ou -1 se nao existir */
int buscarCategoriaPorCodigo(Categoria lista[], int total, int codigo);

/* Cadastra uma categoria (valida codigo positivo/unico e nome).
   Devolve o novo total de categorias. */
int cadastrarCategoria(Categoria lista[], int total);

void listarCategorias(Categoria lista[], int total);

#endif
