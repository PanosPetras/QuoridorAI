#include "AI.h"
#include "Board.h"
#include "List.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int PathScore(char* Board, int size, char* player, struct player white, struct player black){
    lowercase(player);

    if(!strcmp(player, "white")){
        int* Scores = malloc(size * size * sizeof(int));
    }
}

char* GenerateMove(char* Board, int size, char* player, struct player white, struct player black){

}