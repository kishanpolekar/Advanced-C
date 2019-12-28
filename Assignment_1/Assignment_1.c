//
//  main.c
//  Assignment_1
//
//  Created by Kishan on 1/22/18.
//  Copyright © 2018 Kishan. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE 9999

struct node {
    int size;
    int fit;
    struct node *left;
    struct node *right;
};

void Insert(struct node ** Root, int branch) {
    
    if (*Root==NULL) {
        
        *Root=(struct node *)malloc(sizeof(struct node));
        (*Root)->size=branch;
        (*Root)->left=NULL;
        (*Root)->right=NULL;
    }
    else {
        
        if (branch<(*Root)->size) {
            
            Insert(&(*Root)->left, branch);
        }
        else {
            
            Insert(&(*Root)->right, branch);
        }
    }
}

void Traverse(struct node * Root) {
    
    if ((Root->left==NULL) && (Root->right==NULL)) {
        
        printf("%d ", Root->size);
    }
    else if ((Root->left!=NULL) && (Root->right==NULL)) {
        
        Traverse(Root->left);
    }
    else if ((Root->left==NULL) && (Root->right!=NULL)) {
     
        printf("%d ", Root->size);
        Traverse(Root->right);
    }
    else {
        
        Traverse(Root->left);
        printf("%d ", Root->size);
        Traverse(Root->right);
    }
}

struct node * FindBestFit(struct node * Root, struct node * Best, int val) {
    
    if (val==Root->size) {
        
        Best->fit=val;
        Best->size=Root->size;
    }
    else if (val < Root->size){
        
        if ((Root->left!=NULL) && (val < Root->left->size))
            Best=FindBestFit(Root->left, Best, val);
        else
            Best->fit=val;
            Best->size=Root->size;
    }
    else {
        
        if (Root->right!=NULL)
            Best=FindBestFit(Root->right, Best, val);
    }
    return &(*Best);
}


void DestroyTree (struct node * Root) {
    
    if (Root->left!=NULL)
        DestroyTree(Root->left);
    if (Root->right!=NULL)
        DestroyTree(Root->right);
    free(Root);
}
int main(int argc, char *argv[])
{
    //           100
    //         /     \
    //        66      128
    //       / \     /   \
    //      55  78  110  188
    int data[] = {100, 66, 128, 55, 78, 110, 188};
    struct node *root = NULL;
    
    int i;
    for (i=0; i<sizeof(data)/sizeof(int); i++) {
        Insert(&root, data[i]);
    }
    printf("Binary Tree Traversal \n");
    Traverse(root);
    printf("\n");
    
    struct node best;
    best.left = NULL;
    best.right = NULL;
    best.size = MAX_VALUE;
    best.fit = MAX_VALUE;
    
    int val = 170; // 200;
    struct node *item = FindBestFit(root, &best, val);
    
    if (item != NULL && item->fit != MAX_VALUE) printf("\nThe best fit of %d is %d\n",val, item->size);
    else printf("Not found\n");
    
    DestroyTree(root); // Free the memory
    
    return 0;
}
