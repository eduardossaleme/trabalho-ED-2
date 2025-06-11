#include "list.h"
#include <stdio.h>
#include <stdlib.h>

struct List {
    Node *head;
    Node *last;
    int size;
};


List *list_construct(){
    List * list = (List*)calloc(1, sizeof(List));
    list->head=NULL;
    list->last=NULL;
    list->size=0;
    return list;
}

int list_size(List *l){
    return l->size;
}

void list_push_front(List *l, data_type data){
    Node* n=node_construct(data, NULL, l->head);
    if(l->size==0){
        l->last=n;
        l->head=n;
    }
    else{
        l->head->prev=n;
        l->head=n;
    }
    l->size++;
}

void list_push_back(List *l, data_type data){
    Node* n=node_construct(data, l->last, NULL);
    if(l->size==0){
        l->last=n;
        l->head=n;
    }
    else{
        l->last->next=n;
        l->last=n;
    }
    l->size++;
}

void list_print(List *l, void (*print_fn)(data_type)){
    printf("[");
    Node* n = l->head;
    while(n!=NULL){
        print_fn(n->value);
        n=n->next;
        if(n!=NULL){
            printf(", ");
        }
    }
    printf("]");
}

void list_print_reverse(List *l, void (*print_fn)(data_type)){
    printf("[");
    Node* n = l->last;
    while(n!=NULL){
        print_fn(n->value);
        n=n->prev;
        if(n!=NULL){
            printf(", ");
        }
    }
    printf("]");
}

data_type list_get(List *l, int i){
    if(i<0 || i>=l->size){
        printf("ERRO!! INDICE INVALIDO!!\n");
        exit(1);
    }
    Node* n = l->head;
    for(int j=0; j<i; j++){
        n=n->next;
    }
    return n->value;
}

int list_get_index(List *l, data_type val, CmpFunction cmp_fn){
    Node* n = l->head;
    int i=0;
    while(n!=NULL){
        if(cmp_fn(n->value, val)==0){
            return i;
        }
        n=n->next;
        i++;
    }
    return -1;
}

void list_remove(List *l, data_type val){
    Node* n = l->head;
    while(n!=NULL){
        if(n->value==val){
            if(n==l->head){
                list_pop_front(l);
                n=l->head;
            }
            else if(n==l->last){
                list_pop_back(l);
                n=NULL;
            }
            else{
                Node* aux=n->next;
                n->prev->next=n->next;
                n->next->prev=n->prev;
                node_destroy(n);
                n=aux;
                l->size--;
            }
        }
        else{
            n=n->next;
        }
    }
}

data_type list_pop_front(List *l){
    if (l->head == NULL) {
        printf("ERRO!! LISTA DE TAMANHO 0!!\n");
        exit(1);
    }

    data_type d = l->head->value;

    if (l->size == 1) {
        node_destroy(l->head);
        l->head = NULL;
        l->last = NULL;
    } else {
        Node *n = l->head->next;
        n->prev = NULL;
        node_destroy(l->head);
        l->head = n;
    }

    l->size--;

    if (l->size == 1) {
        l->last = l->head;
    }

    return d;
}

data_type list_pop_back(List *l){
    if (l->head == NULL) {
    printf("ERRO!! LISTA DE TAMANHO 0!!\n");
    exit(1);
    }

    data_type d = l->last->value;

    if (l->size == 1) {
        node_destroy(l->last);
        l->last = NULL;
        l->head = NULL;
    } else {
        Node *n = l->last->prev;
        n->next = NULL;
        node_destroy(l->last);
        l->last = n;
    }

    l->size--;

    if (l->size == 1) {
        l->head = l->last;
    }

    return d;

}

void list_destroy(List *l){
    Node* next;
    while(l->head != NULL){
        next=l->head->next;
        node_destroy(l->head);
        l->head=next;
    }
    
    free(l);
}

ListIterator *list_front_iterator(List *l){
    ListIterator* it = (ListIterator*)calloc(1, sizeof(ListIterator));
    it->current=l->head;
    return it;
}

ListIterator *list_back_iterator(List *l){
    ListIterator* it = (ListIterator*)calloc(1, sizeof(ListIterator));
    it->current=l->last;
    return it;
}

data_type *list_iterator_next(ListIterator *it){
    data_type* n = &(it->current->value);
    it->current=it->current->next;
    return n;
}

data_type *list_iterator_previous(ListIterator *it){
    data_type* n = &(it->current->value);
    it->current=it->current->prev;
    return n;
}

bool list_iterator_is_over(ListIterator *it){
    return (it->current == NULL);
}

void list_iterator_destroy(ListIterator *it){
    free(it);
}