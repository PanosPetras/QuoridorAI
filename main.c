#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IOHandler.h"
#include "List.h"

int main(int argc, int* argv[]){
    char Command[100];

	char* Board = NULL;
	int size;

	struct player p1, p2;

	Listptr History = NULL;

	while (1) {
		do {
			fgets(Command, sizeof(Command), stdin);
		} while (Command == NULL);

		ProcessInput(Command);
		
		if (HandleCommand(Command, &Board, &size, &p1, &p2, &History)){
			return 0;
		}
		fflush(stdout);
	}

	if(Board != NULL){
		free(Board);
	}
	if(History != NULL){
		FreeList(&History);
	}
}