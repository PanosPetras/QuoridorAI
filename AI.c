#include "AI.h"
#include "Board.h"
#include "List.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int PathScore(char* Board, int size, char* player, struct player white, struct player black, int depth, int** Scores, Listptr History){
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
        *Scores = malloc(size * size * sizeof(int));
        for (int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                *(*Scores + i * size + j) = -10;
            }
        }
        for (int i = 0; i < size; i++){
            *(*Scores + i + b * size * (size - 1)) = 0;
        }

        History = NULL;
    }


    if(*(*Scores + p->y * size + p->x) == -10){
        *(*Scores + p->y * size + p->x) = -20;

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
                } else if(status == -3){
                    score[k] = 700;
                } else {
                    score[k] = -1;
                }
                k++;
            }
        }
        *(*Scores + p->y * size + p->x) = 1000;
        for (int i = 0; i < 4; i++){
            if(score[i] < *(*Scores + p->y * size + p->x) && score[i] >= 0){
                *(*Scores + p->y * size + p->x) = score[i];
            }
        }

        if(*(*Scores + p->y * size + p->x) != 1000){
            *(*Scores + p->y * size + p->x) = *(*Scores + p->y * size + p->x) + 1;
        } else {
            *(*Scores + p->y * size + p->x) = -1;
        }

    } else if(*(*Scores + p->y * size + p->x) == -20){
        return -1;
    }
    
    return *(*Scores + p->y * size + p->x);
}

int AI_IsMoveValid(char* Board, int size, char* player, char* vertex, struct player* white, struct player* black){
    struct vertex v = StringToVertex(vertex, size);

    lowercase(player);
    int d = size * 2 - 1;

    struct player* pp;
    struct player* ep;
    if(!strcmp(player, "white")){
        pp = white;
        ep = black;
    } else {
        pp = black;
        ep = white;
    }
    
    if(Board == NULL){
        return -1;
    } else if((v.x < 0 || v.x > size - 1) || (v.y < 0 || v.y > size - 1)){
        return 0;
    } else if(*(Board + d * (pp->y + v.y) + (pp->x + v.x)) == '-' || *(Board + d * (pp->y + v.y) + (pp->x + v.x)) == '|'){
        return 0;
    } else {
        if(IsOnVertex(*white, v) || IsOnVertex(*black, v)){
            return -3;
        }
        int distx = abs(pp->x - v.x);
        int disty = abs(pp->y - v.y);
        if(!((distx == 0 && disty == 1) || (distx == 1 && disty == 0))){
            if((distx == 0 && disty == 2) || (distx == 2 && disty == 0) || (distx == 1 && disty == 1)){
                int x = v.x < pp->x ? v.x : pp->x;
                int y = v.y < pp->y ? v.y : pp->y;
                int flag = 0;
                for (int i = 0; i < 2; i++){
                    for(int j = 0; j < 2; j++){
                        struct vertex v1 = {.x = x + i, .y = y + j};
                        if(IsOnVertex(*ep, v1)){
                            flag = 1;
                        }
                    }
                }
                if(!flag){
                    return 0;
                }
            } else {
                return 0;
            }
        }

        return 1;
    }
}

void AI_GenerateMove(char* Board, int size, char* player, struct player* white, struct player* black, int depth, Listptr* History, char* move){
    white->MinScore = PathScore(Board, size, "white", *white, *black, 0, &white->Scores, NULL);
    black->MinScore = PathScore(Board, size, "black", *white, *black, 0, &black->Scores, NULL);

    struct player *pl, *en;
    if(!strcmp(player, "white")){
        pl = white;
        en = black;
    } else {
        pl = black;
        en = white;
    }
    char ver[4] = "";

    //if(pl->MinScore < en->MinScore){
        int max = 1000, x, y, num, tx, ty;
        for (int i = 0; i < 2; i ++){
            for (int j = 0; j < 3; j += 2){
                tx = pl->x + !i * (-1 + j);
                ty = pl->y + i * (-1 + j);
                struct vertex v = {.x = tx, .y = ty};
                VertexToString(v, size, ver);
                if(AI_IsMoveValid(Board, size, player, ver, white, black) == 1){
                    num = *(pl->Scores + ty * size + tx);
                    if(num < max && num >= 0){
                        max = num;
                        x = tx;
                        y = ty;
                    }
                }
            }
        }
        struct vertex v = {.x = x, .y = y};
        if(max >= 700){
            for (int i = 0; i < 2; i ++){
                for (int j = 0; j < 3; j += 2){
                    tx = v.x + !i * (-1 + j);
                    ty = v.y + i * (-1 + j);
                    struct vertex d = {.x = tx, .y = ty};
                    VertexToString(d, size, ver);
                    if(AI_IsMoveValid(Board, size, player, ver, white, black) == 1){
                        num = *(pl->Scores + ty * size + tx);
                        if(num < max && num >= 0){
                            max = num;
                            v.x = tx;
                            v.y = ty;
                        }
                    }
                }
            }
        }
        VertexToString(v, size, ver);
        PlayMove(Board, size, player, ver, white, black, History);

    // } else {

    // }

    strcpy(move, ver);
}