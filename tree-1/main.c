
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"
#include "vector.h"
#include "list.h"
#include "documento.h"



int cmp_str(void *a, void *b)
{
    return strcmp((char *)a, (char *)b);
}

void str_destroy(void* a){
    char* s= (char*)a;
    free(s);
}

void lista_destroy(void* a){
    List* l= (List*)a;
    list_destroy(l);
}

int main()
{
    BinaryTree* bt = binary_tree_construct(cmp_str, str_destroy, lista_destroy);

    char arq[200];
    scanf("%s%*c", arq);

    FILE* pFile = fopen(arq,"r");

    if ( !pFile ){
        printf("Arquivo %s nao foi encontrado!\n", arq);
        exit(1);   
    }

    int n, n2;
    
    fscanf(pFile, "%d%*c", &n);

    for(int i = 0; i < n; i++) {
        char nomeDocumento[200];
        fscanf(pFile, "%[^:]%*c %d%*c%*c", nomeDocumento, &n2);
        for(int j =0; j<n2;j++){
            char *palavra = calloc(100, sizeof(char));
            List* listaDocumentos;
            fscanf(pFile, "%s%*c", palavra);
            void* lista_ext = binary_tree_get(bt, palavra);
            if(lista_ext == NULL) {
                listaDocumentos=list_construct();
                list_push_front(listaDocumentos, documento_construct(nomeDocumento));
                binary_tree_add(bt, palavra, listaDocumentos);
            } else {
                listaDocumentos=(List*)lista_ext;
                Documento* doc=documento_construct(nomeDocumento);
                int idx = list_get_index(listaDocumentos, doc, documento_cmp);
                if(idx==-1){
                    list_push_front(listaDocumentos, doc);
                }
                else{
                    documento_incrementa(list_get(listaDocumentos, idx));
                    documento_destroy(doc);
                }
                free(palavra);
            }
        }
    }
    fclose(pFile);

    Vector *v = binary_tree_inorder_traversal_recursive(bt);
    int size = vector_size(v);
    printf("%d\n", size);
    for(int i = 0; i < size; i++) {
        void *pair = vector_get(v, i);
        char* palavra=(char *)((KeyValPair *)pair)->key;
        printf("%s ", palavra);
        List* lista=(List*)((KeyValPair *)pair)->value;
        printf("%d", list_size(lista));
        while (list_size(lista))
        {
            void* d=list_pop_front(lista);
            printf(" %s %d", documento_get_nome(d), documento_get_qtd(d));
            documento_destroy(d);
        }
        key_val_pair_destroy((KeyValPair *)pair);
        printf("\n");
    }

    vector_destroy(v);
    binary_tree_destroy(bt);

    return 0;
}
