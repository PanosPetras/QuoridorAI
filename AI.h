#include "List.h"

struct player;

void AI_GenerateMove(char*, int, char*, struct player*, struct player*, int, Listptr*, char*);

int AI_IsMoveValid(char*, int, char*, char*, struct player, struct player);

int Minimax(char *, int, char *, struct player, struct player, int, int);