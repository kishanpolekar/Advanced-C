//
//  main.c
//  Assignment_4
//
//  Created by Kishan on 4/18/18.
//  Copyright © 2018 Kishan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <memory.h>

typedef enum { false, true } bool;

struct node {
    int size;
    int fit;
    bool isFree;
    struct node *left;
    struct node *right;
};

#define ALLOCATE_SIZE 1024
#define MAX_VALUE 9999

int heapSize;
struct node *root = NULL;
pthread_mutex_t mutex;

void PrintMemAddr()
{
    void *block = sbrk(0);
    printf("Mem Addr = %p\n",block);
}

void InitMem() {
    
    sbrk(0);
    PrintMemAddr();
}

void * Insert(struct node ** Root, int branch) {
    
    pthread_mutex_lock(&mutex);
    if ((*Root)==NULL) {
        
        (*Root)=sbrk((sizeof(struct node))+branch);
        (*Root)->size=branch;
        (*Root)->isFree=false;
        (*Root)->left=NULL;
        (*Root)->right=NULL;
    }
    else {
        pthread_mutex_unlock(&mutex);
        if (branch<(*Root)->size) {
            
            return Insert(&(*Root)->left, branch);
        }
        else {
            
            return Insert(&(*Root)->right, branch);
        }
    }
    pthread_mutex_unlock(&mutex);
    return ((void *) (*Root) + sizeof(struct node));
}

void * FindBestFit(struct node * Root, struct node * Best, int val) {
    
    if (Root!=NULL) {
       
        if (val <= Root->size){
            
            if (Root->isFree==true){
                
                //Best->size=val;
	        Best=Root;

            }
            else if (Root->left!=NULL) {
                
              Best=FindBestFit(Root->left, Best, val);
            }
            else return NULL;
        }
        else {

            if (Root->right!=NULL)
            	Best=FindBestFit(Root->right, Best, val);
            else return NULL;
        }
    }
    return ((void *) (Best));
}

void * NEW (int size) {
    
    if ((root)==NULL) {
        
        heapSize += sizeof(struct node) + size;
        return Insert(&root, size);
    }
    else {
        
        struct node best;
	best.size=MAX_VALUE;
	best.fit=MAX_VALUE;
	best.left=NULL;
	best.right=NULL;
        void * item = FindBestFit(root, &best, size);
        
        if (item == NULL) {
            
            printf("Allocated a new block\n");
            heapSize += sizeof(struct node) + size;
            if (heapSize>ALLOCATE_SIZE) {
                
                heapSize -= (sizeof(struct node) + size);
                return NULL;
            }
            else {
                
                return Insert(&root, size);
            }
        }
        else {
            
            printf("Allocated an existing block\n");
	    ((struct node *) item) -> isFree=false;
            return (item+sizeof(struct node));            
        }
    }
}

void DEL(void * memPtr) {
    
    memPtr-=sizeof(struct node);
    ((struct node *) memPtr)->isFree=true;
}

void Traverse(struct node * Root) {
    
    if ((Root->left==NULL) && (Root->right==NULL)) {
        
        printf("%d %d\n", Root->size, Root==NULL);
    }
    else if ((Root->left!=NULL) && (Root->right==NULL)) {
        
        Traverse(Root->left);
        printf("%d %d\n", Root->size, Root==NULL);
    }
    else if ((Root->left==NULL) && (Root->right!=NULL)) {
        
        Traverse(Root->right);
        printf("%d %d\n", Root->size, Root==NULL);
    }
    else {
        
        Traverse(Root->left);
        Traverse(Root->right);
        printf("%d %d\n", Root->size, Root==NULL);
    }
}

void FreeMem() {
    
    sbrk(-heapSize);
    PrintMemAddr();
}

int main(int argc, char *argv[])
{
    //           100
    //         /     \
    //        66(50)  144(123)
    //        \         \
    //         78        200
    int data[] = {100, 66, 144, 50, 78, 123, 888, 200};
    
    printf("******** Initial memory address ********\n");
    InitMem();
    
    int i;
    void *memPtr[sizeof(data)/sizeof(int)];
    for (i=0; i<sizeof(data)/sizeof(int); i++) {
        
        if (i == 3) {
            DEL(memPtr[1]); printf("Deleted %d\n",data[1]);
        }
        
        printf("Inserting %d\n",data[i]);
        memPtr[i] = NEW(data[i]);
        if (memPtr[i] == NULL) printf("Error - Not enough memory for %d\n",data[i]);
        else {
            memset(memPtr[i], 0, data[i]);
	    PrintMemAddr();
        }
        if (i == 4) {DEL(memPtr[2]); printf("Deleted %d\n",data[2]); }
    }
    
    printf("******** Traverse ********\n");
    Traverse(root);
    
    printf("******** Initial memory address ********\n");
    FreeMem();
    
    return 0;
}
