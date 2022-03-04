#include "List.h"
//Μέγιστο βάθος της συνάρτησης Alpha-Beta
#define DEPTH 6

//Τα κάνω declare εδώ γιατί χρειάζονται για τα πρωτότυπα των συναρτήσεων
struct player;
struct vertex;

/*Path finding algorithm για να βρίσκει την απόσταση του παίκτη απο τον προορισμό του.
 *Έχει κάποιες ατέλειες και σε λίγες περιπτώσεις μπορεί να κολλήσει κάπου
 *Για να είμαι ειλικρηνής, δεν είμαι σίγουρος για το πως ακριβώς δουλεύει
 *Είναι προσαρμογή του ψευδοκώδικα απο την Wikipedia. 
 *Link: https://en.wikipedia.org/wiki/A*_search_algorithm*/
int Astar(char*, int, struct vertex, int);

//Ουσιαστικά αξολογεί κάθε σενάριο μέσω της Alpha Beta και επιστρέφει το καλύτερο
void AI_GenerateMove(char*, int, char*, struct player*, struct player*, int, Listptr*, char*);

/*Αναλύει το παιχνίδι σε Βάθος DEPTH και αξιολογεί κάθε σενάριο
 *Για την δημιουργία της συνάρτησης συμβουλεύτηκα και πάλι
 *τον ψευδοκώδικα απο την Wikipedia*/
int AlphaBeta(char *, int, char *, struct player, struct player, int, int, int, int);