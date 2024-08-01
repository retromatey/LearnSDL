#include "common.h"

void error(const char* message)
{
	fprintf(stderr, "Error: %s :: %s\n", message, SDL_GetError());
	exit(1);
}