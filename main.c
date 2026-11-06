#include <stdio.h>
#include "util.h"
#include "categoria.h"
#include "produto.h"
#include "arquivo.h"

/* ---------------------------------------------------------
   Sistema de Estoque - Projeto Final (Tarefa 5)
   O main so monta os menus e chama as funcoes dos modulos:
   as regras ficam em produto.c / categoria.c, a leitura generica
   em util.c e os arquivos em arquivo.c.
   --------------------------------------------------------- */

/* Submenu de categorias. Devolve o total de categorias atualizado. */
int menuCategorias(Categoria categorias[], int totalCategorias) {
    int opcao;

    do {
        printf("---------- CATEGORIAS ----------\n");
        printf("1 - Cadastrar Categoria\n");
        printf("2 - Listar Categorias\n");
        printf("3 - Voltar\n");
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1:
                totalCategorias = cadastrarCategoria(categorias, totalCategorias);
                salvarCategorias(categorias, totalCategorias);
                break;
            case 2:
                listarCategorias(categorias, totalCategorias);
                break;
            case 3:
                printf("\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n\n");
        }
    } while (opcao != 3);

    return totalCategorias;
}

int main(void) {
    Produto produtos[MAX_PRODUTOS];
    Categoria categorias[MAX_CATEGORIAS];
    int totalProdutos;
    int totalCategorias;
    int opcao;

    totalCategorias = carregarCategorias(categorias);
    totalProdutos = carregarProdutos(produtos);

    do {
        printf("========== MENU PRINCIPAL ==========\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Pesquisar Produto\n");
        printf("4 - Alterar Estoque de um Produto\n");
        printf("5 - Remover Produto\n");
        printf("6 - Categorias\n");
        printf("7 - Sair\n");
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1:
                totalProdutos = cadastrarProduto(produtos, totalProdutos,
                                                 categorias, totalCategorias);
                salvarProdutos(produtos, totalProdutos);
                break;
            case 2:
                listarProdutos(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 3:
                pesquisarProduto(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 4:
                alterarEstoque(produtos, totalProdutos);
                salvarProdutos(produtos, totalProdutos);
                break;
            case 5:
                removerProduto(produtos, totalProdutos);
                salvarProdutos(produtos, totalProdutos);
                break;
            case 6:
                totalCategorias = menuCategorias(categorias, totalCategorias);
                break;
            case 7:
                salvarCategorias(categorias, totalCategorias);
                salvarProdutos(produtos, totalProdutos);
                printf("Dados salvos. Encerrando o sistema. Ate logo!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n\n");
        }
    } while (opcao != 7);

    return 0;
}
