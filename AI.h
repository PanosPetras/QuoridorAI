#include "List.h"

struct player;
struct vertex;

int Astar(char*, int, struct vertex, int);

void AI_GenerateMove(char*, int, char*, struct player*, struct player*, int, Listptr*, char*);

int AI_IsMoveValid(char*, int, char*, char*, struct player, struct player);

int AlphaBeta(char *, int, char *, struct player, struct player, int, int, int, int);