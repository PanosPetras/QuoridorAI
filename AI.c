#include "AI.h"
#include "Board.h"
#include "List.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int PathScore(char* Board, int size, char* player, struct player white, struct player black, int depth, int* Scores, Listptr History){
    int b;
    struct player* p;
    if(!strcmp(player, "white")){
        p = &white;
        b = 0;
    } else {
        p = &black;
        b = 1;
    }

    if(depth == 0){
        Scores = malloc(size * size * sizeof(int));
        for (int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                *(Scores + i * size + j) = -10;
            }
        }
        for (int i = 0; i < size; i++){
            *(Scores + i + b * size * (size - 1)) = 0;
        }

        History = NULL;
    }


    if(*(Scores + p->y * size + p->x) == -10){
        *(Scores + p->y * size + p->x) = -20;

        int score[4], status, k = 0;
        char ver[4];
        struct player temp;
        struct vertex v = PlayerToVertex(*p, size), t;
        VertexToString(v, size, ver);
        for (int i = 0; i < 2; i ++){
            for (int j = 0; j < 3; j += 2){
                t.x = v.x + i * (-1 + j);
                t.y = v.y + !i * (-1 + j);
                VertexToString(t, size, ver);
                status = PlayMove(Board, size, player, ver, &white, &black, &History);
                
                if(status == 1){
                    score[k] = PathScore(Board, size, player, white, black, depth + 1, Scores, History);

                    Undo(Board, size, &white, &black, &History);
                } else {
                    score[k] = -1;
                }
                k++;
            }
        }
        *(Scores + p->y * size + p->x) = 1000;
        for (int i = 0; i < 4; i++){
            if(score[i] < *(Scores + p->y * size + p->x) && score[i] >= 0){
                *(Scores + p->y * size + p->x) = score[i];
            }
        }

        if(*(Scores + p->y * size + p->x) != 1000){
            *(Scores + p->y * size + p->x) = *(Scores + p->y * size + p->x) + 1;
        } else {
            *(Scores + p->y * size + p->x) = -1;
        }

    } else if(*(Scores + p->y * size + p->x) == -20){
        return -1;
    }
    int res = *(Scores + p->y * size + p->x);

    if(depth == 0){
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                printf("%d  =", *(Scores + i * size + j));
            }
            printf("\n");
        }
        
        free(Scores);
    }
    
    return res;
}

char* GenerateMove(char* Board, int size, char* player, struct player white, struct player black){
    struct player* pl,* en;
    if(!strcmp(player, "white")){
        pl = &white;
        en = &black;
    } else {
        pl = &black;
        en = &white;
    }

}