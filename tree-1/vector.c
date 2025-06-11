#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

struct Vector
{
    data_type *data;
    int size;
    int allocated;
};

Vector *vector_construct(){
    Vector* v=(Vector*)calloc(1, sizeof(Vector));
    v->allocated=10;
    v->size=0;
    v->data=(data_type*)calloc(v->allocated, sizeof(data_type));
    return v;
}

void vector_destroy(Vector *v){
    free(v->data);
    free(v);
}

void vector_push_back(Vector *v, data_type val){
    if(v->size >= v->allocated){
        v->allocated+=v->allocated;
        v->data=(data_type*)realloc(v->data, v->allocated * sizeof(data_type));
    }
    v->data[v->size]=val;
    v->size++;
}

int vector_size(Vector *v){
    return v->size;
}

data_type vector_get(Vector *v, int i){
    if(i<0 || i>=v->size){
        printf("ERRO!! INDICE INVALIDO!!\n");
        exit(1);
    }
    else{
        return v->data[i];
    }
}

void vector_set(Vector *v, int i, data_type val){
    if(i<0 || i>=v->size){
        printf("ERRO!! INDICE INVALIDO!!\n");
        exit(1);
    }
    else{
        v->data[i]=val;
    }
}

int vector_find(Vector *v, data_type val){
    for(int i=0; i<v->size; i++){
        if(vector_get(v, i)== val) return i;
    }
    return -1;
}

data_type vector_max(Vector *v, CompareFunction cmp_fn){
    data_type val =vector_get(v, 0) ;
    for(int i=0; i<v->size; i++){
        if(cmp_fn(vector_get(v, i), val) > 0) val=vector_get(v, i);
    }
    return val;
}

data_type vector_min(Vector *v, CompareFunction cmp_fn){
    data_type val =vector_get(v, 0) ;
    for(int i=0; i<v->size; i++){
        if(cmp_fn(vector_get(v, i), val) < 0) val=vector_get(v, i);
    }
    return val;
}

int vector_argmax(Vector *v, CompareFunction cmp_fn){
    return vector_find(v, vector_max(v, cmp_fn));
}

int vector_argmin(Vector *v, CompareFunction cmp_fn){
    return vector_find(v, vector_min(v, cmp_fn));
}

data_type vector_remove(Vector *v, int i){
    data_type r = v->data[i];
    for(int j = i+1;j < v->size; j++){
        v->data[j-1] = v->data[j];
    }
    v->size--;
    return r;
}

data_type vector_pop_front(Vector *v){
    data_type r =vector_remove(v, 0);
    return r;
}

data_type vector_pop_back(Vector *v){
    data_type r =vector_remove(v, v->size-1);
    return r;
}

void vector_insert(Vector *v, int i, data_type val){
    if(v->size==v->allocated){
        v->allocated+=v->allocated;
        v->data=(data_type*)realloc(v->data, v->allocated * sizeof(data_type));
    }
    for(int j = v->size ;j >=i ; j--){
        v->data[j+1]=v->data[j];
    }
    v->data[i]=val;
    v->size++;
}

void vector_swap(Vector *v, int i, int j){
    data_type r = vector_get(v, i);
    vector_set(v, i, vector_get(v, j));
    vector_set(v, j, r);
}

void vector_sort(Vector *v, CompareFunction cmp_fn) {
    int nTrocas = 1, j;

    if (v == NULL || v->size < 2) {
        return;
    }

    while (nTrocas) {
        nTrocas = 0;
        for (j = 0; j < v->size - 1; j++) {
            if (cmp_fn(v->data[j], v->data[j + 1]) > 0) {
                vector_swap(v, j, j + 1);
                nTrocas++;
            }
        }
    }
}


int vector_binary_search(Vector *v, data_type val){
    int max, min, med;
    min=0;
    max=v->size-1;
    while(max<v->size){
        med=(max+min)/2;
        if(v->data[med]==val){
            return med;
        }
        else if(v->data[med]<val){
            min=med+1;
        }
        else{
            max=med-1;
        }

        if(min>max){
            break;
        }
    }
    return -1;
}

void vector_reverse(Vector *v){
    int i, j;
    i=0;
    j=v->size-1;
    while(i<j){
        vector_swap(v, i, j);
        i++;
        j--;
    }
}