#include "List.h"
#include "AI.h"

struct player{
    int x, y;
    int walls;
    char name[6];
    int MinScore;
    int* Scores;
};

struct vertex{
    int x, y;
};

struct vertex StringToVertex(char*, int);

char* VertexToString(struct vertex, int, char*);

struct vertex PlayerToVertex(struct player);

int IsOnVertex(struct player, struct vertex);

void lowercase(char*);

void InitPlayers(struct player*, struct player*, int);

void InitBoard(int, char**);

void Walls(struct player*, struct player*, int);

void ShowBoard(int, char*, struct player, struct player);

int PlayMove(char*, int, char*, char*, struct player*, struct player*, Listptr*);

int PlayWall(char*, int, char*, char*, char*, struct player*, struct player*, Listptr*);

int Undo(char*, int, struct player*, struct player*, Listptr*);

char* Winner(struct player, struct player, int);