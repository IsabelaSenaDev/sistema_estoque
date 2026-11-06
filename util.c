#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void lerLinha(char *destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    } else {
        limparBuffer();
    }
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

int textoEmBranco(const char *texto) {
    int i;
    for (i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char) texto[i])) {
            return 0;
        }
    }
    return 1;
}

int temPontoEVirgula(const char *texto) {
    return strchr(texto, ';') != NULL;
}
