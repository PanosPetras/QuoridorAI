struct player;

int PathScore(char*, int, char*, struct player, struct player, int, int*);

char* GenerateMove(char*, int, char*, struct player, struct player);

char* Minimax(char*, int);