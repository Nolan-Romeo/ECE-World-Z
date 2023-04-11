#include "std_iso.h"

void collapsePath(int x, int y, int x1, int y1, Tile map[12][12], Maillon** path){ // fonction récursive pour remonter le chemin à l'envers jusqu'au départ
	//printf("collapse");

  LSCAddStart(path, x, y);

	if(x == x1 && y == y1) return;
	switch (map[y][x].direction)
	{
	case 1:
		y -= 1;
		map[y][x].type = 4;
		break;
	case 2:
		x += 1;
		map[y][x].type = 4;
		break;
	case 3:
		y += 1;
		map[y][x].type = 4;
		break;
	case 4:
		x -= 1;
		map[y][x].type = 4;
		break;
	default:
		fprintf(stderr, "erreur, direction incorrecte pour le collapse \n");
		return;
		break;
	}
	collapsePath(x, y, x1, y1, map, path);
}

bool expandTile(int x, int y, int x1, int y1, int x2, int y2, Tile map[12][12], Maillon** path){
	if(y > 0 && map[y-1][x].type != 3){
        map[y-1][x].open = true;
		if(map[y-1][x].Gcoast > map[y][x].Gcoast || map[y-1][x].Gcoast == -1){
			map[y-1][x].Gcoast = map[y][x].Gcoast+1;
			map[y-1][x].direction = 3;
		}
        map[y-1][x].Hcoast = abs(x-x2)+abs(y-1-y2);
        map[y-1][x].Fcoast = map[y-1][x].Gcoast + map[y-1][x].Hcoast;
		if(map[y-1][x].type == 2) {
			collapsePath(x, y-1, x1, y1, map, path);
			return true;
		}
    }
	if(x < 11 && map[y][x+1].type != 3){
        map[y][x+1].open = true;
		if(map[y][x+1].Gcoast > map[y][x].Gcoast || map[y][x+1].Gcoast == -1){
			map[y][x+1].Gcoast = map[y][x].Gcoast+1;
			map[y][x+1].direction = 4;
		}
        map[y][x+1].Hcoast = abs(x+1-x2)+abs(y-y2);
        map[y][x+1].Fcoast = map[y][x+1].Gcoast + map[y][x+1].Hcoast;
		if(map[y][x+1].type == 2) {
			collapsePath(x+1, y, x1, y1, map, path);
			return true;
		}
    }
    if(y < 11 && map[y+1][x].type != 3){
        map[y+1][x].open = true;
		if(map[y+1][x].Gcoast > map[y][x].Gcoast || map[y+1][x].Gcoast == -1){
			map[y+1][x].Gcoast = map[y][x].Gcoast+1;
			map[y+1][x].direction = 1;
		}
        map[y+1][x].Hcoast = abs(x-x2)+abs(y+1-y2);
        map[y+1][x].Fcoast = map[y+1][x].Gcoast + map[y+1][x].Hcoast;
		if(map[y+1][x].type == 2) {
			collapsePath(x, y+1, x1, y1, map, path);
			return true;
		}
    }
    if(x > 0 && map[y][x-1].type != 3){
        map[y][x-1].open = true;
		if(map[y][x-1].Gcoast > map[y][x].Gcoast || map[y][x-1].Gcoast == -1){
			map[y][x-1].Gcoast = map[y][x].Gcoast+1;
			map[y][x-1].direction = 2;
		}
        map[y][x-1].Hcoast = abs(x-1-x2)+abs(y-y2);
        map[y][x-1].Fcoast = map[y][x-1].Gcoast + map[y][x-1].Hcoast;
		if(map[y][x-1].type == 2) {
			collapsePath(x-1, y, x1, y1, map, path);
			return true;
		}
    }

    map[y][x].treated = true;
	return false;
}

bool findPath(int x1, int y1, int x2, int y2, Tile map[NB_ETAGE][PLAT_SIZE][PLAT_SIZE], Maillon** path){

	if(x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || x1 >= 12 || y1 >= 12 || x2 >= 12 || y2 >= 12) return false;

	printf("findPath");

    TileToExpand tileToExpand;
    int iterations = 0;

	Tile map2D[PLAT_SIZE][PLAT_SIZE];

	for(int y = 0; y < PLAT_SIZE; y++){
		for(int x = 0; x < PLAT_SIZE; x++){
			if(map[1][y][x].type == 0) map2D[y][x].type = 0;
			else map2D[y][x].type = 3;
			map2D[y][x].Gcoast = -1;
			map2D[y][x].Hcoast = -1;
			map2D[y][x].Fcoast = -1;
			map2D[y][x].treated = false;
			map2D[y][x].open = false;
		}
	}

	map2D[y1][x1].type = 1; //depart
	map2D[y1][x1].open = true;
	map2D[y1][x1].Gcoast = 0;
	map2D[y1][x1].Hcoast = abs(x1-x2)+abs(y1-y2);
	map2D[y1][x1].Fcoast = map2D[y1][x1].Gcoast + map2D[y1][x1].Hcoast;
	map2D[y2][x2].type = 2; //arrivée

    while(true){
        tileToExpand.Fcoast = 1000;
		tileToExpand.Hcoast = 1000;
        tileToExpand.x = -1;
        tileToExpand.y = -1;
        for(int y = 0; y < 12; y++){
            for(int x = 0; x < 12; x++){
                if(map2D[y][x].open){
                    if((((map2D[y][x].Fcoast != -1) && (map2D[y][x].Fcoast < tileToExpand.Fcoast)) || ((map2D[y][x].Fcoast == tileToExpand.Fcoast) && (map2D[y][x].Hcoast < tileToExpand.Hcoast))) && (map2D[y][x].treated == false)){
                        tileToExpand.Fcoast = map2D[y][x].Fcoast;
                        tileToExpand.Hcoast = map2D[y][x].Hcoast;;
                        tileToExpand.x = x;
                        tileToExpand.y = y;
                    }
                }
            }
        }
        //printf("%d, %d\n", tileToExpand.x, tileToExpand.y);
        if(tileToExpand.x != -1 && tileToExpand.y != -1){
            if(expandTile(tileToExpand.x, tileToExpand.y, x1, y1, x2, y2, map2D, path)) return true;
        }
        if(iterations >= MAX_ASTAR_ITERATIONS){
			printf("ERROR: chemin impossible de (%d, %d) à (%d,%d)\n", x1, y1, x2, y2);
			return false;
		}
        iterations++;
    }
		return false;
}