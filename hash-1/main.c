
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "vector.h"
#include "list.h"
#include "documento.h"

int hash_fn(HashTable *table, void *key)
{
    char *str = key;

    int table_size = hash_table_size(table);

    unsigned long hash_val = 0, base = 27183;

    unsigned long random_val = 31415;

    int size = strlen(str);
    for (int i = 0; i < size; i++)

    {
        hash_val = (hash_val * random_val + str[i]) % table_size;
        random_val = (random_val * base) % (table_size - 1);
    }

    return hash_val;
}

int cmp_str(void *a, void *b)
{
    return strcmp((char *)a, (char *)b);
}

int cmp_hash(const void *a,const void *b){
    HashTableItem *item1 = (HashTableItem *)a;
    HashTableItem *item2 = (HashTableItem *)b;
    return cmp_str(item1->key, item2->key);
}

int main()
{
    HashTable *h = hash_table_construct(919, hash_fn, cmp_str);

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
            char *palavra = malloc(sizeof(char) * 100);
            List* listaDocumentos;
            fscanf(pFile, "%s%*c", palavra);
            if(hash_table_get(h, palavra) == NULL) {
                listaDocumentos=list_construct();
                list_push_front(listaDocumentos, documento_construct(nomeDocumento));
            } else {
                listaDocumentos = (List*)hash_table_get(h, palavra);
                Documento* doc=documento_construct(nomeDocumento);
                int idx = list_get_index(listaDocumentos, doc, documento_cmp);
                if(idx==-1){
                    list_push_front(listaDocumentos, doc);
                }
                else{
                    documento_incrementa(list_get(listaDocumentos, idx));
                    documento_destroy(doc);
                }
            }
            void *prev = hash_table_set(h, palavra, listaDocumentos);
            if(prev != NULL) {
                free(palavra);
            }
        }
    }
    fclose(pFile);


    printf("%d\n", hash_table_num_elems(h));
    HashTableIterator* hIt = hash_table_iterator(h);
    while(!(hash_table_iterator_is_over(hIt))) {
        void *pair = hash_table_iterator_next(hIt);
        char* palavra=(char *)((HashTableItem *)pair)->key;
        printf("%s ", palavra);
        List* lista=(List*)((HashTableItem *)pair)->val;
        printf("%d", list_size(lista));
        while (list_size(lista))
        {
            void* d=list_pop_front(lista);
            printf(" %s %d", documento_get_nome(d), documento_get_qtd(d));
            documento_destroy(d);
        }
        printf("\n");
        list_destroy(lista);
        free(palavra);
    }
    hash_table_iterator_destroy(hIt);


    hash_table_destroy(h);

    return 0;
}
