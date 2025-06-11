#include "hash.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

struct HashTable{
    List** l;
    int size, num;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
};

struct HashTableIterator{
    HashTable* h;
    int num;
    int idx;
    ListIterator* it;
};


HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn){
    HashTable* h=(HashTable*)calloc(1, sizeof(HashTable));
    h->size=table_size;
    h->hash_fn=hash_fn;
    h->cmp_fn=cmp_fn;
    h->l=(List**)calloc(h->size, sizeof(List*));
    return h;
}

void *hash_table_set(HashTable *h, void *key, void *val){
    int idx=h->hash_fn(h, key);
    HashTableItem* item;
    if(h->l[idx]==NULL){
        h->l[idx]=list_construct();
    }
    ListIterator* it = list_front_iterator(h->l[idx]);
    while(!list_iterator_is_over(it)){
        item=it->current->value;
        if(h->cmp_fn(key, item->key)==0){
            void* aux=item->val;
            item->val=val;
            list_iterator_destroy(it);
            return aux;
        }
        list_iterator_next(it);
    }
    HashTableItem* new_item=(HashTableItem*)calloc(1, sizeof(HashTableItem));
    new_item->key=key;
    new_item->val=val;
    list_push_front(h->l[idx], new_item);
    h->num++;
    list_iterator_destroy(it);
    return NULL;
}

void *hash_table_get(HashTable *h, void *key){
    int idx=h->hash_fn(h, key);
    if(h->l[idx]==NULL){
        return NULL;
    }
    ListIterator* it = list_front_iterator(h->l[idx]);
    while(!list_iterator_is_over(it)){
        HashTableItem* item=it->current->value;
        if(h->cmp_fn(key, item->key)==0){
            list_iterator_destroy(it);
            return item->val;
        }
        list_iterator_next(it);
    }
    list_iterator_destroy(it);
    return NULL;
}

void *hash_table_pop(HashTable *h, void *key){
    int idx=h->hash_fn(h, key);
    if(h->l[idx]==NULL){
        return NULL;
    }
    ListIterator* it = list_front_iterator(h->l[idx]);
    while(!list_iterator_is_over(it)){
        HashTableItem* item=it->current->value;
        if(h->cmp_fn(key, item->key)==0){
            void* aux=item->val;
            list_remove(h->l[idx], it->current);
            h->num--;
            list_iterator_destroy(it);
            return aux;
        }
        list_iterator_next(it);
    }
    list_iterator_destroy(it);
    return NULL;
}

int hash_table_size(HashTable *h){
    return h->size;
}

int hash_table_num_elems(HashTable *h){
    return h->num;
}

void hash_table_destroy(HashTable *h){
    for(int i=0; i<h->size; i++){
        if(h->l[i]!=NULL){
            ListIterator* it = list_front_iterator(h->l[i]);
            while(!list_iterator_is_over(it)){
                HashTableItem* item=it->current->value;
                free(item);
                list_iterator_next(it);
            }
            list_iterator_destroy(it);
            list_destroy(h->l[i]);
        }
    }
    free(h->l);
    free(h);
}

HashTableIterator *hash_table_iterator(HashTable *h){
    HashTableIterator* it=(HashTableIterator*)calloc(1, sizeof(HashTableIterator));
    it->h=h;
    it->idx=0;
    it->it=NULL;
    return it;
}

int hash_table_iterator_is_over(HashTableIterator *it){
    if(it->num>=it->h->num){
        return 1;
    }
    if(it->idx>=it->h->size){
        return 1;
    }
    if(it->it==NULL){
        return 0;
    }
    if(list_iterator_is_over(it->it)){
        return 1;
    }
    return 0;
}

HashTableItem *hash_table_iterator_next(HashTableIterator *it){
    if(it->it!=NULL){
        list_iterator_next(it->it);
    }
    while(it->idx<it->h->size){
        if(it->h->l[it->idx]!=NULL){
            if(it->it==NULL){
                it->it=list_front_iterator(it->h->l[it->idx]);
            }
            if(!list_iterator_is_over(it->it)){
                HashTableItem* item=it->it->current->value;
                it->num++;
                return item;
            }
            else{
                list_iterator_destroy(it->it);
            }
        }
        it->idx++;
        if(it->idx<it->h->size){
            it->it=NULL;
        }
    }
    return NULL;
}

void hash_table_iterator_destroy(HashTableIterator *it){
    list_iterator_destroy(it->it);
    free(it);
}

Vector *hash_to_vector(HashTable *h){
    Vector* v=vector_construct();
    HashTableIterator *it = hash_table_iterator(h);
    while (!hash_table_iterator_is_over(it))
    {
        HashTableItem *pair = hash_table_iterator_next(it);
        vector_push_back(v, pair);
    }
    hash_table_iterator_destroy(it);
    return v;
}
