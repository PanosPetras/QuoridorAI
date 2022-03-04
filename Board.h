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

//Μετατράπει ένα string συντεταγμένων(π.χ Α4) σε struct vertex
struct vertex StringToVertex(char*, int);

//Μετατρέπει ένα struct vetex σε string συντεταγμένων
char* VertexToString(struct vertex, int, char*);

//Δημιουργεί ένα struct vertex απο τις συντεταγμένες του παίκτη
struct vertex PlayerToVertex(struct player);

//Ελέγχει εαν ένας παίκτης βρίσκεται πάνω σε ένα vertex
int IsOnVertex(struct player, struct vertex);

//Μετάτρέπει ένα string σε lowercase charcters
void lowercase(char*);

//Αρχικοποιεί τα structs των παικτών
void InitPlayers(struct player*, struct player*, int);

//Αρχικοποιεί τον πίνακα του παιχνιδιού στη διάσταση size * size
void InitBoard(int, char**);

//Δίνει τοίχους στους παίκτες
void Walls(struct player*, struct player*, int);

//Εμφανίζει τον Πίνακα του παιχνιδιού
void ShowBoard(int, char*, struct player, struct player);

//Ουσιαστικά κάνω implement τις εντολές του πρωτοκόλλου με το αντίστοιχο όνομα
int PlayMove(char*, int, char*, char*, struct player*, struct player*, Listptr*);

int PlayWall(char*, int, char*, char*, char*, struct player*, struct player*, Listptr*);

int Undo(char*, int, struct player*, struct player*, Listptr*);

char* Winner(struct player, struct player, int);