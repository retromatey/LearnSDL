#include "common.h"
#include "input.h"

extern App app;

static void on_key_down(SDL_KeyboardEvent* e)
{
	if (e->repeat == 0 && e->keysym.scancode < KEYBOARD_INPUT && e->keysym.scancode >= 0)
	{
		app.keyboard[e->keysym.scancode] = true;
	}
}

static void on_key_up(SDL_KeyboardEvent* e)
{
	if (e->repeat == 0 && e->keysym.scancode < KEYBOARD_INPUT && e->keysym.scancode >= 0)
	{
		app.keyboard[e->keysym.scancode] = false;
	}
}

void handle_input(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYUP:
				on_key_up(&event.key);
				break;
			case SDL_KEYDOWN:
				on_key_down(&event.key);
				break;
		}
	}
}