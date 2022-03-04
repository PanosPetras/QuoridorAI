#include "AI.h"
#include "Board.h"
#include "List.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    *(fScore + start.y * size + start.x) = size - 1 - abs(start.y - size);

    struct vertex current;
    char ver[4], ver2[4];
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
                if(t.x >= 0 && t.x < size && t.y >= 0 && t.y < size && *(Board + (2 * size - 1) * (current.y + t.y) + (current.x + t.x)) == ' '){
                    VertexToString(t, size, ver);
                    int tentative_gScore = *(gScore + current.y * size + current.x) + 1;
                    if(tentative_gScore < *(gScore + t.y * size + t.x)){
                        if(flag == 0){
                            VertexToString(t, size, ver2);
                            InsertAtEnd(&CameFrom, ver2);
                            flag = 1;
                        } else {
                            VertexToString(t, size, ver2);
                            Remove(&CameFrom, -1);
                            InsertAtEnd(&CameFrom, ver2);
                        }
                        *(gScore + t.y * size + t.x) = tentative_gScore;
                        *(fScore + t.y * size + t.x) = tentative_gScore + abs(t.y - goal);
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
    int b;
    struct player *pl, *en;
    lowercase(player);
    if(!strcmp(player, "white") || !strcmp(player, "w")){
        pl = white;
        en = black;
        b = 0;
    } else {
        pl = black;
        en = white;
        b = 1;
    }
    char ver[6] = "";

    int max = 1000, x, y, num, tx, ty, res;
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 3; j += 2){
            tx = pl->x + i * (-1 + j);
            ty = pl->y + !i * (-1 + j);
            struct vertex v = {.x = tx, .y = ty};
            VertexToString(v, size, ver);
            res = AI_IsMoveValid(Board, size, player, ver, *white, *black);
            if(res == 1){
                PlayMove(Board, size, player, ver, white, black, History);
                num = AlphaBeta(Board, size, en->name, *white, *black, 0, 0, -10000, 10000);
                Undo(Board, size, white, black, History);
                if(num < max){
                    max = num;
                    x = tx;
                    y = ty;
                }
            } else if(res == -3){
                struct vertex u = {.x = tx + i * (-1 + j), .y = ty + !i * (-1 + j)};
                VertexToString(u, size, ver);
                res = AI_IsMoveValid(Board, size, player, ver, *white, *black);
                if(res == 1){
                    PlayMove(Board, size, player, ver, white, black, History);
                    num = AlphaBeta(Board, size, en->name, *white, *black, 0, 0, -10000, 10000);
                    Undo(Board, size, white, black, History);
                    if(num < max){
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
    
    strcpy(move, ver);
}

int AlphaBeta(char *Board, int size, char *player, struct player white, struct player black, int depth, int IsMaximizer, int a, int b){
    struct player *pl, *en;
    int k;
    if (!strcmp(player, "white")){
        pl = &white;
        en = &black;
        k = 0;
    } else {
        pl = &black;
        en = &white;
        k = 1;
    }
    if(Winner(white, black, size) != NULL){
        if(IsMaximizer){
            if(!strcmp(player, Winner(white, black, size))){
                return -40 + depth;
            } else {
                return 40 + depth;
            }
        } else {
            if(!strcmp(player, Winner(white, black, size))){
                return 40 + depth;
            } else {
                return -40 + depth;
            }
        }
    }

    if (depth >= 6){
        return Astar(Board, size, PlayerToVertex(*en), size * !k - 1 * !k) - Astar(Board, size, PlayerToVertex(*pl), size * k - 1 * k) + depth;
    }

    char ver[4];
    Listptr History = NULL;
    int d = size * 2 - 1;
    int num, value;
    
    if(IsMaximizer){
        value = 1000;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                struct vertex v = {.x = pl->x + i * (-1 + j), .y = pl->y + !i * (-1 + j)};
                VertexToString(v, size, ver);
                if(PlayMove(Board, size, player, ver, &white, &black, &History) == 1){
                    num = AlphaBeta(Board, size, en->name, white, black, depth + 1, 0, a, b);
                    Undo(Board, size, &white, &black, &History);
                    if(value > num){
                        value = num;
                    }
                    if(value >= b){
                        break;
                    }
                    if(a < value){
                        a = value;
                    }
                }
            }
        }
    } else {
        value = -1000;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                struct vertex v = {.x = pl->x + i * (-1 + j), .y = pl->y + !i * (-1 + j)};
                VertexToString(v, size, ver);
                if(PlayMove(Board, size, player, ver, &white, &black, &History) == 1){
                    num = AlphaBeta(Board, size, en->name, white, black, depth + 1, 1, a, b);
                    Undo(Board, size, &white, &black, &History);
                    if(value < num){
                        value = num;
                    }
                    if(value >= a){
                        break;
                    }
                    if(b < value){
                        b = value;
                    }
                }
            }
        }
    }

    return value;
}