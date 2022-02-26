#include "List.h"

struct player;

int PathScore(char*, int, char*, struct player, struct player, int, int*, Listptr);

char* GenerateMove(char*, int, char*, struct player, struct player);

char* Minimax(char*, int);