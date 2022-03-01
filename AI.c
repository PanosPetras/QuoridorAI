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
                    score[k] = 200;
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

int AI_IsMoveValid(char* Board, int size, char* player, char* vertex, struct player white, struct player black){
    struct vertex v = StringToVertex(vertex, size);

    lowercase(player);
    int d = size * 2 - 1;

    struct player* pp;
    struct player* ep;
    if(!strcmp(player, "white")){
        pp = &white;
        ep = &black;
    } else {
        pp = &black;
        ep = &white;
    }
    
    if(Board == NULL){
        return -1;
    } else if((v.x < 0 || v.x > size - 1) || (v.y < 0 || v.y > size - 1)){
        return 0;
    } else if(*(Board + d * (pp->y + v.y) + (pp->x + v.x)) == '-' || *(Board + d * (pp->y + v.y) + (pp->x + v.x)) == '|'){
        return 0;
    } else {
        if(IsOnVertex(white, v) || IsOnVertex(black, v)){
            return -3;
        }
        int distx = abs(pp->x - v.x);
        int disty = abs(pp->y - v.y);
        if((distx == 0 && disty == 2) || (distx == 2 && disty == 0)){
            struct vertex v1 = {.x = (pp->x + v.x) / 2, .y = (pp->y + v.y) / 2};
            if(!IsOnVertex(*ep, v1)){
                return 0;
            }
        }

        return 1;
    }
}

int AI_IsValidWall(char *Board, int size, char *player, char *vertex, char *alignment, struct player white, struct player black){
    struct vertex v = StringToVertex(vertex, size);

    int d = size * 2 - 1;

    if (strcmp(player, "white") && strcmp(player, "black")) {
        return -2;
    } else if (strcmp(alignment, "horizontal") && strcmp(alignment, "vertical")) {
        return -2;
    } else if (Board == NULL) {
        return -1;
    } else if ((v.x < 0 || v.x > size - 2) || (v.y < 0 || v.y > size - 2)) {
        return 0;
    } else {
        int orientation = !strcmp(alignment, "vertical") ? 1 : 0;
        for (int i = 0; i < 3; i++){
            if (*(Board + d * (v.y * 2 + i * orientation + 1 * !orientation) + v.x * 2 + i * !orientation + 1 * orientation) == '-' ||
                *(Board + d * (v.y * 2 + i * orientation + 1 * !orientation) + v.x * 2 + i * !orientation + 1 * orientation) == '|') {
                return 0;
            }
        }
        char l;
        if (!strcmp(player, "white")) {
            if (white.walls <= 0) {
                return 0;
            }
        } else {
            if (black.walls <= 0) {
                return 0;
            }
        }

        return 1;
    }
}

void AI_GenerateMove(char* Board, int size, char* player, struct player* white, struct player* black, int depth, Listptr* History, char* move){
    struct player *pl, *en;
    if(!strcmp(player, "white")){
        pl = white;
        en = black;
    } else {
        pl = black;
        en = white;
    }
    char ver[4] = "";
        
    for (int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            printf("%d|", *(pl->Scores + i * size + j));
        }
        printf("\n");
    }

    //if(pl->MinScore < en->MinScore){
        int max = 1000, x, y, num, tx, ty, res;
        for (int i = 0; i < 2; i ++){
            for (int j = 0; j < 3; j += 2){
                tx = pl->x + !i * (-1 + j);
                ty = pl->y + i * (-1 + j);
                struct vertex v = {.x = tx, .y = ty};
                VertexToString(v, size, ver);
                res = AI_IsMoveValid(Board, size, player, ver, *white, *black);
                if(res == 1){
                    num = *(pl->Scores + ty * size + tx);
                    if(num < max && num >= 0){
                        max = num;
                        x = tx;
                        y = ty;
                    }
                } else if(res == -3){
                    struct vertex u = {.x = tx + !i * (-1 + j), .y = ty + i * (-1 + j)};
                    VertexToString(u, size, ver);
                    res = AI_IsMoveValid(Board, size, player, ver, *white, *black);
                    if(res == 1){
                        num = *(pl->Scores + u.y * size + u.x);
                        if(num < max && num >= 0){
                            max = num;
                            x = u.x;
                            y = u.y;
                        }
                    }
                }
            }
        }
        struct vertex v = {.x = x, .y = y};
        VertexToString(v, size, ver);
        PlayMove(Board, size, player, ver, white, black, History);

    // } else {

    // }

    strcpy(move, ver);
}

int Minimax(char *Board, int size, char *player, struct player white, struct player black, int depth, int IsMinimizer){
    struct player *pl, *en;
    if (!strcmp(player, white.name)){
        pl = &white;
        en = &black;
    } else {
        pl = &black;
        en = &white;
    }

    if (depth >= 5){
        white.MinScore = PathScore(Board, size, white.name, white, black, 0, &white.Scores, NULL);
        black.MinScore = PathScore(Board, size, black.name, white, black, 0, &black.Scores, NULL);

        int res = en->MinScore - pl->MinScore;

        free(white.Scores);
        free(black.Scores);

        return en->MinScore - pl->MinScore;
    }
    if(Winner(white, black, size) != NULL){
        if(!strcmp(player, Winner(white, black, size))){
            return -400 * (-1 + IsMinimizer * 2);
        } else {
            return -400 * (-1 + !IsMinimizer * 2);
        }
    }

    char ver[4];
    Listptr History = NULL;
    int d = size * 2 - 1;

    int max = IsMinimizer ? 1000 : -1000, num, res;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j += 2) {
            struct vertex v = {.x = pl->x + !i * (-1 + j), .y = pl->y + i * (-1 + j)};
            VertexToString(v, size, ver);
            if (AI_IsMoveValid(Board, size, player, ver, white, black) == 1){
                PlayMove(Board, size, player, ver, &white, &black, &History);
                num = Minimax(Board, size, en->name, white, black, depth + 1, !IsMinimizer);
                Undo(Board, size, &white, &black, &History);
                if (num > max && !IsMinimizer){
                    max = num;
                } else if (num < max && IsMinimizer){
                    max = num;
                }
            }
        }
    }
    char alignments[2][11] = {"horizontal", "vertical"};

    for (int i = 0; i < size - 1; i++){
        for (int j = 0; j < size - 1; j++){
            for(int k = 0; k < 2; k++){
                struct vertex v = {.x = i, .y = j};
                VertexToString(v, size, ver);
                if(AI_IsValidWall(Board, size, player, ver, alignments[k], white, black) == 1){
                    PlayWall(Board, size, player, ver, alignments[k], &white, &black, &History);
                    num = Minimax(Board, size, en->name, white, black, depth + 1, !IsMinimizer);
                    Undo(Board, size, &white, &black, &History);
                    if (num > max && !IsMinimizer){
                        max = num;
                    } else if (num < max && IsMinimizer){
                        max = num;
                    }
                }
            }
        }
    }

    return max;
}

/*Problem: won't jump over player when there are no other options*/