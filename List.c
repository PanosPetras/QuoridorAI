#include "List.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Listptr InitList(Listptr* p, char* data){
    if(*p == NULL){
        *p = malloc(sizeof(listnode));
    }
    (*p)->next = NULL;

    (*p)->data = malloc(strlen(data) * sizeof(char));
    strcpy((*p)->data, data);

    return *p;
}

int len(Listptr p){
    int count = 0;
    while (p != NULL){
        p = p->next;
        count++;
    }
    
    return count;
}

int find(Listptr p, char* element){
    int index = 0;

    while (p != NULL){
        if(!strcmp(p->data, element)){
            return index;
        }
        index++;
        p = p->next;
    }

    return -1;    
}

char* get(Listptr p, int index){
    int size = len(p);

    if(index >= size || index < size * (-1)){
        return "";
    }
    if(index < 0){
        index = size + index;
    }

    for (int i = 0; i < index; i++){
        p = p->next;
    }

    return p->data;    
}

void InsertAtStart(Listptr* p, char* data){
    listnode* l = malloc(sizeof(listnode));
    l->next = *p;

    l->data = malloc(strlen(data) * sizeof(char));
    strcpy(l->data, data);

    *p = l;
}

void InsertAtEnd(Listptr* p, char* data){
    if(*p != NULL){
        Listptr it = *p;
        while (it->next != NULL){
            it = it->next;
        }
        it->next = malloc(sizeof(listnode));
        it->next->next = NULL;

        it->next->data = malloc(strlen(data) * sizeof(char));
        strcpy(it->next->data, data);
    } else {
        listnode* l = malloc(sizeof(listnode));
        l->next = NULL;

        l->data = malloc(strlen(data) * sizeof(char));
        strcpy(l->data, data);

        *p = l;
    }
}

void Remove(Listptr* p, int index){
    int size = len(*p);

    if(index >= size || index < size * (-1)){
        return;
    }
    if(index < 0){
        index = size + index;
    }
    Listptr rem;

    if(index > 0){
        Listptr save = *p;
        for (int i = 0; i < index - 1; i++){
            *p = (*p)->next;
        }

        rem = (*p)->next;
        if((*p)->next->next != NULL){
            (*p)->next = (*p)->next->next;
        } else {
            (*p)->next = NULL;
        }
        *p = save;
    } else {
        rem = *p;
        if(len(*p) > 1){
            *p = (*p)->next;
        } else {
            *p = NULL;
        }
    }

    free(rem->data);
    free(rem);
}

void PrintList(Listptr p){
	while (p !=NULL){
		printf("%s", p->data);
		p = p->next;
	}
}

void FreeList(Listptr* p){
    for (int i = len(*p) - 1; i >= 0; i--){
        Remove(p, i);
    }    
}