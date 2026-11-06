#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ---------------------------------------------------------
   Sistema de Estoque - Projeto Final
   Tarefa 4: Categorias e Persistencia em Arquivo
   --------------------------------------------------------- */

#define MAX_PRODUTOS 50
#define MAX_CATEGORIAS 20
#define ARQ_PRODUTOS "produtos.txt"
#define ARQ_CATEGORIAS "categorias.txt"

typedef struct Categoria {
    int codigo;
    char nome[30];
} Categoria;

typedef struct Produto {
    int codigo;
    char nome[50];
    char prateleira;
    float precoUnitario;
    int quantidadeEstoque;
    int disponivelParaVenda; /* calculado: 1 se quantidade > 0, senao 0 */
    int codigoCategoria;     /* so o codigo; o nome fica na lista de categorias */
} Produto;

/* ===================== LEITURA DO TECLADO ===================== */

void lerLinha(char *destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
}

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int lerInteiro(const char *mensagem) {
    int valor;
    int ok;
    do {
        printf("%s", mensagem);
        ok = scanf("%d", &valor);
        limparBuffer();
        if (!ok) {
            printf("Entrada invalida. Digite um numero inteiro.\n");
        }
    } while (!ok);
    return valor;
}

float lerFloat(const char *mensagem) {
    float valor;
    int ok;
    do {
        printf("%s", mensagem);
        ok = scanf("%f", &valor);
        limparBuffer();
        if (!ok) {
            printf("Entrada invalida. Digite um numero (ex: 12.50).\n");
        }
    } while (!ok);
    return valor;
}

/* Devolve 1 se o texto estiver vazio ou so com espacos, senao 0 */
int nomeEmBranco(const char *nome) {
    int i;
    for (i = 0; nome[i] != '\0'; i++) {
        if (!isspace((unsigned char) nome[i])) {
            return 0;
        }
    }
    return 1;
}

/* Devolve 1 se o texto tem ';' (o ';' separa os campos no arquivo) */
int temPontoEVirgula(const char *texto) {
    return strchr(texto, ';') != NULL;
}

/* ========================== CATEGORIAS ========================== */

/* Busca categoria pelo codigo. Devolve a posicao ou -1. */
int buscarCategoriaPorCodigo(Categoria lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
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

/* Devolve o novo total de categorias */
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
            c.codigo = 0; /* forca repetir a leitura */
        }
    } while (c.codigo <= 0);

    /* Nome: nao pode ficar em branco */
    do {
        printf("Nome da categoria: ");
        lerLinha(c.nome, sizeof(c.nome));
        if (nomeEmBranco(c.nome)) {
            printf("Nome invalido! O nome nao pode ficar em branco.\n");
        } else if (temPontoEVirgula(c.nome)) {
            printf("Nome invalido! Nao use o caractere ';'.\n");
            c.nome[0] = '\0';
        }
    } while (nomeEmBranco(c.nome));

    lista[total] = c;
    printf("\nCategoria cadastrada com sucesso!\n\n");
    return total + 1;
}

/* ============================ ARQUIVOS ============================ */

/* Formato de categorias.txt: uma categoria por linha -> codigo;nome */
int carregarCategorias(Categoria lista[]) {
    FILE *arq = fopen(ARQ_CATEGORIAS, "r");
    char linha[100];
    int total = 0;

    if (arq == NULL) {
        return 0; /* arquivo ainda nao existe: comeca vazio */
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

/* Formato de produtos.txt: codigo;nome;prateleira;preco;quantidade;codigoCategoria */
int carregarProdutos(Produto lista[]) {
    FILE *arq = fopen(ARQ_PRODUTOS, "r");
    char linha[200];
    int total = 0;

    if (arq == NULL) {
        return 0; /* arquivo ainda nao existe: comeca vazio */
    }

    while (total < MAX_PRODUTOS && fgets(linha, sizeof(linha), arq) != NULL) {
        Produto p;
        if (sscanf(linha, "%d;%49[^;];%c;%f;%d;%d",
                   &p.codigo, p.nome, &p.prateleira, &p.precoUnitario,
                   &p.quantidadeEstoque, &p.codigoCategoria) == 6) {
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
        fprintf(arq, "%d;%s;%c;%.2f;%d;%d\n",
                lista[i].codigo, lista[i].nome, lista[i].prateleira,
                lista[i].precoUnitario, lista[i].quantidadeEstoque,
                lista[i].codigoCategoria);
    }

    fclose(arq);
}

/* =========================== PRODUTOS =========================== */

/* Agora recebe tambem a lista de categorias, para mostrar o NOME */
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

int buscarPorNome(Produto lista[], int total, const char *nome) {
    int i;
    for (i = 0; i < total; i++) {
        if (strcmp(lista[i].nome, nome) == 0) {
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

    /* Sem categoria nao da para cadastrar produto */
    if (totalCategorias == 0) {
        printf("Nenhuma categoria cadastrada! Cadastre ao menos uma categoria antes.\n\n");
        return total;
    }

    /* --- Codigo: positivo e que ainda nao exista --- */
    do {
        p.codigo = lerInteiro("Codigo do produto: ");
        if (p.codigo <= 0) {
            printf("Codigo invalido! Deve ser um numero positivo.\n");
        } else if (buscarPorCodigo(lista, total, p.codigo) != -1) {
            printf("Codigo ja cadastrado! Digite outro codigo.\n");
            p.codigo = 0;
        }
    } while (p.codigo <= 0);

    /* --- Nome --- */
    do {
        printf("Nome do produto: ");
        lerLinha(p.nome, sizeof(p.nome));
        if (nomeEmBranco(p.nome)) {
            printf("Nome invalido! O nome nao pode ficar em branco.\n");
        } else if (temPontoEVirgula(p.nome)) {
            printf("Nome invalido! Nao use o caractere ';'.\n");
            p.nome[0] = '\0';
        }
    } while (nomeEmBranco(p.nome));

    /* --- Prateleira --- */
    do {
        printf("Prateleira (A, B, C ou D): ");
        scanf(" %c", &p.prateleira);
        limparBuffer();
        p.prateleira = (char) toupper((unsigned char) p.prateleira);
        if (p.prateleira != 'A' && p.prateleira != 'B' &&
            p.prateleira != 'C' && p.prateleira != 'D') {
            printf("Prateleira invalida! Use apenas A, B, C ou D.\n");
        }
    } while (p.prateleira != 'A' && p.prateleira != 'B' &&
             p.prateleira != 'C' && p.prateleira != 'D');

    /* --- Preco --- */
    do {
        p.precoUnitario = lerFloat("Preco unitario: ");
        if (p.precoUnitario <= 0) {
            printf("Preco invalido! Deve ser maior que zero.\n");
        }
    } while (p.precoUnitario <= 0);

    /* --- Quantidade --- */
    do {
        p.quantidadeEstoque = lerInteiro("Quantidade em estoque: ");
        if (p.quantidadeEstoque < 0) {
            printf("Quantidade invalida! Nao pode ser negativa.\n");
        }
    } while (p.quantidadeEstoque < 0);

    /* --- Categoria: mostra as existentes e valida o codigo digitado --- */
    listarCategorias(categorias, totalCategorias);
    do {
        p.codigoCategoria = lerInteiro("Codigo da categoria: ");
        if (buscarCategoriaPorCodigo(categorias, totalCategorias, p.codigoCategoria) == -1) {
            printf("Categoria nao existe! Digite um codigo da lista.\n");
            p.codigoCategoria = -1;
        }
    } while (p.codigoCategoria == -1);

    /* --- Disponivel para venda: calculado --- */
    p.disponivelParaVenda = (p.quantidadeEstoque > 0) ? 1 : 0;

    lista[total] = p;

    printf("\n===== Resumo do Cadastro =====\n");
    exibirProduto(p, categorias, totalCategorias);
    printf("===============================\n\n");

    return total + 1;
}

void listarProdutos(Produto lista[], int total,
                    Categoria categorias[], int totalCategorias) {
    int i;

    if (total == 0) {
        printf("Nenhum produto foi cadastrado.\n\n");
        return;
    }

    printf("\n===== Lista de Produtos (%d) =====\n", total);
    for (i = 0; i < total; i++) {
        printf("--- Produto %d ---\n", i + 1);
        exibirProduto(lista[i], categorias, totalCategorias);
        printf("\n");
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
            posicao = buscarPorCodigo(lista, total, codigo);
            break;
        }
        case 2: {
            char nome[50];
            printf("Nome do produto: ");
            lerLinha(nome, sizeof(nome));
            posicao = buscarPorNome(lista, total, nome);
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

/* ===================== SUBMENU DE CATEGORIAS ===================== */

/* Devolve o total de categorias atualizado */
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
                salvarCategorias(categorias, totalCategorias); /* salva a cada cadastro */
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

/* ============================== MAIN ============================== */

int main(void) {
    Produto produtos[MAX_PRODUTOS];
    Categoria categorias[MAX_CATEGORIAS];
    int totalProdutos;
    int totalCategorias;
    int opcao;

    /* Carrega os dados dos arquivos ao iniciar */
    totalCategorias = carregarCategorias(categorias);
    totalProdutos = carregarProdutos(produtos);

    do {
        printf("========== MENU PRINCIPAL ==========\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Pesquisar Produto\n");
        printf("4 - Categorias\n");
        printf("5 - Sair\n");
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1:
                totalProdutos = cadastrarProduto(produtos, totalProdutos,
                                                 categorias, totalCategorias);
                salvarProdutos(produtos, totalProdutos); /* salva a cada cadastro */
                break;
            case 2:
                listarProdutos(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 3:
                pesquisarProduto(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 4:
                totalCategorias = menuCategorias(categorias, totalCategorias);
                break;
            case 5:
                /* Salva as duas listas antes de encerrar */
                salvarCategorias(categorias, totalCategorias);
                salvarProdutos(produtos, totalProdutos);
                printf("Dados salvos. Encerrando o sistema. Ate logo!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n\n");
        }
    } while (opcao != 5);

    return 0;
}
