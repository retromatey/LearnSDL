#include "common.h"
#include "init.h"

extern App app;

void init_SDL(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		app.window = SDL_CreateWindow("LearnSDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

		if (app.window != NULL)
		{
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
			app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

			if (app.renderer != NULL)
			{
				int img_init = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

				if (img_init < 1)
				{
					error("Failed to init img library");
				}
			}
			else
			{
				error("Failed to init renderer");
			}
		}
		else
		{
			error("Failed to init window");
		}
	}
	else
	{
		error("Failed to init SDL libraries");
	}
}

void destroy(void)
{
	// TODO: destroy textures
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	IMG_Quit();
	SDL_Quit();
}