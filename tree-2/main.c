
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
    while (list_size(l))
        {
            void* d=list_pop_front(l);
            documento_destroy(d);
        }
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

    int n, n2, num;
    
    fscanf(pFile, "%d%*c", &n);

    for(int i = 0; i < n; i++) {
        char *palavra = malloc(sizeof(char) * 100);
        List* listaDocumentos = list_construct();
        fscanf(pFile, "%s%*c%d%*c", palavra, &n2);
        for(int j =0; j<n2;j++){
            char nomeDocumento[200];
            fscanf(pFile, "%s %d%*c", nomeDocumento, &num);
            Documento* doc = documento_construct(nomeDocumento);
            documento_aumenta(doc, (num-1));//o documento quando criado já é criado com 1 de qtd
            list_push_front(listaDocumentos, doc);
        }
        binary_tree_add_recursive(bt, palavra,listaDocumentos);
    }


    fclose(pFile);



    scanf("%s", arq);

    FILE* File = fopen(arq,"r");

    if ( !File ){
        printf("Arquivo %s nao foi encontrado!\n", arq);
        exit(1);   
    }

    fscanf(File, "%d%*c", &n);

    Vector* stopWords = vector_construct();

    for(int i = 0; i < n; i++){
        char *palavra = calloc(30, sizeof(char));
        fscanf(File, "%s%*c", palavra);
        vector_push_back(stopWords, palavra);
    }

    fclose(File);




    char busca[100];
    Vector* documentos= vector_construct();

    while(scanf("%*c%s", busca) != EOF){
        if(vector_binary_search(stopWords, busca, cmp_str) == -1){
            void* lista = binary_tree_get(bt, busca);

            if(lista != NULL){
                List* listaDocumentos = (List*)lista;
                ListIterator* it = list_front_iterator(listaDocumentos);

                while(!(list_iterator_is_over(it))){
                    void* docIt = *list_iterator_next(it);
                    int idx = vector_find(documentos, docIt, documento_cmp);
                    if(idx==-1){
                        Documento* doc = documento_construct(documento_get_nome(docIt));
                        documento_aumenta(doc, (documento_get_qtd(docIt)-1));
                        vector_push_back(documentos, doc);
                    }
                    
                    else{
                        documento_aumenta(vector_get(documentos, idx), (documento_get_qtd(docIt)));
                    }
                    
                }
                list_iterator_destroy(it);
            }
        }
    }

    vector_sort(documentos, documento_cmp_ord);

    int size = vector_size(documentos);

    for(int i = 0; i < size; i++){
        void *pair = vector_get(documentos, i);
        if(i<10){
            printf("%s %d\n", documento_get_nome(pair), documento_get_qtd(pair));
        }
        documento_destroy(pair);
    }

    vector_destroy(documentos); 

    size = vector_size(stopWords);
    for(int i = 0; i < size; i++){
        void *pair = vector_get(stopWords, i);
        char* palavra = (char*)pair;
        free(palavra);
    }

    vector_destroy(stopWords);
    binary_tree_destroy(bt);

    return 0;
}
