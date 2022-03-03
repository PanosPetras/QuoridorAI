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
        if(*Scores != NULL){
            free(*Scores);
            *Scores = NULL;
        }
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

        int score[4], status, min = 1000, num;
        char ver[4];
        struct player temp;
        struct vertex v = PlayerToVertex(*p), t;
        VertexToString(v, size, ver);
        for (int i = 0; i < 2; i ++){
            for (int j = 0; j < 3; j += 2){
                t.x = v.x + i * (-1 + j);
                t.y = v.y + !i * (-1 + j);
                VertexToString(t, size, ver);
                status = PlayMove(Board, size, player, ver, &white, &black, &History);
                
                if(status == 1){
                    num = PathScore(Board, size, player, white, black, depth + 1, Scores, History);

                    Undo(Board, size, &white, &black, &History);
                } else if(status == -3){
                    num = 200;
                } else {
                    num = -1;
                }
                if(num < min && num >= 0){
                    min = num;
                }
            }
        }

        if(min != 1000){
            *(*Scores + p->y * size + p->x) = min + 1;
        } else {
            *(*Scores + p->y * size + p->x) = -1;
        }

    } else if(*(*Scores + p->y * size + p->x) == -20){
        return -1;
    }
    
    return *(*Scores + p->y * size + p->x);
}

int Astar(char* Board, int size, struct vertex start, int goal){

    Listptr openSet = NULL;
    InitList(&openSet, "");
    VertexToString(start, size, openSet->data);

    Listptr CameFrom = NULL;

    int* gScore = malloc(size * size * sizeof(int));
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            *(gScore + j * size + i) = 10000;
        }
    }
    *(gScore + start.y * size + start.x) = 0;

    int* fScore = malloc(size * size * sizeof(int));
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            *(fScore + j * size + i) = 10000;
        }
    }
    *(fScore + start.y * size + start.x) = size - 1;

    struct vertex current;
    char ver[4];
    while (openSet != NULL){
        current = StringToVertex(openSet->data, size);
        int min = *(fScore + current.y * size + current.x);
        Listptr it = openSet;
        while(it->next != NULL){
            struct vertex v = StringToVertex(it->data, size);
            if(*(fScore + v.y * size + v.x) < min){
                min = *(fScore + v.y * size + v.x);
                current = v;
            }
            it = it->next;
        }
        
        if(current.y == goal){
            int res = len(CameFrom);
            struct vertex v1, v2;
            for(int i = 1; i < len(CameFrom); i++){
                v1 = StringToVertex(get(CameFrom, i), size);
                v2 = StringToVertex(get(CameFrom, i - 1), size);
                if(!((abs(v1.x - v2.x) == 1 && abs(v1.y - v2.y) == 0) || (abs(v1.x - v2.x) == 0 && abs(v1.y - v2.y) == 1))){
                    Remove(&CameFrom, i);
                }
            }
            FreeList(&openSet);
            FreeList(&CameFrom);
            free(gScore);
            free(fScore);
            return res;
        }

        VertexToString(current, size, ver);
        int index = find(openSet, ver), flag = 0;
        Remove(&openSet, index);

        for (int i = 0; i < 2; i ++){
            for (int j = 0; j < 3; j += 2){
                struct vertex t = {.x = current.x + i * (-1 + j), .y = current.y + !i * (-1 + j)};
                if(t.x >= 0 && t.x < size && t.y >= 0 && t.y < size){
                    VertexToString(t, size, ver);
                    int tentative_gScore = *(gScore + current.y * size + current.x) + t.x + abs(t.y - goal);
                    if(tentative_gScore < *(gScore + t.y * size + t.x)){
                        if(flag == 0){
                            InsertAtEnd(&CameFrom, ver);
                            flag = 1;
                        } else {
                            Remove(&CameFrom, -1);
                            InsertAtEnd(&CameFrom, ver);
                        }
                        *(gScore + t.y * size + t.x) = tentative_gScore;
                        *(fScore + t.y * size + t.x) = tentative_gScore + (size - 1 - abs(t.y - goal));
                        if(find(openSet, ver) == -1){
                            InsertAtEnd(&openSet, ver);
                        } 
                    }
                }
            }
        }
    }
    
    FreeList(&openSet);
    FreeList(&CameFrom);
    free(gScore);
    free(fScore);
    return -1;
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
        if(!((distx == 0 && disty == 1) || (distx == 1 && disty == 0))){
            if((distx == 0 && disty == 2) || (distx == 2 && disty == 0)){
                struct vertex v1 = {.x = (pp->x + v.x) / 2, .y = (pp->y + v.y) / 2};
                if(!IsOnVertex(*ep, v1)){
                    return 0;
                }
                if(*(Board + d * (ep->y + v.y) + (ep->x + v.x)) == '-' || *(Board + d * (ep->y + v.y) + (ep->x + v.x)) == '|'){
                    return 0;
                }
            } else {
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
    //white->MinScore = PathScore(Board, size, white->name, *white, *black, 0, &white->Scores, NULL);
    //black->MinScore = PathScore(Board, size, black->name, *white, *black, 0, &black->Scores, NULL);
    int b;
    struct player *pl, *en;
    if(!strcmp(player, "white")){
        pl = white;
        en = black;
        b = 0;
    } else {
        pl = black;
        en = white;
        b = 1;
    }
    char ver[4] = "";
        
    // for (int i = 0; i < size; i++){
    //     for(int j = 0; j < size; j++){
    //         printf("%d|", *(pl->Scores + i * size + j));
    //     }
    //     printf("\n");
    // }

    //if(pl->MinScore < en->MinScore){
        int max = 1000, x, y, num, tx, ty, res;
        for (int i = 0; i < 2; i ++){
            for (int j = 0; j < 3; j += 2){
                tx = pl->x + i * (-1 + j);
                ty = pl->y + !i * (-1 + j);
                struct vertex v = {.x = tx, .y = ty};
                VertexToString(v, size, ver);
                res = AI_IsMoveValid(Board, size, player, ver, *white, *black);
                if(res == 1){
                    //num = *(pl->Scores + ty * size + tx);
                    num = Astar(Board, size, v, size * b - 1 * b);
                    printf("x %d y %d num %d\n", v.x, v.y , num);
                    if(num < max && num >= 0){
                        max = num;
                        x = tx;
                        y = ty;
                    }
                } else if(res == -3){
                    struct vertex u = {.x = tx + i * (-1 + j), .y = ty + !i * (-1 + j)};
                    VertexToString(u, size, ver);
                    res = AI_IsMoveValid(Board, size, player, ver, *white, *black);
                    if(res == 1){
                        num = Astar(Board, size, u, size * b - 1 * b);
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

        return res;
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

    int max = IsMinimizer ? -1000 : 1000, num, res;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j += 2) {
            struct vertex v = {.x = pl->x + i * (-1 + j), .y = pl->y + !i * (-1 + j)};
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