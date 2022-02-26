#include "AI.h"
#include "Board.h"
#include "List.h"
#include <stdlib.h>
#include <string.h>

int PathScore(char* Board, int size, char* player, struct player white, struct player black, int depth, int* Scores, Listptr History){
    lowercase(player);

    if(strcmp(player, "white") && strcmp(player, "black")){
        return -1;
    }
    struct player* p;
    if(!strcmp(player, "white")){
        p = &white;
    } else {
        p = &black;
    }

    if((p->x < 0 || p->x > size - 1) || (p->y < 0 || p->y > size - 1)){
        return -1;
    } else if(Winner(white, black, size)){
        return 0;
    }

    if(depth == 0){
        Scores = malloc(size * size * sizeof(int));
        for (int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                *(Scores + i * size + j) = 'e';
            }
        }

        History = NULL;
        
    }

    int d = size * 2 - 1;

    if(*(Scores + p->x * size + p->y) == 'e'){
        *(Scores + p->x * size + p->y) = 's';

        int score[4], status;
        char ver[4];
        struct player temp;
        struct vertex v = PlayerToVertex(*p, size), t;
        for (int i = 0; i < 3; i += 2){
            for (int j = 0; j < 3; j += 2){
                t.x = v.x - 1 + i;
                t.y = v.y - 1 + j;
                VertexToString(t, size, ver);
                status = PlayMove(Board, size, player, ver, &white, &black, &History);
                
                if(status == 1){
                    score[i / 2] = PathScore(Board, size, player, white, black, depth + 1, Scores, History);

                    Undo(Board, size, &white, &black, &History);
                } else {
                    score[i / 2 + j / 2] = -1;
                }
            }
        }
        *(Scores + p->x * size + p->y) = score[0];
        for (int i = 0; i < 4; i++){
            *(Scores + p->x * size + p->y) = *(Scores + p->x * size + p->y) > score[i] || score[i] < 0? *(Scores + p->x * size + p->y) : score[i];
        }
        if(*(Scores + p->x * size + p->y) != -1){
            *(Scores + p->x * size + p->y) = *(Scores + p->x * size + p->y) + 1;
        }

    } else if(*(Scores + p->x * size + p->y) == 's'){
        return -1;
    }

    if(depth == 0){
        free(Scores);
    }
    
    return *(Scores + p->x * size + p->y);
}

char* GenerateMove(char* Board, int size, char* player, struct player white, struct player black){

}