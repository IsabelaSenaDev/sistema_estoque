#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "produto.h"
#include "util.h"

/* Regra do dominio: disponivel e sempre derivado da quantidade */
static void atualizarDisponibilidade(Produto *p) {
    p->disponivelParaVenda = (p->quantidadeEstoque > 0) ? 1 : 0;
}

/* Regra do dominio: prateleira so aceita A, B, C ou D */
static int prateleiraValida(char prateleira) {
    return prateleira == 'A' || prateleira == 'B' ||
           prateleira == 'C' || prateleira == 'D';
}

void exibirProduto(Produto p, Categoria categorias[], int totalCategorias) {
    int pos;

    printf("Codigo..................: %d\n", p.codigo);
    printf("Nome....................: %s\n", p.nome);
    printf("Prateleira..............: %c\n", p.prateleira);
    printf("Preco unitario..........: %.2f\n", p.precoUnitario);
    printf("Quantidade em estoque...: %d\n", p.quantidadeEstoque);
    printf("Disponivel para venda...: %d (%s)\n",
           p.disponivelParaVenda,
           p.disponivelParaVenda ? "Sim" : "Nao");

    pos = buscarCategoriaPorCodigo(categorias, totalCategorias, p.codigoCategoria);
    if (pos != -1) {
        printf("Categoria...............: %s\n", categorias[pos].nome);
    } else {
        printf("Categoria...............: desconhecida\n");
    }
}

int buscarPorCodigo(Produto lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

int buscarAtivoPorCodigo(Produto lista[], int total, int codigo) {
    int pos = buscarPorCodigo(lista, total, codigo);
    if (pos != -1 && lista[pos].ativo == 0) {
        return -1; /* removido: e como se nao existisse */
    }
    return pos;
}

int buscarAtivoPorNome(Produto lista[], int total, const char *nome) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].ativo == 1 && strcmp(lista[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

int cadastrarProduto(Produto lista[], int total,
                     Categoria categorias[], int totalCategorias) {
    Produto p;

    if (total >= MAX_PRODUTOS) {
        printf("Lista cheia! Limite de %d produtos atingido.\n\n", MAX_PRODUTOS);
        return total;
    }

    if (totalCategorias == 0) {
        printf("Nenhuma categoria cadastrada! Cadastre ao menos uma categoria antes.\n\n");
        return total;
    }

    /* Codigo: positivo e unico (considera ate os produtos removidos,
       pois eles continuam na lista) */
    do {
        p.codigo = lerInteiro("Codigo do produto: ");
        if (p.codigo <= 0) {
            printf("Codigo invalido! Deve ser um numero positivo.\n");
        } else if (buscarPorCodigo(lista, total, p.codigo) != -1) {
            printf("Codigo ja cadastrado! Digite outro codigo.\n");
            p.codigo = 0;
        }
    } while (p.codigo <= 0);

    /* Nome */
    do {
        printf("Nome do produto: ");
        lerLinha(p.nome, sizeof(p.nome));
        if (textoEmBranco(p.nome)) {
            printf("Nome invalido! O nome nao pode ficar em branco.\n");
        } else if (temPontoEVirgula(p.nome)) {
            printf("Nome invalido! Nao use o caractere ';'.\n");
            p.nome[0] = '\0';
        }
    } while (textoEmBranco(p.nome));

    /* Prateleira */
    do {
        printf("Prateleira (A, B, C ou D): ");
        scanf(" %c", &p.prateleira);
        limparBuffer();
        p.prateleira = (char) toupper((unsigned char) p.prateleira);
        if (!prateleiraValida(p.prateleira)) {
            printf("Prateleira invalida! Use apenas A, B, C ou D.\n");
        }
    } while (!prateleiraValida(p.prateleira));

    /* Preco */
    do {
        p.precoUnitario = lerFloat("Preco unitario: ");
        if (p.precoUnitario <= 0) {
            printf("Preco invalido! Deve ser maior que zero.\n");
        }
    } while (p.precoUnitario <= 0);

    /* Quantidade */
    do {
        p.quantidadeEstoque = lerInteiro("Quantidade em estoque: ");
        if (p.quantidadeEstoque < 0) {
            printf("Quantidade invalida! Nao pode ser negativa.\n");
        }
    } while (p.quantidadeEstoque < 0);

    /* Categoria: precisa existir */
    listarCategorias(categorias, totalCategorias);
    do {
        p.codigoCategoria = lerInteiro("Codigo da categoria: ");
        if (buscarCategoriaPorCodigo(categorias, totalCategorias, p.codigoCategoria) == -1) {
            printf("Categoria nao existe! Digite um codigo da lista.\n");
            p.codigoCategoria = -1;
        }
    } while (p.codigoCategoria == -1);

    atualizarDisponibilidade(&p);
    p.ativo = 1;

    lista[total] = p;

    printf("\n===== Resumo do Cadastro =====\n");
    exibirProduto(p, categorias, totalCategorias);
    printf("===============================\n\n");

    return total + 1;
}

void listarProdutos(Produto lista[], int total,
                    Categoria categorias[], int totalCategorias) {
    int i;
    int ativos = 0;

    for (i = 0; i < total; i++) {
        if (lista[i].ativo == 1) {
            ativos++;
        }
    }

    if (ativos == 0) {
        printf("Nenhum produto foi cadastrado.\n\n");
        return;
    }

    printf("\n===== Lista de Produtos (%d) =====\n", ativos);
    for (i = 0; i < total; i++) {
        if (lista[i].ativo == 1) {
            printf("--- Produto ---\n");
            exibirProduto(lista[i], categorias, totalCategorias);
            printf("\n");
        }
    }
    printf("==================================\n\n");
}

void pesquisarProduto(Produto lista[], int total,
                      Categoria categorias[], int totalCategorias) {
    int tipo;
    int posicao;

    printf("\n--- Pesquisar Produto ---\n");
    printf("1 - Por codigo\n");
    printf("2 - Por nome\n");
    tipo = lerInteiro("Escolha como pesquisar: ");

    switch (tipo) {
        case 1: {
            int codigo = lerInteiro("Codigo do produto: ");
            posicao = buscarAtivoPorCodigo(lista, total, codigo);
            break;
        }
        case 2: {
            char nome[50];
            printf("Nome do produto: ");
            lerLinha(nome, sizeof(nome));
            posicao = buscarAtivoPorNome(lista, total, nome);
            break;
        }
        default:
            printf("Opcao invalida!\n\n");
            return;
    }

    if (posicao == -1) {
        printf("Nenhum produto foi encontrado.\n\n");
    } else {
        printf("\n===== Produto Encontrado =====\n");
        exibirProduto(lista[posicao], categorias, totalCategorias);
        printf("==============================\n\n");
    }
}

void alterarEstoque(Produto lista[], int total) {
    int codigo;
    int posicao;
    int tipo;
    int quantidade;

    codigo = lerInteiro("Codigo do produto: ");
    posicao = buscarAtivoPorCodigo(lista, total, codigo);
    if (posicao == -1) {
        printf("Produto nao encontrado.\n\n");
        return;
    }

    printf("Produto: %s | Estoque atual: %d\n",
           lista[posicao].nome, lista[posicao].quantidadeEstoque);
    printf("1 - Entrada (chegada de mercadoria)\n");
    printf("2 - Saida (venda ou retirada)\n");
    tipo = lerInteiro("Escolha o tipo de movimentacao: ");

    if (tipo != 1 && tipo != 2) {
        printf("Opcao invalida!\n\n");
        return;
    }

    do {
        quantidade = lerInteiro("Quantidade: ");
        if (quantidade <= 0) {
            printf("Quantidade invalida! Deve ser maior que zero.\n");
        }
    } while (quantidade <= 0);

    if (tipo == 1) {
        lista[posicao].quantidadeEstoque += quantidade;
    } else {
        if (quantidade > lista[posicao].quantidadeEstoque) {
            printf("Saida maior que o estoque atual! Operacao nao realizada.\n\n");
            return;
        }
        lista[posicao].quantidadeEstoque -= quantidade;
    }

    atualizarDisponibilidade(&lista[posicao]);

    printf("Estoque atualizado! Novo estoque: %d | Disponivel para venda: %s\n\n",
           lista[posicao].quantidadeEstoque,
           lista[posicao].disponivelParaVenda ? "Sim" : "Nao");
}

void removerProduto(Produto lista[], int total) {
    int codigo;
    int posicao;

    codigo = lerInteiro("Codigo do produto a remover: ");
    posicao = buscarAtivoPorCodigo(lista, total, codigo);
    if (posicao == -1) {
        printf("Produto nao encontrado.\n\n");
        return;
    }

    lista[posicao].ativo = 0; /* exclusao logica: nao apaga, so marca */
    printf("Produto \"%s\" removido com sucesso!\n\n", lista[posicao].nome);
}
