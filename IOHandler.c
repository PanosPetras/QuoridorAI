#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IOHandler.h"

int isInteger(char* s){
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (s[i] < '0' || s[i] > '9')
              return 0;
    }
    return 1;
}

void ProcessInput(char* Input){
	for(int i = 0; Input[i] != '\0'; i++){
		if(Input[i] <= 31 || Input[i] == 127){
			if(Input[i] != 9 || Input[i] != 10){
				Input[i] = ' ';
			}
		}
	}
}

int HandleCommand(char* Input, char** Board, int* size, struct player* white, struct player* black, Listptr* History){
	char KnownCommands[13][20] = {  "name", "known_command", "list_commands",
									"quit", "boardsize", "clear_board", "walls",
									"playmove", "playwall", "genmove", "undo",
									"winner", "showboard"};

	strtok(Input, "#");
	strtok(Input, "\n\t");

	char* token;
	token = strtok(Input, " ");

	if(!strcmp(Input, "name")){
		printf("= Panzerkampfwagen VI Tiger Ausführung B\n\n");
	} else if(!strcmp(Input, "known_command")) {
		token = strtok(NULL, " ");
		if(token != NULL){
			for(int i = 0; i < 11; i++){
				if(!strcmp(token, KnownCommands[i])){
					printf("= true\n\n");
					return 0;
				}
			}
		}
		printf("= false\n\n");

	} else if(!strcmp(Input, "list_commands")) {
		printf("= ");

		for(int i = 0; i < 13; i++){
			printf("%s\n", KnownCommands[i]);
		}

		printf("\n");

	} else if(!strcmp(Input, "quit")) {
		printf("=\n\n");
		return 1;

	} else if(!strcmp(Input, "boardsize")) {
		token = strtok(NULL, " ");
		if(token != NULL){
			if(isInteger(token)){
				*size = atoi(token);
				if(*size % 2 && *size <= 25 && *size >= 3){
					InitBoard(*size, Board);
					InitPlayers(white, black, *size);
					FreeList(History);
					white->MinScore = PathScore(*Board, *size, "white", *white, *black, 0, &white->Scores, NULL);
					black->MinScore = PathScore(*Board, *size, "black", *white, *black, 0, &black->Scores, NULL);
					printf("=\n\n");
				} else {
					printf("? invalid size\n\n");
				}
			} else {
				printf("? invalid argument\n\n");
			}
		} else {
			printf("? invalid syntax\n\n");
		}

	} else if(!strcmp(Input, "clear_board")) {
		InitBoard(*size, Board);
		InitPlayers(white, black, *size);
		FreeList(History);
		white->MinScore = PathScore(*Board, *size, "white", *white, *black, 0, &white->Scores, NULL);
		black->MinScore = PathScore(*Board, *size, "black", *white, *black, 0, &black->Scores, NULL);
		printf("=\n\n");

	} else if(!strcmp(Input, "walls")) {
		token = strtok(NULL, " ");
		if(token != NULL){
			if (isInteger(token)) {
				int walls = atoi(token);
				Walls(white, black, walls);
				printf("=\n\n");
			} else {
				printf("? invalid argument\n\n");
			}
		} else {
			printf("? invalid syntax \n\n");
		}

	} else if(!strcmp(Input, "playmove")) {
		char* p = strtok(NULL, " ");
		char* move = strtok(NULL, " ");
		if(p != NULL && move != NULL){
			int status = PlayMove(*Board, *size, p, move, white, black, History);
			if(status == 1){
				white->MinScore = PathScore(*Board, *size, "white", *white, *black, 0, &white->Scores, NULL);
				black->MinScore = PathScore(*Board, *size, "black", *white, *black, 0, &black->Scores, NULL);
				printf("=\n\n");
			} else if(status == -2){
				printf("? invalid syntax \n\n");
			} else if(status == -1){
				printf("? empty board\n\n");
			} else if(status == 0 || status == -3){
				printf("? illegal move\n\n");
			}
		} else {
			printf("? invalid syntax \n\n");
		}

	} else if(!strcmp(Input, "playwall")) {
		char* p = strtok(NULL, " ");
		char* move = strtok(NULL, " ");
		char* alignment = strtok(NULL, " ");
		if(p != NULL && move != NULL && alignment != NULL){
			int status = PlayWall(*Board, *size, p, move, alignment, white, black, History);
			if(status == 1){
				white->MinScore = PathScore(*Board, *size, "white", *white, *black, 0, &white->Scores, NULL);
				black->MinScore = PathScore(*Board, *size, "black", *white, *black, 0, &black->Scores, NULL);
				printf("=\n\n");
			} else if(status == -2){
				printf("? invalid syntax \n\n");
			} else if(status == -1){
				printf("? empty board\n\n");
			} else if(status == 0){
				printf("? illegal move\n\n");
			}
		} else {
			printf("? invalid syntax \n\n");
		}

	} else if(!strcmp(Input, "genmove")) {
		token = strtok(NULL, " ");
		if(*Board == NULL){
			printf("? empty board\n\n");
		} else if(token != NULL){
			if(!strcmp(token, "white") || !strcmp(token, "black")){
			 	char res[6] = "";
			 	AI_GenerateMove(*Board, *size, token, white, black, 0, History, res);
			 	printf("= %s\n\n", res);
			} else {
			 	printf("? invalid syntax\n\n");
			}
		} else {
			printf("? invalid syntax\n\n");
		}

	} else if(!strcmp(Input, "undo")) {
		char* p = strtok(NULL, " ");
		int moves;
		if(p == NULL){
			moves = 1;
		} else if (isInteger(p)){
			moves = atoi(p);
		} else {
			printf("? invalid syntax\n\n");
			return 0;
		}

		if(moves > len(*History)){
			printf("? cannot undo\n\n");
		} else {
			for(int i = 0; i < moves; i++){
				Undo(*Board, *size, white, black, History);
			}
			printf("=\n\n");
		}

	} else if(!strcmp(Input, "winner")) {
		char* res = Winner(*white, *black, *size);
		if(res != NULL){
			printf("= true %s\n\n", res);
		} else {
			printf("= false\n\n");
		}
		
	} else if(!strcmp(Input, "showboard")) {
		if(*Board != NULL){
			printf("=\n");
			ShowBoard(*size, *Board, *white, *black);
			printf("\n\n");
		} else {
			printf("? empty board\n\n");
		}

	} else {
		printf("? unknown command\n\n");
	}

	return 0;
}