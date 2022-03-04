#include "Board.h"

//Αφαιρώ τα unwanted characters απο το input
void ProcessInput(char*);

//Εδώ διαχειρίζομαι τις εντολές του input
int HandleCommand(char*, char**, int*, struct player*, struct player*, Listptr*);