#include "binary_tree.h"
#include <stdio.h> 
#include <stdlib.h>

struct  NodeBt{
    void *key;
    void *value;
    struct NodeBt *left;
    struct NodeBt *right;
};

struct BinaryTree{
    NodeBt *root;
    CmpFn cmp_fn;
    KeyDestroyFn key_destroy_fn;
    ValDestroyFn val_destroy_fn;
};


KeyValPair *key_val_pair_construct(void *key, void *val){
    KeyValPair *kvp = (KeyValPair*)calloc(1, sizeof(KeyValPair));
    kvp->key = key;
    kvp->value = val;
    return kvp;
}

void key_val_pair_destroy(KeyValPair *kvp){
    free(kvp);
}

NodeBt *nodeBt_construct_bt(void *key, void *value, NodeBt *left, NodeBt *right){
    NodeBt *nodeBt = (NodeBt*)calloc(1, sizeof(NodeBt));
    nodeBt->key = key;
    nodeBt->value = value;
    nodeBt->left = left;
    nodeBt->right = right;
    return nodeBt;
}

void nodeBt_destroy(NodeBt *nodeBt){
    free(nodeBt);
}

BinaryTree *binary_tree_construct(
    CmpFn cmp_fn, KeyDestroyFn key_destroy_fn,
    ValDestroyFn val_destroy_fn){
    BinaryTree *bt = (BinaryTree*)calloc(1, sizeof(BinaryTree));
    bt->cmp_fn = cmp_fn;
    bt->key_destroy_fn = key_destroy_fn;
    bt->val_destroy_fn = val_destroy_fn;
    return bt;
}

void binary_tree_add(BinaryTree *bt, void *key, void *value){
    NodeBt *new_nodeBt = nodeBt_construct_bt(key, value, NULL, NULL);
    if(bt->root == NULL){
        bt->root = new_nodeBt;
        return;
    }
    NodeBt *current = bt->root;
    NodeBt *parent = NULL;
    while(current != NULL){
        parent = current;
        if(bt->cmp_fn(key, current->key) < 0){
            current = current->left;
        }else if(bt->cmp_fn(key, current->key) > 0){
            current = current->right;
        }
        else{
            bt->val_destroy_fn(current->value);
            current->value = value;
            bt->key_destroy_fn(key);
            free(new_nodeBt);
            return;
        }
    }
    if(bt->cmp_fn(key, parent->key) < 0){
        parent->left = new_nodeBt;
    }else{
        parent->right = new_nodeBt;
    }
}

NodeBt * _binary_tree_add_recursive_aux(NodeBt *nodeBt, void *key, void *value, CmpFn cmp_fn){
    if(nodeBt == NULL){
        return nodeBt_construct_bt(key, value, NULL, NULL);
    }
    if(cmp_fn(key, nodeBt->key) < 0){
        nodeBt->left = _binary_tree_add_recursive_aux(nodeBt->left, key, value, cmp_fn);
    }else{
        nodeBt->right = _binary_tree_add_recursive_aux(nodeBt->right, key, value, cmp_fn);
    }
    return nodeBt;
}

void binary_tree_add_recursive(BinaryTree *bt, void *key, void *value){
    bt->root = _binary_tree_add_recursive_aux(bt->root, key, value, bt->cmp_fn);
}

int binary_tree_empty(BinaryTree *bt){
    return bt->root == NULL;
}

void *binary_tree_get(BinaryTree *bt, void *key){
    NodeBt *current = bt->root;
    while(current != NULL){
        if(bt->cmp_fn(key, current->key) == 0){
            return current->value;
        }else if(bt->cmp_fn(key, current->key) < 0){
            current = current->left;
        }else{
            current = current->right;
        }
    }
    return NULL;
}

NodeBt * _binary_tree_min_aux(NodeBt *nodeBt){
    if(nodeBt == NULL){
        return NULL;
    }
    while(nodeBt->left != NULL){
        nodeBt = nodeBt->left;
    }
    return nodeBt;
}

KeyValPair *binary_tree_min(BinaryTree *bt){
    NodeBt *min = _binary_tree_min_aux(bt->root);
    if(min == NULL){
        return NULL;
    }
    return key_val_pair_construct(min->key, min->value);
}

NodeBt * _binary_tree_max_aux(NodeBt *nodeBt){
    if(nodeBt == NULL){
        return NULL;
    }
    while(nodeBt->right != NULL){
        nodeBt = nodeBt->right;
    }
    return nodeBt;
}

KeyValPair *binary_tree_max(BinaryTree *bt){
    NodeBt *max = _binary_tree_max_aux(bt->root);
    if(max == NULL){
        return NULL;
    }
    return key_val_pair_construct(max->key, max->value);
}

KeyValPair *binary_tree_pop_min(BinaryTree *bt){
    KeyValPair *min = binary_tree_min(bt);
    if(min == NULL){
        return NULL;
    }
    binary_tree_remove(bt, min->key);
    return min;
}

KeyValPair *binary_tree_pop_max(BinaryTree *bt){
    KeyValPair *max = binary_tree_max(bt);
    if(max == NULL){
        return NULL;
    }
    binary_tree_remove(bt, max->key);
    return max;
}

void binary_tree_remove(BinaryTree *bt, void *key){
    NodeBt *current = bt->root;
    NodeBt *parent = NULL;
    
    while(current != NULL && bt->cmp_fn(key, current->key) != 0){
        parent = current;
        if(bt->cmp_fn(key, current->key) < 0){
            current = current->left;
        }else{
            current = current->right;
        }
    }
    if(current == NULL){
        return;
    }

    // void *tmp_key = current->key;
    // void *tmp_value = current->value;

    if(current->left == NULL && current->right == NULL){
        if(parent == NULL){
            bt->root = NULL;
        }else if(parent->left == current){
            parent->left = NULL;
        }else{
            parent->right = NULL;
        }
        nodeBt_destroy(current);
    }
    else if(current->left == NULL){
        if(parent == NULL){
            bt->root = current->right;
        }else if(parent->left == current){
            parent->left = current->right;
        }else{
            parent->right = current->right;
        }
        nodeBt_destroy(current);
    }
    else if(current->right == NULL){
        if(parent == NULL){
            bt->root = current->left;
        }else if(parent->left == current){
            parent->left = current->left;
        }else{
            parent->right = current->left;
        }
        nodeBt_destroy(current);
    }
    else{
        NodeBt *min = _binary_tree_min_aux(current->right);
        void* sucessor_key = min->key;
        void* sucessor_value = min->value;
        binary_tree_remove(bt, min->key);
        current->key = sucessor_key;
        current->value = sucessor_value;
    }
    // bt->key_destroy_fn(tmp_key);
    // bt->val_destroy_fn(tmp_value);
}

void _binary_tree_print_recursive(NodeBt *nodeBt){
    if(nodeBt == NULL){
        printf("NULL");
        return;
    }
    printf("(%d, ", *(int*)nodeBt->key);
    _binary_tree_print_recursive(nodeBt->left);
    printf(", ");
    _binary_tree_print_recursive(nodeBt->right);
    printf(")");

}

void binary_tree_print(BinaryTree *bt){
    _binary_tree_print_recursive(bt->root);
    printf("\n");
}

void _nodeBt_destroy_recursive(NodeBt *nodeBt, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn){
    if(nodeBt == NULL){
        return;
    }
    _nodeBt_destroy_recursive(nodeBt->left, key_destroy_fn, val_destroy_fn);
    _nodeBt_destroy_recursive(nodeBt->right, key_destroy_fn, val_destroy_fn);
    key_destroy_fn(nodeBt->key);
    val_destroy_fn(nodeBt->value);
    nodeBt_destroy(nodeBt);
}

void binary_tree_destroy(BinaryTree *bt){
    _nodeBt_destroy_recursive(bt->root, bt->key_destroy_fn, bt->val_destroy_fn);
    free(bt);
}

Vector *binary_tree_inorder_traversal(BinaryTree *bt){
    Vector *v = vector_construct();
    NodeBt *current = bt->root;
    Vector *s = vector_construct();
    while(current != NULL || vector_size(s) > 0){
        while(current != NULL){
            vector_push_back(s, current);
            current = current->left;
        }
        current = vector_pop_back(s);
        vector_push_back(v, key_val_pair_construct(current->key, current->value));
        current = current->right;
    }
    vector_destroy(s);
    return v;
}

Vector *binary_tree_preorder_traversal(BinaryTree *bt){
    Vector *v = vector_construct();
    NodeBt *current = bt->root;
    Vector *s = vector_construct();
    vector_push_back(s, current);
    while(vector_size(s) > 0){
        current = vector_pop_back(s);
        vector_push_back(v, key_val_pair_construct(current->key, current->value));
        if(current->right != NULL){
            vector_push_back(s, current->right);
        }
        if(current->left != NULL){
            vector_push_back(s, current->left);
        }
    }
    vector_destroy(s);
    return v;
}

Vector *binary_tree_postorder_traversal(BinaryTree *bt){
    Vector *v = vector_construct();
    NodeBt *current = bt->root;
    Vector *q1 = vector_construct();
    Vector *q2 = vector_construct();
    vector_push_back(q1, current);
    while(vector_size(q1) > 0){
        current = vector_pop_back(q1);
        vector_push_back(q2, current);
        if(current->left != NULL){
            vector_push_back(q1, current->left);
        }
        if(current->right != NULL){
            vector_push_back(q1, current->right);
        }
    }
    while(vector_size(q2) > 0){
        current = vector_pop_back(q2);
        vector_push_back(v, key_val_pair_construct(current->key, current->value));
    }
    vector_destroy(q1);
    vector_destroy(q2);
    return v;
}

Vector *binary_tree_levelorder_traversal(BinaryTree *bt){
    Vector *v = vector_construct();
    NodeBt *current = bt->root;
    Vector *q = vector_construct();
    vector_push_back(q, current);
    while(vector_size(q) > 0){
        current = vector_pop_front(q);
        vector_push_back(v, key_val_pair_construct(current->key, current->value));
        if(current->left != NULL){
            vector_push_back(q, current->left);
        }
        if(current->right != NULL){
            vector_push_back(q, current->right);
        }
    }
    vector_destroy(q);
    return v;
}

void _binary_tree_inorder_traversal_recursive_aux(NodeBt *nodeBt, Vector *v){
    if(nodeBt == NULL){
        return;
    }
    _binary_tree_inorder_traversal_recursive_aux(nodeBt->left, v);
    vector_push_back(v, key_val_pair_construct(nodeBt->key, nodeBt->value));
    _binary_tree_inorder_traversal_recursive_aux(nodeBt->right, v);
}

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt){
    Vector *v = vector_construct();
    _binary_tree_inorder_traversal_recursive_aux(bt->root, v);
    return v;
}

void _binary_tree_preorder_traversal_recursive_aux(NodeBt *nodeBt, Vector *v){
    if(nodeBt == NULL){
        return;
    }
    vector_push_back(v, key_val_pair_construct(nodeBt->key, nodeBt->value));
    _binary_tree_preorder_traversal_recursive_aux(nodeBt->left, v);
    _binary_tree_preorder_traversal_recursive_aux(nodeBt->right, v);
}

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt){
    Vector *v = vector_construct();
    _binary_tree_preorder_traversal_recursive_aux(bt->root, v);
    return v;
}

void _binary_tree_postorder_traversal_recursive_aux(NodeBt *nodeBt, Vector *v){
    if(nodeBt == NULL){
        return;
    }
    _binary_tree_postorder_traversal_recursive_aux(nodeBt->left, v);
    _binary_tree_postorder_traversal_recursive_aux(nodeBt->right, v);
    vector_push_back(v, key_val_pair_construct(nodeBt->key, nodeBt->value));
}

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt){
    Vector *v = vector_construct();
    _binary_tree_postorder_traversal_recursive_aux(bt->root, v);
    return v;
}