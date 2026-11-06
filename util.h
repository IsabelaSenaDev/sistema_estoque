#ifndef UTIL_H
#define UTIL_H

/* Funcoes GENERICAS (servem para qualquer programa): leitura do teclado
   e checagens de texto. Nada aqui conhece produto ou categoria. */

void limparBuffer(void);
void lerLinha(char *destino, int tamanho);
int lerInteiro(const char *mensagem);
float lerFloat(const char *mensagem);

/* 1 se o texto estiver vazio ou so com espacos */
int textoEmBranco(const char *texto);

/* 1 se o texto tiver ';' (usado como separador nos arquivos) */
int temPontoEVirgula(const char *texto);

#endif
