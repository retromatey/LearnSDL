#include "models.h"

void create_app(App* app)
{
	memset(app, 0, sizeof(App));
	app->game_running = false;

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		app->window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);	// flags is zero here?

		if (app->window)
		{
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
			app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);

			if (app->renderer)
			{
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 1)
				{
					error("Error creating app, failed to initialize imaging");
				}
			}
			else
			{
				error("Error creating app, failed to initialize renderer");
			}
		}
		else
		{
			error("Error creating app, failed to initialize window");
		}
	}
	else
	{
		error("Error creating app, failed to initialize SDL");
	}
}

void destroy_app(App* app)
{
	SDL_DestroyWindow(app->window);
	SDL_DestroyRenderer(app->renderer);
}

void create_entities(App* app, Entity* ship, Entity* missile)
{
	memset(ship, 0, sizeof(Entity));
	memset(missile, 0, sizeof(Entity));

	ship->alive = true;
	ship->x_pos = 100;
	ship->y_pos = 100;
	ship->dx = SPEED_SPACESHIP;
	ship->dy = SPEED_SPACESHIP;

	ship->texture = IMG_LoadTexture(app->renderer, IMG_SPACESHIP);

	if (!ship->texture)
		error("Error creating entities, failed to load ship texture");

	missile->alive = false;
	missile->x_pos = ship->x_pos;
	missile->y_pos = ship->y_pos;
	missile->dx = SPEED_MISSILE;
	missile->dy = SPEED_MISSILE;

	missile->texture = IMG_LoadTexture(app->renderer, IMG_MISSILE);

	if (!missile->texture)
		error("Error creating entities, failed to load missile texture");
}

void destroy_entities(Entity* ship, Entity* missile)
{
	SDL_DestroyTexture(ship->texture);
	SDL_DestroyTexture(missile->texture);
}