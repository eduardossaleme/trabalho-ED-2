#ifndef DOCUMENTO_H
#define DOCUMENTO_H

typedef struct Documento Documento;

Documento *documento_construct(char *nome);

void documento_incrementa(void* d);

void documento_aumenta(void* d, int qtd);

int documento_get_qtd(void*d);

char* documento_get_nome(void*d);

int documento_cmp(void*d1, void*d2);

int documento_cmp_ord(void*d1, void*d2);

void documento_destroy(void* d);


#endif