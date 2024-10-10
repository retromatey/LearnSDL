#include "common.h"

void error(const char* msg)
{
	fprintf(stderr, "Error: %s :: %s\n", msg, SDL_GetError());
	exit(1);
}