#include "List.h"

struct player;

int PathScore(char*, int, char*, struct player, struct player, int, int**, Listptr);

void AI_GenerateMove(char*, int, char*, struct player*, struct player*, int, Listptr*, char*);

char* Minimax(char*, int, char*, struct player, struct player, int);