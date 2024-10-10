#include "common.h"
#include "init.h"
#include "draw.h"
#include "input.h"
#include "player.h"

#define SDL_MAIN_HANDLED

static void frame_delay(long *then, float *remainder);

App app;
Player player;

int main(int argc, char* argv[])
{
	float remainder;
	long then;

	memset(&app, 0, sizeof(App));

	init_SDL();
	init_player();
	atexit(destroy);

	then = SDL_GetTicks();
	remainder = 0;

	while (1)
	{
		prepare_scene();
		handle_input();
		app.delegate.update();
		app.delegate.draw();
		render_scene();
		frame_delay(&then, &remainder);
	}

	return 0;
}

static void frame_delay(long *then, float *remainder)
{
	long frameTime = SDL_GetTicks() - *then;
	long wait = 16 + *remainder;
	wait -= frameTime;

	if (wait < 1)
		wait = 1;

	SDL_Delay(wait);

	*remainder -= (int)*remainder;
	*remainder += 0.667;
	*then = SDL_GetTicks();
}