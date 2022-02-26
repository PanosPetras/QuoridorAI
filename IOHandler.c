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
									"playmove", "playwall", "genmove", /*"undo",*/
									"winner", "showboard"};

	strtok(Input, "#");
	strtok(Input, "\n\t");

	char* token;
	token = strtok(Input, " ");

	if(!strcmp(Input, "name")){
		printf("= Panzerkampfwagen Tiger Ausf. B\nType:			Heavy tank\nPlace of origin:	Nazi Germany\nService history\nIn service:		1944–45\nWars: 			World War II\nProduction history\nDesigner:		Henschel & Son / Krupp (turret)\nDesigned:		1943\nManufacturer:		Henschel & Son / Krupp (turret)\nUnit cost:		321,500 Reichsmark ($160,750 USD) in 1944–45\nProduced:		1944–45\nNo. built:		489\nSpecifications\nMass:			68.5 tonnes (67.4 long tons; 75.5 short tons) early turret\n			69.8 tonnes (68.7 long tons; 76.9 short tons) production turret\nLength:			7.38 m (24 ft 3 in) hull\n			10.286 m (33 ft 9.0 in) with gun forward)\nWidth:			3.755 m (12 ft 3.8 in)\nHeight:			3.09 m (10 ft 2 in)\nCrew:			5 (commander, gunner, loader, radio operator, driver)\nArmour:			25–185 mm (0.98–7.28 in)\nMain armament: 		1× 8.8 cm Kampfwagenkanone 43\nEarly turret: 		80 rounds\nProduction turret: 	86 rounds\nSecondary armament: 	2× 7.92 mm MG 34 machine guns\n			5,850 rounds\nEngine:			V-12 Maybach HL 230 P30 gasoline\n			700 PS (690 hp, 515 kW)\nPower/weight:		10 PS (7.5 kW) /tonne (8.97 hp/tonne)\nTransmission:		Maybach OLVAR OG 40 12 16 B (8 forward and 4 reverse)\nSuspension: 		Torsion bar\nGround clearance: 	495 to 510 mm (19.5 to 20.1 in)\nFuel capacity: 		860 litres (190 imp gal)\nOperational range: 	Road: 190 km (120 mi), Cross country: 120 km (75 mi)\nMaximum speed, 		road: 41.5 km/h (25.8 mph)\nSustained speed, 	road: 38 km/h (24 mph)\nCross country: 		15 to 20 km/h (9.3 to 12.4 mph)\nThe Tiger II is a German heavy tank of the Second World War. The final official German designation was Panzerkampfwagen Tiger Ausf. B, often shortened to Tiger B. The ordnance inventory designation was Sd.Kfz. 182. (Sd.Kfz. 267 and 268 for command vehicles). It was known as King Tiger by Allied soldiers, and is also known under the informal name Königstiger (the German name for the Bengal tiger which translates literally as Royal Tiger). The name Königstiger was never used in contemporary German documentation, but was used extensively after the war.The Tiger II was the successor to the Tiger I, combining the latter\'s thick armour with the armour sloping used on the Panther medium tank. The tank weighed almost 70 tonnes, and was protected by 100 to 185 mm (3.9 to 7.3 in) of armour to the front. It was armed with the long barrelled 8.8 cm KwK 43 L/71 anti-tank cannon. The chassis was also the basis for the Jagdtiger turretless Jagdpanzer anti-tank vehicle.		The Tiger II was issued to heavy tank battalions of the Army and the Waffen-SS. It was first used in combat by 503rd Heavy Panzer Battalion during the Allied invasion of Normandy on 11 July 1944; on the Eastern Front, the first unit to be outfitted with the Tiger II was the 501st Heavy Panzer Battalion, which by 1 September 1944 listed 25 Tiger IIs operational.\n\n");
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

	} else if(!strcmp(Input, "playwall")) {
		char* p = strtok(NULL, " ");
		char* move = strtok(NULL, " ");
		char* alignment = strtok(NULL, " ");
		if(p != NULL && move != NULL && alignment != NULL){
			int status = PlayWall(*Board, *size, p, move, alignment, white, black, History);
			if(status == 1){
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
		char* p = strtok(NULL, " ");
		lowercase(p);
		if(p != NULL && (!strcmp(p, "white") || !strcmp(p, "black"))){
			printf("= %s\n\n", GenerateMove(*Board, *size, p, *white, *black));
		} else {
			printf("? invalid syntax\n\n");
		}

	} else if(!strcmp(Input, "undo")) {
		char* p = strtok(NULL, " ");
		int moves;
		if(p == NULL){
			moves = 1;
		}
		if(isInteger(p)){
			moves = atoi(p);
			if(moves > len(*History)){
				printf("? cannot undo\n\n");
			} else {
				for(int i = 0; i < moves; i++){
					Undo(*Board, *size, white, black, History);
				}
				printf("=\n\n");
			}
		} else {
			printf("? invalid syntax\n\n");
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