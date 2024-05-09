#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *newTree = (TreeMap *) malloc(sizeof(TreeMap));
    if (newTree == NULL) return NULL;
    newTree->root = NULL;
    newTree->current = NULL;
    newTree->lower_than = lower_than;
    return newTree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode *newNode = createTreeNode(key, value);
    if(tree->root == NULL){
        tree->root = newNode;
        return;
    }
    
    TreeNode *aux = tree->root;
    
    while(1){
        if(is_equal(tree, newNode->pair->key, aux->pair->key) == 1){
            return;
        }
        else if(tree->lower_than(newNode->pair->key, aux->pair->key) == 1){
            if(aux->left == NULL) {
                aux->left = newNode;
                newNode->parent = aux;
                tree->current = newNode;
                return;
            }
            aux = aux->left;
        } else {
            if(aux->right == NULL){
                aux->right = newNode;
                newNode->parent = aux;
                tree->current = newNode;
                return;
            }
            aux = aux->right;
        } 
    }
}

TreeNode * minimum(TreeNode * x){
    TreeNode *aux = x;

    while(aux->left != NULL) aux = aux->left;
    return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if(node->left == NULL && node->right == NULL){
        if(node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL;
        free(node);
    } else if(node->left == NULL || node->right == NULL){
        if(node->left == NULL){
            node->right->parent = node->parent;
            if(node->parent->left == node) node->parent->left = node->right;
            else node->parent->right = node->right;
        }
        else {
            node->left->parent = node->parent;
            if(node->parent->left == node) node->parent->left = node->left;
            else node->parent->right = node->left;
        }
        free(node);
    } else{
        TreeNode *aux = minimum(node->right);
        node->pair = aux->pair;
        removeNode(tree, aux);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;

    while(aux != NULL){
        if(is_equal(tree, aux->pair->key, key)){
            tree->current = aux;
            return aux->pair;
        } else if(tree->lower_than(key, aux->pair->key) == 1){
            aux = aux->left;
        } else aux = aux->right;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *ubNode = tree->root;

    while(ubNode != NULL){
        if(is_equal(tree, ubNode->pair->key, key)){
            tree->current = ubNode;
            break;
        } else if(tree->lower_than(key, ubNode->pair->key) == 1){
            if(ubNode->left == NULL || tree->lower_than(ubNode->left->pair->key, key) == 1) 
                break;
            ubNode = ubNode->left;
        } else{
            if(ubNode->right == NULL) 
                return NULL;
            ubNode = ubNode->right;
        }
    }
    return ubNode->pair;;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *first = minimum(tree->root);
    return first->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *aux = tree->current;

    if(aux->right != NULL){
        aux = aux->right;
        aux = minimum(aux);
        tree->current = aux;
    } else{
        while(aux->parent != NULL){
            if(tree->lower_than(aux->pair->key, aux->parent->pair->key) == 1){
                tree->current = aux->parent;
                return aux->parent->pair;
            }
            aux = aux->parent;
        }
        return NULL;
    }
    return aux->pair;
}
