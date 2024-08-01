#include "common.h"
#include "game.h"

int main(int argc, char* argv[])
{	
	game_init();
	atexit(game_destroy);
	game_start();

	return 0;
}