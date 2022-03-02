#ifndef List
#define List

typedef struct node* Listptr;

typedef struct node{
    char data[30];
    Listptr next;
} listnode;

Listptr InitList(Listptr*, char*);

void FreeList(Listptr*);

int len(Listptr);

int find(Listptr, char*);

char* get(Listptr, int);

void InsertAtStart(Listptr*, char*);

void InsertAtEnd(Listptr*, char*);

void Remove(Listptr*, int);

void PrintList(Listptr);

#endif