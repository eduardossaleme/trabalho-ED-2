#include "documento.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Documento{
    char* nome;
    int qtd;
};

Documento *documento_construct(char *nome){
    Documento* d = (Documento*)calloc(1, sizeof(Documento));
    d->nome = (char*)calloc(strlen(nome)+1, sizeof(char));
    strcpy(d->nome, nome);
    d->qtd=1;
    return d;
}

void documento_incrementa(void* d){
    Documento* doc = (Documento*)d;
    doc->qtd++;
}

int documento_get_qtd(void*d){
    Documento* doc = (Documento*)d;
    return doc->qtd;
}

char* documento_get_nome(void*d){
    Documento* doc = (Documento*)d;
    return doc->nome;
}

int documento_cmp(void*d1, void*d2){
    Documento* doc1 = (Documento*)d1;
    Documento* doc2 = (Documento*)d2;
    return strcmp(doc1->nome, doc2->nome);
}

void documento_destroy(void* d){
    Documento* doc = (Documento*)d;
    free(doc->nome);
    free(doc);
}
