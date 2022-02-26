#include "Board.h"
#include "List.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct vertex StringToVertex(char* s, int size){
    lowercase(s);
    struct vertex v = {.x = *s - 'a', .y = size - atoi(s + 1)};
    return v;
}

char* VertexToString(struct vertex v, int size, char* dest){
    sprintf(dest, "%c%d", v.x + 'a', size - v.y);

    return dest;
}

struct vertex PlayerToVertex(struct player p, int size){
    struct vertex v = {.x = p.x, .y = p.y};
    return v;
}

int IsOnVertex(struct player p, struct vertex v){
    if(p.x == v.x && p.y == v.y){
        return 1;
    } else {
        return 0;
    }
}

void lowercase(char* s){
    if(s != NULL){
        for(char i = 0; *(s + i) != '\0'; i++){
            if(*(s + i) >= 'A' && *(s + i) <= 'Z'){
                *(s + i) += 'a' - 'A';
            }
        }
    } else {
        return;
    }
}

void InitPlayers(struct player* white, struct player* black, int size){
    int x = size / 2;
    white->y = size - 1;
    white->x = x;
    white->walls = 0;

    black->y = 0;
    black->x = x;
    black->walls = 0;
}

void Walls(struct player* p1, struct player* p2, int walls){
    p1->walls = walls;
    p2->walls = walls;
}

void InitBoard(int size, char** Board){
    int d = size * 2 - 1;

    if(*Board != NULL){
        free(*Board);
    }

    *Board = malloc(d * d * sizeof(char));

    for(int i = 0; i < d; i++){
        for(int j = 0 ; j < d; j++){
            *(*Board + i * d + j) = ' ';
        }
    }

    int x = size - 0.5;

    *(*Board + x) = 'B';
    *(*Board + d * (d - 1) + x) = 'W';
}

void ShowBoard(int size, char* Board, struct player white, struct player black){
    int d = size * 2 - 1;

    for (int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
            printf("%c", *(Board + i * d + j));
        }
        printf("\n");
    }
    

    for (int i = 0; i < d; i++){
        printf("-");
    }
        printf("\n");
    printf("   ");
    for (int i = 0; i < size; i++){
        printf("%c ", i + 'A');
    }
    printf("\n");

    for(int i = 0; i < d + 2; i++){
        if(i % 2 == 0){
            printf("  ");
            for(int k = 0; k < size; k++){
                if(i > 0 && i < d){
                    if(*(Board + (i - 1) * d + (k) * 2 - 1) == '|' || *(Board + (i - 1) * d + (k) * 2 - 1) == '-'){
                        printf("%c-", *(Board + (i - 1) * d + k * 2 - 1));
                        continue;
                    }
                }
                printf("+-");
            }
            printf("+");
        }else{
            printf("%2d|", size - (i) / 2);
            for (int k = 0; k < size; k++){
                printf("%c|", *(Board + (i - 1) * d + (k * 2)));
            }
            
        }
        printf("\n");
    }
    printf("Walls White: %2d\nWalls Black: %2d", white.walls, black.walls);
}

int PlayMove(char* Board, int size, char* player, char* vertex, struct player* white, struct player* black, Listptr* History){
    struct vertex v = StringToVertex(vertex, size);

    lowercase(player);
    int d = size * 2 - 1;

    struct player* pp;
    struct player* ep;
    char l;

    if(strcmp(player, "white") && strcmp(player, "black")){
        return -2;
    } else {
        if(!strcmp(player, "white")){
            pp = white;
            ep = black;
            l = 'W';
        } else {
            pp = black;
            ep = white;
            l = 'B';
        }
    }
    
    if(Board == NULL){
        return -1;
    } else if((v.x < 0 || v.x > size - 1) || (v.y < 0 || v.y > size - 1)){
        return 0;
    } else if(*(Board + d * (pp->y + v.y) + (pp->x + v.x)) == '-' || *(Board + d * (pp->y + v.y) + (pp->x + v.x)) == '|'){
        return 0;
    } else { 
        if(IsOnVertex(*white, v) || IsOnVertex(*black, v)){
            return 0;
        }
        int distx = abs(pp->x - v.x);
        int disty = abs(pp->y - v.y);
        if(!((distx == 0 && disty == 1) || (distx == 1 && disty == 0))){
            if((distx == 0 && disty == 2) || (distx == 2 && disty == 0)){
                struct vertex v1 = {.x = (pp->x + v.x) / 2, .y = (pp->y + v.y) / 2};
                if(!IsOnVertex(*ep, v1)){
                    return 0;
                }
            } else {
                return 0;
            }
        }

        *(Board + d * (v.y * 2) + (v.x * 2)) = l;
        *(Board + d * (pp->y * 2) + (pp->x * 2)) = ' ';

        char mv[80];
        char str[4];
        VertexToString(PlayerToVertex(*pp, size), size, str);
        sprintf(mv, "m %c %s %s", l, str, vertex);
        InsertAtEnd(History, mv);

        pp->x = v.x;
        pp->y = v.y;

        return 1;
    }
}

int PlayWall(char* Board, int size, char* player, char* vertex, char* alignment, struct player* white, struct player* black, Listptr* History){
    struct vertex v = StringToVertex(vertex, size);

    lowercase(player);
    lowercase(alignment);
    int d = size * 2 - 1;

    printf("PathScore white: %d\nPathScore black: %d", PathScore(Board, size, "white", *white, *black, 0, NULL, NULL), PathScore(Board, size, "black", *white, *black, 0, NULL, NULL));
    if(strcmp(player, "white") && strcmp(player, "black")){
        return -2;
    } else if(strcmp(alignment, "horizontal") && strcmp(alignment, "vertical")){
        return -2;
    } else if(Board == NULL){
        return -1;
    } else if((v.x < 0 || v.x > size - 2) || (v.y < 0 || v.y > size - 2)){
        return 0;
    } else if(PathScore(Board, size, "white", *white, *black, 0, NULL, NULL) == -1 || PathScore(Board, size, "black", *white, *black, 0, NULL, NULL) == -1){
        return 0;
    } else {
        int orientation = !strcmp(alignment, "vertical") ? 1 : 0;
        for(int i = 0; i < 3; i++){
            if(*(Board + d * (v.y * 2 + i * orientation + 1 * !orientation) + v.x * 2 + i * !orientation + 1 * orientation) == '-' ||
            *(Board + d * (v.y * 2 + i * orientation + 1 * !orientation) + v.x * 2 + i * !orientation + 1 * orientation) == '|'){
                return 0;
            }
        }
        char l;
        if(!strcmp(player, "white")){
            if(white->walls <= 0){
                return 0;
            }
            white->walls = white->walls - 1;
            l = 'W';
            
        } else {
            if(black->walls <= 0){
                return 0;
            }
            black->walls = black->walls - 1;
            l = 'B';

        }

        for(int i = 0; i < 3; i++){
            *(Board + d * (v.y * 2 + i * orientation + 1 * !orientation) + v.x * 2 + i * !orientation + 1 * orientation) = orientation ? '|' : '-';
        }

        char mv[80];
        char str[4];
        sprintf(mv, "w %c %s %s", l, vertex, orientation);
        InsertAtEnd(History, mv);

        return 1;
    }
}

int Undo(char* Board, int size, struct player* white, struct player* black, Listptr* History){
    if(*History == NULL){
        return -1;
    }
    int pos = len(*History);
    int d = size * 2 - 1;

    char lastmove[20];
    strcpy(lastmove, get(*History, -1));
    Remove(History, -1);

    char* arg = strtok(lastmove, " ");

    if(!strcmp(arg, "m")){
		char* p = strtok(NULL, " ");
		char* before = strtok(NULL, " ");
		char* now = strtok(NULL, " ");

        struct vertex vbef = StringToVertex(before, size);
        struct vertex vnow = StringToVertex(now, size);
        
        struct player* pp;
        char l;

        if(!strcmp(p, "W")){
            pp = white;
            l = 'W';
        } else {
            pp = black;
            l = 'B';
        }

        pp->x = vbef.x;
        pp->y = vbef.y;

        *(Board + d * (vbef.y * 2) + (vbef.x * 2)) = l;
        *(Board + d * (vnow.y * 2) + (vnow.x * 2)) = ' ';
    } else if(!strcmp(arg, "w")){
		char* p = strtok(NULL, " ");
		char* ver = strtok(NULL, " ");
		char* alignment = strtok(NULL, " ");

        struct vertex v = StringToVertex(ver, size);
        
        struct player* pp;

        if(!strcmp(p, "W")){
            white->walls = white->walls + 1;
        } else {
            black->walls = black->walls + 1;
        }

        int orientation = !strcmp(alignment, "vertical") ? 1 : 0;

        for(int i = 0; i < 3; i++){
            *(Board + d * (v.y * 2 + i * orientation + 1 * !orientation) + v.x * 2 + i * !orientation + 1 * orientation) = ' ';
        }
    }

    return 1;
}

char* Winner(struct player whiteplayer, struct player blackplayer, int size){
    if(whiteplayer.y == 0){
        return "white";
    } else if(blackplayer.y == size - 1){
        return "black";
    } else {
        return NULL;
    }
}